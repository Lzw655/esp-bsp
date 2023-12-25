#include "mipi.h"
#include "gdma_struct.h"
#include "interrupt_core0_reg.h"
#include "soc/soc.h"
#include "rom/cache.h"
#include "esp32p4/rom/ets_sys.h"
#include "esp_intr_alloc.h"
#include "riscv/interrupt.h"
#include "esp_log.h"
#include "esp_system.h"
#include "gpio_struct.h"
#include "lp_iomux_struct.h"
#include "iomux_struct.h"
#include "isp.h"
#include "hp_sys_clkrst_reg.h"

#define TEST_MULTI_FRAME        (0)
#define MIPI_LCD_DMA_TRANS_DAR  (0x50105000)
#define MIPI_CAM_DMA_TRANS_SAR  (0x50104000)
#define GDMA_INTR_SOURCE        (((INTERRUPT_CORE0_GDMA_INT_MAP_REG - DR_REG_INTERRUPT_CORE0_BASE) / 4))
#define GDMA_BUFFER_MASTER      (1)

static uint32_t buf_dma_addr[3];
static uint32_t buf_cache_addr[3]; 

volatile uint32_t buf_wr_ptr = 1; // 0,1,2  Updated in ISR only when GDMA finished receive  one frame from MIPI-CSI, and wants to receive  next one.
volatile uint32_t buf_rd_ptr = 0; // 0,1,2  Updated in ISR only when GDMA finished transmit one frame into MIPI-DSI, and wants to transmit next one.
volatile uint32_t csi_frame_cnt = 0;
volatile uint32_t dsi_frame_cnt = 0;

static void delay_us(uint32_t t)
{
    ets_delay_us(t);
}

static void gpio_probe_enable(int io_num)
{
    typeof(LP_IOMUX.pad[0]) *lp_pad;
    typeof(IOMUX.gpio[0]) *hp_pad;
    if (io_num >= 32) {
        hp_pad = (typeof(IOMUX.gpio[0]) *)((uint32_t)&IOMUX.gpio[0] + 4*io_num);
        hp_pad->mcu_oe = 0;
        hp_pad->mcu_sel = 1;
        hp_pad->mcu_ie = 0;
        hp_pad->fun_ie = 1;
        GPIO.enable1_w1ts.val = GPIO.enable1_w1ts.val | (0x1 << (io_num - 32));
    } else {
        if (io_num < 16) {
            lp_pad = (typeof(LP_IOMUX.pad[0]) *)((uint32_t)&LP_IOMUX.pad[0] + 4*io_num);
            lp_pad->mux_sel = 0;
        }
        hp_pad = (typeof(IOMUX.gpio[0]) *)((uint32_t)&IOMUX.gpio[0] + 4*io_num);
        hp_pad->mcu_oe = 0;
        hp_pad->mcu_sel = 1;
        hp_pad->mcu_ie = 0;
        hp_pad->fun_ie = 1;
        GPIO.enable_w1ts = GPIO.enable_w1ts | (0x1 << io_num);
    }
}

void IRAM_ATTR gpio_probe(int io_num, int level)
{
    if (io_num >= 32) {
        if (level == 1) {
            GPIO.out1_w1ts.val = (0x1 << (io_num - 32)); 
        } else {
            GPIO.out1_w1tc.val = (0x1 << (io_num - 32)); 
        }
    } else {
        if (level == 1) {
            GPIO.out_w1ts = (0x1 << io_num); 
        } else {
            GPIO.out_w1tc = (0x1 << io_num); 
        }
    }
}

static void gdma_isr()
{
    uint32_t rtn = 0;

    // printf("[ISR] Enter into GDMA ISR.\n");

    // Clear interrupt.
    rtn = (GDMA.int_st0.val);

    // If is channel 1 interrupt event.
    if (rtn & 0x1) {
        if (GDMA.ch[0].int_st0.val) { // Is channel 1 "transfer done" interrupt event.
            typeof(GDMA.ch[0].int_st0) status = GDMA.ch[0].int_st0;

            if (status.dma_tfr_done) {
                gpio_probe(MIPI_CSI_PROBE_IO, 1);

                if (TEST_MULTI_FRAME) {
                    switch (buf_wr_ptr) {
                        case 0 :
                            buf_wr_ptr = 1;
                            break;

                        case 1 :
                            buf_wr_ptr = 2;
                            break;

                        case 2 :
                            buf_wr_ptr = 0;
                            break;

                        default:
                            ;
                    }
                }

                GDMA.ch[0].dar0 = buf_dma_addr[buf_wr_ptr];
                GDMA.chen0.val = ((GDMA.chen0.val) | 0x101);

                csi_frame_cnt++;
                gpio_probe(MIPI_CSI_PROBE_IO, 0);
            }

            GDMA.ch[0].int_clr0.val = 0xffffffff;
        }
    }

    // If is channel 2 interrupt event.
    if (rtn & 0x2) {
        if (GDMA.ch[1].int_st0.val) { // Is channel 2 "transfer done" interrupt event.
            typeof(GDMA.ch[1].int_st0) status = GDMA.ch[1].int_st0;

            if (status.dma_tfr_done) {
                gpio_probe(MIPI_DSI_PROBE_IO, 1);

                GDMA.ch[1].sar0 = buf_dma_addr[buf_rd_ptr];
                GDMA.chen0.val = ((GDMA.chen0.val) | 0x202);

                dsi_frame_cnt++;
                gpio_probe(MIPI_DSI_PROBE_IO, 0);
            }

            // Clear all interrupt of ch1
            GDMA.ch[1].int_clr0.val = 0xffffffff;
        }
    }
}

static int gdma_init()
{
    uint32_t csi_block_ts;

    if (TEST_CSI_COLOR_MODE >= MIPI_CSI_RAW8_MODE) {
        csi_block_ts = ((MIPI_CSI_DISP_HSIZE * MIPI_CSI_DISP_VSIZE * TEST_DSI_COLOR_WIDTH) / TEST_CSI_TR_WIDTH);
    } else {
        csi_block_ts = ((MIPI_CSI_DISP_HSIZE * MIPI_CSI_DISP_VSIZE * TEST_CSI_COLOR_WIDTH) / TEST_CSI_TR_WIDTH);
    }

    uint32_t dsi_block_ts = ((MIPI_DSI_DISP_HSIZE * MIPI_DSI_DISP_VSIZE * TEST_DSI_COLOR_WIDTH) / TEST_DSI_TR_WIDTH);

    // Enable GDMA.
    GDMA.cfg0.val = 0x0;
    GDMA.cfg0.int_en = 0x1;
    GDMA.cfg0.dmac_en = 0x1;

    GDMA.ch[0].int_st_ena0.val = 0x0;
    GDMA.ch[0].int_sig_ena0.val = 0x0;
    GDMA.ch[0].int_st_ena0.dma_tfr_done = 0x1;
    GDMA.ch[0].int_sig_ena0.dma_tfr_done = 0x1;
    GDMA.ch[0].cfg0.val = 0x0;
    GDMA.ch[0].cfg0.wr_uid = 0x2;
    GDMA.ch[0].cfg0.rd_uid = 0x2;
    GDMA.ch[0].cfg0.src_multblk_type = 0x0;
    GDMA.ch[0].cfg0.dst_multblk_type = 0x0;
    GDMA.ch[0].cfg1.val = 0x0;
    GDMA.ch[0].cfg1.tt_fc = 0x4;
    GDMA.ch[0].cfg1.hs_sel_src = 0x0;
    GDMA.ch[0].cfg1.hs_sel_dst = 0x0;
    GDMA.ch[0].cfg1.src_per = 0x1;
    GDMA.ch[0].cfg1.dst_per = 0x0;
    GDMA.ch[0].cfg1.src_osr_lmt = 0x1;
    GDMA.ch[0].cfg1.dst_osr_lmt = 0x4;
    GDMA.ch[0].sar0 = MIPI_CAM_DMA_TRANS_SAR;
    GDMA.ch[0].dar0 = buf_dma_addr[buf_wr_ptr];
    GDMA.ch[0].block_ts0.val = csi_block_ts - 1;
    GDMA.ch[0].ctl0.val = 0x0;
    GDMA.ch[0].ctl0.sms = 0x0;
    GDMA.ch[0].ctl0.dms = GDMA_BUFFER_MASTER;
    GDMA.ch[0].ctl0.sinc = 0x1;
    GDMA.ch[0].ctl0.dinc = 0x0;
    GDMA.ch[0].ctl0.src_tr_width = (TEST_CSI_TR_WIDTH == 64 ? 0x3 : 0x2);
    GDMA.ch[0].ctl0.dst_tr_width = (TEST_CSI_TR_WIDTH == 64 ? 0x3 : 0x2);
    GDMA.ch[0].ctl0.src_msize = 0x7;
    GDMA.ch[0].ctl0.dst_msize = 0x8;
    GDMA.ch[0].ctl1.ioc_blktfr = 0;
    GDMA.ch[0].sstatar0 = MIPI_CAM_DMA_TRANS_SAR + 16;
    GDMA.ch[0].ctl1.arlen = 16;
    GDMA.ch[0].ctl1.arlen_en = 1;
    GDMA.ch[0].ctl1.awlen = 16;
    GDMA.ch[0].ctl1.awlen_en = 1;

    GDMA.ch[1].int_st_ena0.val = 0x0;
    GDMA.ch[1].int_sig_ena0.val = 0x0;
    GDMA.ch[1].int_st_ena0.dma_tfr_done = 0x1;
    GDMA.ch[1].int_sig_ena0.dma_tfr_done = 0x1;
    GDMA.ch[1].cfg0.val = 0x0;
    GDMA.ch[1].cfg0.wr_uid = 0x3;
    GDMA.ch[1].cfg0.rd_uid = 0x3;
    GDMA.ch[1].cfg0.src_multblk_type = 0x0;
    GDMA.ch[1].cfg0.dst_multblk_type = 0x0;
    GDMA.ch[1].cfg1.val = 0x0;
    GDMA.ch[1].cfg1.tt_fc = 0x6;
    GDMA.ch[1].cfg1.hs_sel_src = 0x0;
    GDMA.ch[1].cfg1.hs_sel_dst = 0x0;
    GDMA.ch[1].cfg1.src_per = 0x0;
    GDMA.ch[1].cfg1.dst_per = 0x0;
    GDMA.ch[1].cfg1.src_osr_lmt = 0x4;
    GDMA.ch[1].cfg1.dst_osr_lmt = 0x1;
    GDMA.ch[1].sar0 = buf_dma_addr[buf_rd_ptr];
    GDMA.ch[1].dar0 = MIPI_LCD_DMA_TRANS_DAR;
    GDMA.ch[1].block_ts0.val =  dsi_block_ts - 1;
    GDMA.ch[1].ctl0.val = 0x0;
    GDMA.ch[1].ctl0.sms = GDMA_BUFFER_MASTER;
    GDMA.ch[1].ctl0.dms = 0x0;
    GDMA.ch[1].ctl0.sinc = 0x0;
    GDMA.ch[1].ctl0.dinc = 0x1;
    GDMA.ch[1].ctl0.src_tr_width = (TEST_DSI_TR_WIDTH == 64 ? 0x3 : 0x2);
    GDMA.ch[1].ctl0.dst_tr_width = (TEST_DSI_TR_WIDTH == 64 ? 0x3 : 0x2);
    GDMA.ch[1].ctl0.src_msize = 0x8;
    GDMA.ch[1].ctl0.dst_msize = 0x7;
    GDMA.ch[1].ctl1.ioc_blktfr = 0;
    GDMA.ch[1].ctl1.arlen = 16;
    GDMA.ch[1].ctl1.arlen_en = 1;
    GDMA.ch[1].ctl1.awlen = 16;
    GDMA.ch[1].ctl1.awlen_en = 1;

    return 0;
}

static int mipi_start(void)
{
#if !TEST_DISP_PATTERN
    GDMA.chen0.val = ((GDMA.chen0.val) | 0x101);
    delay_us(10);

    mipi_csi_bridge_initialization();

    // chip 上 Sensor需要 mipi 初始化完成后再初始化 
    // while (sccb_prob() < 2);
#if TEST_CSI_OV5640
    ov5640_init();
#endif
#if TEST_CSI_OV5647
    ov5647_init();
#if TEST_CSI_AF
    sccb_prob();
    dw9714_init(DW9714_METHOD_DUAL);
#endif
#if TEST_CSI_IMX219
    imx219_init();
#endif
#endif
#else
    typedef struct {
        uint8_t data[TEST_DSI_COLOR_WIDTH / 8];
    } mipi_dsi_data_t; // 8bit 16bit 24bit 32bit 数据测试
#if (TEST_DSI_COLOR_MODE == MIPI_DSI_RGB888_MODE)
#define RED   (MIPI_DSI_RGB888_RED)
#define GREEN (MIPI_DSI_RGB888_GREEN)
#define BLUE  (MIPI_DSI_RGB888_BLUE)
#else
#define RED   (MIPI_DSI_RGB565_RED)
#define GREEN (MIPI_DSI_RGB565_GREEN)
#define BLUE  (MIPI_DSI_RGB565_BLUE)
#endif
    mipi_dsi_data_t *tx_data = (mipi_dsi_data_t *)buf_cache_addr[buf_rd_ptr];
    mipi_dsi_data_t line[MIPI_DSI_DISP_HSIZE];
    uint32_t dsi_size = sizeof(mipi_dsi_data_t);
    uint32_t data = 0;
#if 1

    for (int y = 0; y < MIPI_DSI_DISP_VSIZE / 2; y++) {
        for (int x = 0; x < MIPI_DSI_DISP_HSIZE; x++) {
            if (y < (MIPI_DSI_DISP_VSIZE / 3)) {
                data = RED;
            } else if (y < (MIPI_DSI_DISP_VSIZE / 3) * 2) {
                data = GREEN;
            } else {
                data = BLUE;
            }

            memcpy(&line[x], &data, dsi_size);
        }

        memcpy(&tx_data[y * MIPI_DSI_DISP_HSIZE], line, MIPI_DSI_DISP_HSIZE * (TEST_DSI_COLOR_WIDTH / 8));
    }

    for (int y = MIPI_DSI_DISP_VSIZE / 2; y < MIPI_DSI_DISP_VSIZE; y++) {
        for (int x = 0; x < MIPI_DSI_DISP_HSIZE; x++) {
            if (x < (MIPI_DSI_DISP_HSIZE / 3)) {
                data = RED;
            } else if (x < (MIPI_DSI_DISP_HSIZE / 3) * 2) {
                data = GREEN;
            } else {
                data = BLUE;
            }

            memcpy(&line[x], &data, dsi_size);
        }

        memcpy(&tx_data[y * MIPI_DSI_DISP_HSIZE], line, MIPI_DSI_DISP_HSIZE * (TEST_DSI_COLOR_WIDTH / 8));
    }
    esp_spiram_writeback_cache();
#else

    for (int x = 0; x < MIPI_DSI_DISP_VSIZE * MIPI_DSI_DISP_HSIZE; x++) {

        data = 0x000000;

        memcpy(&tx_data[x], &data, dsi_size);
    }

#endif
#endif
    // Enable GDMA CH-2 transfer.
    GDMA.chen0.val = ((GDMA.chen0.val) | 0x202);
    delay_us(10);

    MIPI_DSI_BRIDGE.dma_flow_ctrl.dsi_dma_flow_controller = 1;
    MIPI_DSI_BRIDGE.dma_req_cfg.dma_burst_len = 256;
    MIPI_DSI_BRIDGE.raw_buf_almost_empty_thrd.dsi_raw_buf_almost_empty_thrd = 1024 - MIPI_DSI_BRIDGE.dma_req_cfg.dma_burst_len;
    // Enable MIPI DSI.
    MIPI_DSI_BRIDGE.en.dsi_en = 0x1;
    // MIPI_DSI_BRIDGE.dpi_rsv_dpi_data.dpi_rsv_data = 0xFFFFFF;
    // Enable MIPI DPI_EN.
    MIPI_DSI_BRIDGE.dpi_misc_config.dpi_en = 0x1;
    MIPI_DSI_BRIDGE.dpi_config_update.val  = 0x1; // Remember to do update.
    printf("[MAIN] MIPI transfer start.\n");

    return 0;
}

int mipi_init(uint32_t dma_addr, uint32_t cache_addr)
{
    int stat;

    REG_SET_FIELD(HP_SYS_CLKRST_PERI_CLK_CTRL02_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPHY_CLK_SRC_SEL, 1);
    REG_SET_FIELD(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_CSI_DPHY_CLK_SRC_SEL, 1);
    REG_CLR_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPHY_CFG_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPHY_CFG_CLK_EN);
    REG_CLR_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPHY_PLL_REFCLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPHY_PLL_REFCLK_EN);
    REG_CLR_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_CSI_DPHY_CFG_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_CSI_DPHY_CFG_CLK_EN);

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_DSI_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_DSI_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_DSI_BRG);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_DSI_BRG);

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_CSI_HOST_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_CSI_HOST_SYS_CLK_EN);
    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_CSI_BRG_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_CSI_BRG_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_CSI_HOST);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_CSI_HOST);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_CSI_BRG);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_CSI_BRG);

    REG_SET_FIELD(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPICLK_DIV_NUM, (240000000 / MIPI_DPI_CLOCK_RATE) - 1);
    REG_SET_FIELD(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPICLK_SRC_SEL, 1);
    REG_SET_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPICLK_EN); 

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_GDMA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_GDMA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_GDMA);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_GDMA);

    REG_SET_FIELD(HP_SYS_CLKRST_PERI_CLK_CTRL26_REG, HP_SYS_CLKRST_REG_ISP_CLK_DIV_NUM, (240000000 / 80000000) - 1);
    REG_SET_FIELD(HP_SYS_CLKRST_PERI_CLK_CTRL25_REG, HP_SYS_CLKRST_REG_ISP_CLK_SRC_SEL, 1);
    REG_CLR_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL25_REG, HP_SYS_CLKRST_REG_ISP_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL25_REG, HP_SYS_CLKRST_REG_ISP_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_ISP);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_ISP);

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_DMA2D_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_DMA2D_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_DMA2D);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_DMA2D);

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_PPA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_PPA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_PPA);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_PPA);

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_JPEG_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_JPEG_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_JPEG);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_JPEG);

    gpio_probe_enable(MIPI_CSI_PROBE_IO);
    gpio_probe_enable(MIPI_DSI_PROBE_IO);
    gpio_probe_enable(MIPI_TP_RST_IO);
    gpio_probe_enable(MIPI_LCD_RST_IO);
    gpio_probe_enable(MIPI_LCD_LED_IO);
    gpio_probe(MIPI_TP_RST_IO, 0);
    gpio_probe(MIPI_LCD_RST_IO, 0);
    gpio_probe(MIPI_LCD_LED_IO, 0);

    delay_us(200000);

    gpio_probe(MIPI_TP_RST_IO, 1);
    gpio_probe(MIPI_LCD_RST_IO, 1);
    gpio_probe(MIPI_LCD_LED_IO, 1);

    sccb_init(MIPI_SCCB_FRE, MIPI_SCCB_SCL_IO, MIPI_SCCB_SDA_IO);

    isp_init();

    // ---------------------------------------
    // 1. Init of MIPI-CAM.
    // ---------------------------------------
    stat = mipi_csi_initialization();

    if (stat) {
        printf("[MIPI] Error - Initialize MIPI CSI failed.\n");
        return -1;
    }

    // ---------------------------------------
    // 2. Init of MIPI-LCD.
    // ---------------------------------------
    stat = mipi_dsi_initialization();
    if(stat) {
        printf("[MIPI] Error - Initialize MIPI DSI failed.\n");
        return -1;
    }

    delay_us(500);

    buf_dma_addr[0]   = dma_addr;
    buf_cache_addr[0] = cache_addr;
    buf_dma_addr[1]   = buf_dma_addr[0];
    buf_cache_addr[1] = buf_cache_addr[0];

    gdma_init();

    intr_handle_t intr_handle;

    esp_intr_alloc(GDMA_INTR_SOURCE, ESP_INTR_FLAG_LEVEL2, gdma_isr, NULL, &intr_handle);

    memset(cache_addr, 0xFF, (MIPI_DSI_DISP_HSIZE * MIPI_DSI_DISP_VSIZE * (TEST_DSI_COLOR_WIDTH / 8)));
    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE | CACHE_MAP_L1_DCACHE, cache_addr, (MIPI_DSI_DISP_HSIZE * MIPI_DSI_DISP_VSIZE * (TEST_DSI_COLOR_WIDTH / 8)));

    mipi_start();

#if 0 
    while (dsi_frame_cnt < 1000) {
        printf("csi_frame_cnt: %d, dsi_frame_cnt: %d\n", csi_frame_cnt, dsi_frame_cnt);
        printf("csi_bridge_raw: 0x%x, buf_depth,: %d, overrun: %d, discard: %d\n", MIPI_CSI_BRIDGE.int_raw.val, MIPI_CSI_BRIDGE.buf_flow_ctl.csi_buf_depth, MIPI_CSI_BRIDGE.int_raw.csi_buf_overrun, MIPI_CSI_BRIDGE.int_raw.discard);
        printf("dsi_bridge_raw: 0x%x, raw_buf_depth: %d\n", MIPI_DSI_BRIDGE.int_raw.val, MIPI_DSI_BRIDGE.fifo_flow_status.raw_buf_depth);
        printf("isp_raw: 0x%x, data_type_err,: %d, data_type_setting_err: %d, async_fifo_ovf: %d\n", ISP.int_raw.val, ISP.int_raw.data_type_err, ISP.int_raw.data_type_setting_err, ISP.int_raw.async_fifo_ovf);
        printf("MIPI_CSI_HOST.stopstate[0x%x]: 0x%x\n", &MIPI_CSI_HOST.phy_stopstate.val, MIPI_CSI_HOST.phy_stopstate.val);
        printf("MIPI_CSI_HOST.phy_rx[0x%x]: 0x%x\n", &MIPI_CSI_HOST.phy_rx.val, MIPI_CSI_HOST.phy_rx.val);
        printf("MIPI_CSI_HOST.int_st_main[0x%x]: 0x%x\n", &MIPI_CSI_HOST.int_st_main.val, MIPI_CSI_HOST.int_st_main.val);
        printf("MIPI_CSI_HOST.int_st_phy[0x%x]: 0x%x\n", &MIPI_CSI_HOST.int_st_phy.val, MIPI_CSI_HOST.int_st_phy.val);
        printf("MIPI_CSI_HOST.int_st_phy_fatal[0x%x]: 0x%x\n", &MIPI_CSI_HOST.int_st_phy_fatal.val, MIPI_CSI_HOST.int_st_phy_fatal.val);  
        printf("MIPI_CSI_HOST.int_st_bndry_frame_fatal[0x%x]: 0x%x\n", &MIPI_CSI_HOST.int_st_bndry_frame_fatal.val, MIPI_CSI_HOST.int_st_bndry_frame_fatal.val);  
        printf("MIPI_DSI_HOST.int_st0[0x%x]: 0x%x\n", &MIPI_DSI_HOST.int_st0.val, MIPI_DSI_HOST.int_st0.val); 
        printf("MIPI_DSI_HOST.int_st1[0x%x]: 0x%x\n", &MIPI_DSI_HOST.int_st1.val, MIPI_DSI_HOST.int_st1.val); 
        delay_us(2000);
    }
#endif

    return 0;
}