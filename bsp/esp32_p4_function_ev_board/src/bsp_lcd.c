/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "esp_check.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "esp_intr_alloc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "interrupt_core0_reg.h"
#include "hp_sys_clkrst_reg.h"
#include "mipi_dsi_host_struct.h"
#include "gdma_struct.h"

#include "mipi_dsi.h"
#include "bsp_lcd.h"

#define TEST_DISP_PATTERN   (0)

#define MIPI_LCD_DMA_TRANS_DAR  (0x50105000)
#define GDMA_INTR_SOURCE        (((INTERRUPT_CORE0_GDMA_INT_MAP_REG - DR_REG_INTERRUPT_CORE0_BASE) / 4))
#define GDMA_BUFFER_MASTER      (1)

static const char *TAG = "bsp_lcd";

volatile uint8_t *dsi_frame_buf = NULL;
volatile uint32_t dsi_frame_cnt = 0;

static void gdma_isr()
{
    uint32_t rtn = 0;

    // ESP_LOGI(TAG, "[ISR] Enter into GDMA ISR.\n");

    // Clear interrupt.
    rtn = (GDMA.int_st0.val);

    // If is channel 2 interrupt event.
    if (rtn & 0x2) {
        if (GDMA.ch[1].int_st0.val) { // Is channel 2 "transfer done" interrupt event.
            typeof(GDMA.ch[1].int_st0) status = GDMA.ch[1].int_st0;

            if (status.dma_tfr_done) {
                // gpio_probe(MIPI_DSI_PROBE_IO, 1);

                GDMA.ch[1].sar0 = (uint32_t)dsi_frame_buf;
                GDMA.chen0.val = ((GDMA.chen0.val) | 0x202);

                // dsi_frame_cnt++;
                // gpio_probe(MIPI_DSI_PROBE_IO, 0);
            }

            // Clear all interrupt of ch1
            GDMA.ch[1].int_clr0.val = 0xffffffff;
        }
    }
}

esp_err_t bsp_lcd_init(void)
{
    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_GDMA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_GDMA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_GDMA);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_GDMA);

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_DMA2D_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_DMA2D_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_DMA2D);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_DMA2D);

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_PPA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_PPA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_PPA);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_PPA);

    dsi_frame_buf = heap_caps_aligned_alloc(TEST_DSI_TR_WIDTH, MIPI_DSI_DISP_BUF_SIZE, MALLOC_CAP_SPIRAM);
    assert(dsi_frame_buf != NULL);

    uint32_t dsi_block_ts = (MIPI_DSI_DISP_BUF_SIZE / TEST_DSI_TR_WIDTH);

    // Enable GDMA.
    GDMA.cfg0.val = 0x0;
    GDMA.cfg0.int_en = 0x1;
    GDMA.cfg0.dmac_en = 0x1;

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
    GDMA.ch[1].sar0 = (uint32_t)dsi_frame_buf;
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

    intr_handle_t intr_handle;

    esp_intr_alloc(GDMA_INTR_SOURCE, ESP_INTR_FLAG_LEVEL2, gdma_isr, NULL, &intr_handle);

    return ESP_OK;
}

esp_err_t bsp_lcd_start(void)
{
    mipi_dsih_hal_gen_cmd_mode_en(1);

#if !TEST_DSI_LOOPBACK
    MIPI_DSI_HOST.lpclk_ctrl.val = 0x00000000 |
                                   ((0 & 0x00000001) << 1) | // bit[    1] AUTO_CLKLANE_CTRL     = 0 (Automatic mechanism to stop providing clock in the clock lane when time allows.
                                   ((0 & 0x00000001) << 0);  // bit[    0] TXREQUESTCLKHS        = 1 (Requesting High Speed Clock Transmission.
    // MIPI_DSI_HOST.phy_ulps_ctrl.phy_txrequlpsclk = 1;

    // RDDID: Read Display ID(DAH)
    // This read byte returns 24-bit display identification information.
    // The 1 parameter (ID1): the module’s manufacture ID.
    // The 2 parameter (ID2): the module/driver version ID.
    // The 3 parameter (ID3): the module/driver ID
    uint8_t ID1, ID2, ID3;
    mipi_dcs_read_cmd(0xDA, 3, &ID1, &ID2, &ID3);
    ESP_LOGI(TAG, "ID1: 0x%x, ID2: 0x%x, ID3: 0x%x\n", ID1, ID2, ID3);

#if TEST_DSI_ST7796
    mipi_dcs_write_cmd(0x11, 0);         //Sleep Out
    vTaskDelay(pdMS_TO_TICKS(200));
    mipi_dcs_write_cmd(0xf0, 1, 0xc3); //enable command 2 part 1
    mipi_dcs_write_cmd(0xf0, 1, 0x96); //enable command 2 part 2
    mipi_dcs_write_cmd(0x36, 1, 0x48);

    mipi_dcs_write_cmd(0x3a, 1, 0x75);         //16bit pixel

    mipi_dcs_write_cmd(0xb4, 1, 0x01);
    mipi_dcs_write_cmd(0xb7, 1, 0xc6);
    mipi_dcs_write_cmd(0xe8, 8, 0x40, 0x8a, 0x00, 0x00, 0x29, 0x19, 0xa5, 0x33);
    mipi_dcs_write_cmd(0xc1, 5, 0x06, 0xc2, 0xa7, 0xc5, 0x18);
    mipi_dcs_write_cmd(0xe0, 14, 0xf0, 0x09, 0x0b, 0x06, 0x04, 0x15, 0x2f, 0x54, 0x42, 0x3c, 0x17, 0x14, 0x18, 0x1b);

    //Negative Voltage Gamma Coltrol
    mipi_dcs_write_cmd(0xe1, 14, 0xf0, 0x09, 0x0b, 0x06, 0x04, 0x03, 0x2d, 0x43, 0x42, 0x3b, 0x16, 0x14, 0x17, 0x1b);

    mipi_dcs_write_cmd(0xf0, 1, 0x3c);
    mipi_dcs_write_cmd(0xf0, 1, 0x69);

    mipi_dcs_write_cmd(0x13, 0);
    mipi_dcs_write_cmd(0x38, 0); //Normal mode
    vTaskDelay(pdMS_TO_TICKS(120));
    mipi_dcs_write_cmd(0x29, 0); //Display ON

#endif

#if TEST_DSI_NT35516
    mipi_dcs_write_cmd(0xFF, 4, 0xAA, 0x55, 0x25, 0x01);
    mipi_dcs_write_cmd(0xF0, 5, 0x55, 0xAA, 0x52, 0x08, 0x00);
    mipi_dcs_write_cmd(0xB1, 1, 0xFC);
    mipi_dcs_write_cmd(0xB8, 4, 0x01, 0x02, 0x02, 0x02);
    mipi_dcs_write_cmd(0xBC, 3, 0x05, 0x05, 0x05);
    mipi_dcs_write_cmd(0xB7, 2, 0x00, 0x00);
    mipi_dcs_write_cmd(0xC8, 18, 0x01, 0x00, 0x46, 0x1E, 0x46, 0x1E, 0x46, 0x1E, 0x46, 0x1E, 0x64, 0x3C, 0x3C, 0x64, 0x64, 0x3C, 0x3C, 0x64);
    mipi_dcs_write_cmd(0xF0, 5, 0xAA, 0x55, 0x52, 0x08, 0x01);
    mipi_dcs_write_cmd(0xB0, 3, 0x05, 0x05, 0x05);
    mipi_dcs_write_cmd(0xB6, 3, 0x44, 0x44, 0x44);
    mipi_dcs_write_cmd(0xB1, 3, 0x05, 0x05, 0x05);
    mipi_dcs_write_cmd(0xB7, 3, 0x34, 0x34, 0x34);
    mipi_dcs_write_cmd(0xB3, 3, 0x16, 0x16, 0x16);
    mipi_dcs_write_cmd(0xB4, 3, 0x0A, 0x0A, 0x0A);
    mipi_dcs_write_cmd(0xBC, 3, 0x00, 0x90, 0x11);
    mipi_dcs_write_cmd(0xBD, 3, 0x00, 0x90, 0x11);
    mipi_dcs_write_cmd(0xBE, 1, 0x51);
    mipi_dcs_write_cmd(0xD1, 16, 0x00, 0x17, 0x00, 0x24, 0x00, 0x3D, 0x00, 0x52, 0x00, 0x66, 0x00, 0x86, 0x00, 0xA0, 0x00, 0xCC);
    mipi_dcs_write_cmd(0xD2, 16, 0x00, 0xF1, 0x01, 0x26, 0x01, 0x4E, 0x01, 0x8C, 0x01, 0xBC, 0x01, 0xBE, 0x01, 0xE7, 0x02, 0x0E);
    mipi_dcs_write_cmd(0xD3, 16, 0x02, 0x22, 0x02, 0x3C, 0x02, 0x4F, 0x02, 0x71, 0x02, 0x90, 0x02, 0xC6, 0x02, 0xF1, 0x03, 0x3A);
    mipi_dcs_write_cmd(0xD4, 4, 0x03, 0xB15, 0x03, 0xC1);
    mipi_dcs_write_cmd(0xD5, 16, 0x00, 0x17, 0x00, 0x24, 0x00, 0x3D, 0x00, 0x52, 0x00, 0x66, 0x00, 0x86, 0x00, 0xA0, 0x00, 0xCC);
    mipi_dcs_write_cmd(0xD6, 16, 0x00, 0xF1, 0x01, 0x26, 0x01, 0x4E, 0x01, 0x8C, 0x01, 0xBC, 0x01, 0xBE, 0x01, 0xE7, 0x02, 0x0E);
    mipi_dcs_write_cmd(0xD7, 16, 0x02, 0x22, 0x02, 0x3C, 0x02, 0x4F, 0x02, 0x71, 0x02, 0x90, 0x02, 0xC6, 0x02, 0xF1, 0x03, 0x3A);
    mipi_dcs_write_cmd(0xD8, 4, 0x03, 0xB5, 0x03, 0xC1);
    mipi_dcs_write_cmd(0xD9, 16, 0x00, 0x17, 0x00, 0x24, 0x00, 0x3D, 0x00, 0x52, 0x00, 0x66, 0x00, 0x86, 0x00, 0xA0, 0x00, 0xCC);
    mipi_dcs_write_cmd(0xDD, 16, 0x00, 0xF1, 0x01, 0x26, 0x01, 0x4E, 0x01, 0x8C, 0x01, 0xBC, 0x01, 0xBE, 0x01, 0xE7, 0x02, 0x0E);
    mipi_dcs_write_cmd(0xDE, 16, 0x02, 0x22, 0x02, 0x3C, 0x02, 0x4F, 0x02, 0x71, 0x02, 0x90, 0x02, 0xC6, 0x02, 0xF1, 0x03, 0x3A);
    mipi_dcs_write_cmd(0xDF, 4, 0x03, 0xB5, 0x03, 0xC1);
    mipi_dcs_write_cmd(0xE0, 16, 0x00, 0x17, 0x00, 0x24, 0x00, 0x3D, 0x00, 0x52, 0x00, 0x66, 0x00, 0x86, 0x00, 0xA0, 0x00, 0xCC);
    mipi_dcs_write_cmd(0xE1, 16, 0x00, 0xF1, 0x01, 0x26, 0x01, 0x4E, 0x01, 0x8C, 0x01, 0xBC, 0x01, 0xBE, 0x01, 0xE7, 0x02, 0x0E);
    mipi_dcs_write_cmd(0xE2, 16, 0x02, 0x22, 0x02, 0x3C, 0x02, 0x4F, 0x02, 0x71, 0x02, 0x90, 0x02, 0xC6, 0x02, 0xF1, 0x03, 0x3A);
    mipi_dcs_write_cmd(0xE3, 4, 0x03, 0xB5, 0x03, 0xC1);
    mipi_dcs_write_cmd(0xE4, 16, 0x00, 0x17, 0x00, 0x24, 0x00, 0x3D, 0x00, 0x52, 0x00, 0x66, 0x00, 0x86, 0x00, 0xA0, 0x00, 0xCC);
    mipi_dcs_write_cmd(0xE5, 16, 0x00, 0xF1, 0x01, 0x26, 0x01, 0x4E, 0x01, 0x8C, 0x01, 0xBC, 0x01, 0xBE, 0x01, 0xE7, 0x02, 0x0E);
    mipi_dcs_write_cmd(0xE6, 16, 0x02, 0x22, 0x02, 0x3C, 0x02, 0x4F, 0x02, 0x71, 0x02, 0x90, 0x02, 0xC6, 0x02, 0xF1, 0x03, 0x3A);
    mipi_dcs_write_cmd(0xE7, 4, 0x03, 0xB5, 0x03, 0xC1);
    mipi_dcs_write_cmd(0xE8, 16, 0x00, 0x17, 0x00, 0x24, 0x00, 0x3D, 0x00, 0x52, 0x00, 0x66, 0x00, 0x86, 0x00, 0xA0, 0x00, 0xCC);
    mipi_dcs_write_cmd(0xE9, 16, 0x00, 0xF1, 0x01, 0x26, 0x01, 0x4E, 0x01, 0x8C, 0x01, 0xBC, 0x01, 0xBE, 0x01, 0xE7, 0x02, 0x0E);
    mipi_dcs_write_cmd(0xEA, 16, 0x02, 0x22, 0x02, 0x3C, 0x02, 0x4F, 0x02, 0x71, 0x02, 0x90, 0x02, 0xC6, 0x02, 0xF1, 0x03, 0x3A);
    mipi_dcs_write_cmd(0xEB, 4, 0x03, 0xB5, 0x03, 0xC1);
    mipi_dcs_write_cmd(0x35, 1, 0x0);
    mipi_dcs_write_cmd(0x11, 0);
    vTaskDelay(pdMS_TO_TICKS(200));
    mipi_dcs_write_cmd(0x29, 0);
    mipi_dcs_write_cmd(0x30, 4, 0x0, 0x0, (MIPI_DSI_IMAGE_VSIZE - 1) >> 8, (MIPI_DSI_IMAGE_VSIZE - 1) & 0xFF);
    mipi_dcs_write_cmd(0x36, 1, 0x00);
    mipi_dcs_write_cmd(0x38, 0);
    mipi_dcs_write_cmd(0x3A, 1, 0x75);
    mipi_dcs_write_cmd(0x12, 0);
    // mipi_dcs_write_cmd(0x13, 0);

#endif

#if TEST_DSI_ILI9806E
    mipi_dcs_write_cmd(0xFF, 0x5, 0xFF, 0x98, 0x06, 0x04, 0x00); // Change to Page 0
    mipi_dcs_write_cmd(0x20, 0x1, 0x00);

    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x01); // Change to Page 1
    mipi_dcs_write_cmd(0x08, 1, 0x10);                         // output SDA
    mipi_dcs_write_cmd(0x21, 1, 0x01);                         // DE = 1 Active
    mipi_dcs_write_cmd(0x30, 1, 0x02);                         // 480 X 800
    mipi_dcs_write_cmd(0x31, 1, 0x02);                         // 2-Dot Inversion
    mipi_dcs_write_cmd(0x60, 1, 0x07);                         // SDTI
    mipi_dcs_write_cmd(0x61, 1, 0x06);                         // CRTI
    mipi_dcs_write_cmd(0x62, 1, 0x06);                         // EQTI
    mipi_dcs_write_cmd(0x63, 1, 0x04);                         // PCTI
    mipi_dcs_write_cmd(0x40, 1, 0x14);                         // BT  +2.5/-2.5 pump for DDVDH-L
    mipi_dcs_write_cmd(0x41, 1, 0x55);                         // DVDDH DVDDL clamp
    mipi_dcs_write_cmd(0x42, 1, 0x11);                         // VGH/VGL
    mipi_dcs_write_cmd(0x43, 1, 0x09);                         // VGH clamp
    mipi_dcs_write_cmd(0x44, 1, 0x0C);                         // VGL clamp
    mipi_dcs_write_cmd(0x45, 1, 0x14);                         // VGL_REG  -10V
    mipi_dcs_write_cmd(0x50, 1, 0x50);                         // VGMP
    mipi_dcs_write_cmd(0x51, 1, 0x50);                         // VGMN
    mipi_dcs_write_cmd(0x52, 1, 0x00);                         //Flicker
    mipi_dcs_write_cmd(0x53, 1, 0x47);                         //Flicker4F

    //++++++++++++++++++ Gamma Setting ++++++++++++++++++//

    mipi_dcs_write_cmd(0xA0, 1, 0x00);  // Gamma 0 /255
    mipi_dcs_write_cmd(0xA1, 1, 0x09);  // Gamma 4 /251
    mipi_dcs_write_cmd(0xA2, 1, 0x0C);  // Gamma 8 /247
    mipi_dcs_write_cmd(0xA3, 1, 0x0F);  // Gamma 16    /239
    mipi_dcs_write_cmd(0xA4, 1, 0x06);  // Gamma 24 /231
    mipi_dcs_write_cmd(0xA5, 1, 0x09);  // Gamma 52 / 203
    mipi_dcs_write_cmd(0xA6, 1, 0x07);  // Gamma 80 / 175
    mipi_dcs_write_cmd(0xA7, 1, 0x016); // Gamma 108 /147
    mipi_dcs_write_cmd(0xA8, 1, 0x06);  // Gamma 147 /108
    mipi_dcs_write_cmd(0xA9, 1, 0x09);  // Gamma 175 / 80
    mipi_dcs_write_cmd(0xAA, 1, 0x11);  // Gamma 203 / 52
    mipi_dcs_write_cmd(0xAB, 1, 0x06);  // Gamma 231 / 24
    mipi_dcs_write_cmd(0xAC, 1, 0x0E);  // Gamma 239 / 16
    mipi_dcs_write_cmd(0xAD, 1, 0x19);  // Gamma 247 / 8
    mipi_dcs_write_cmd(0xAE, 1, 0x0C);  // Gamma 251 / 4
    mipi_dcs_write_cmd(0xAF, 1, 0x00);  // Gamma 255 / 0

    ///==============Nagitive
    mipi_dcs_write_cmd(0xC0, 1, 0x00); // Gamma 0 /255
    mipi_dcs_write_cmd(0xC1, 1, 0x09); // Gamma 4  /251
    mipi_dcs_write_cmd(0xC2, 1, 0x0C); // Gamma 8  /247
    mipi_dcs_write_cmd(0xC3, 1, 0x0F); // Gamma 16  /239
    mipi_dcs_write_cmd(0xC4, 1, 0x06); // Gamma 24  /231
    mipi_dcs_write_cmd(0xC5, 1, 0x09); // Gamma 52  /203
    mipi_dcs_write_cmd(0xC6, 1, 0x07); // Gamma 80  /175
    mipi_dcs_write_cmd(0xC7, 1, 0x16); // Gamma 108  /147
    mipi_dcs_write_cmd(0xC8, 1, 0x06); // Gamma 147  /108
    mipi_dcs_write_cmd(0xC9, 1, 0x09); // Gamma 175  /80
    mipi_dcs_write_cmd(0xCA, 1, 0x11); // Gamma 203  /52
    mipi_dcs_write_cmd(0xCB, 1, 0x06); // Gamma 231  /24
    mipi_dcs_write_cmd(0xCC, 1, 0x0E); // Gamma 239  /16
    mipi_dcs_write_cmd(0xCD, 1, 0x19); // Gamma 247  /8
    mipi_dcs_write_cmd(0xCE, 1, 0x0C); // Gamma 251  /4
    mipi_dcs_write_cmd(0xCF, 1, 0x00); // Gamma 255

    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x07); // Change to Page 7

    mipi_dcs_write_cmd(0x17, 1, 0x32);
    vTaskDelay(pdMS_TO_TICKS(10));
    mipi_dcs_write_cmd(0x06, 1, 0x13);
    vTaskDelay(pdMS_TO_TICKS(10));

    vTaskDelay(pdMS_TO_TICKS(10));
    mipi_dcs_write_cmd(0x02, 1, 0x77);
    vTaskDelay(pdMS_TO_TICKS(10));

    mipi_dcs_write_cmd(0x18, 1, 0x1D);
    vTaskDelay(pdMS_TO_TICKS(10));
    mipi_dcs_write_cmd(0xE1, 1, 0x79);
    vTaskDelay(pdMS_TO_TICKS(10));

    //+++++++++++++++++++++++++++++++++++++++++++++++++++//

    //****************************************************************************//
    //****************************** Page 6 Command ******************************//
    //****************************************************************************//
    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x06); // Change to Page 6
    mipi_dcs_write_cmd(0x00, 1, 0xA0);
    mipi_dcs_write_cmd(0x01, 1, 0x05);
    mipi_dcs_write_cmd(0x02, 1, 0x00);
    mipi_dcs_write_cmd(0x03, 1, 0x00);
    mipi_dcs_write_cmd(0x04, 1, 0x01);
    mipi_dcs_write_cmd(0x05, 1, 0x01);
    mipi_dcs_write_cmd(0x06, 1, 0x88);
    mipi_dcs_write_cmd(0x07, 1, 0x04);
    mipi_dcs_write_cmd(0x08, 1, 0x01);
    mipi_dcs_write_cmd(0x09, 1, 0x90);
    mipi_dcs_write_cmd(0x0A, 1, 0x04);
    mipi_dcs_write_cmd(0x0B, 1, 0x01);
    mipi_dcs_write_cmd(0x0C, 1, 0x01);
    mipi_dcs_write_cmd(0x0D, 1, 0x01);
    mipi_dcs_write_cmd(0x0E, 1, 0x00);
    mipi_dcs_write_cmd(0x0F, 1, 0x00);
    mipi_dcs_write_cmd(0x10, 1, 0x55);
    mipi_dcs_write_cmd(0x11, 1, 0x50);
    mipi_dcs_write_cmd(0x12, 1, 0x01);
    mipi_dcs_write_cmd(0x13, 1, 0x85);
    mipi_dcs_write_cmd(0x14, 1, 0x85);
    mipi_dcs_write_cmd(0x15, 1, 0xC0);
    mipi_dcs_write_cmd(0x16, 1, 0x0B);
    mipi_dcs_write_cmd(0x17, 1, 0x00);
    mipi_dcs_write_cmd(0x18, 1, 0x00);
    mipi_dcs_write_cmd(0x19, 1, 0x00);
    mipi_dcs_write_cmd(0x1A, 1, 0x00);
    mipi_dcs_write_cmd(0x1B, 1, 0x00);
    mipi_dcs_write_cmd(0x1C, 1, 0x00);
    mipi_dcs_write_cmd(0x1D, 1, 0x00);

    mipi_dcs_write_cmd(0x20, 1, 0x01);
    mipi_dcs_write_cmd(0x21, 1, 0x23);
    mipi_dcs_write_cmd(0x22, 1, 0x45);
    mipi_dcs_write_cmd(0x23, 1, 0x67);
    mipi_dcs_write_cmd(0x24, 1, 0x01);
    mipi_dcs_write_cmd(0x25, 1, 0x23);
    mipi_dcs_write_cmd(0x26, 1, 0x45);
    mipi_dcs_write_cmd(0x27, 1, 0x67);

    mipi_dcs_write_cmd(0x30, 1, 0x02);
    mipi_dcs_write_cmd(0x31, 1, 0x22);
    mipi_dcs_write_cmd(0x32, 1, 0x11);
    mipi_dcs_write_cmd(0x33, 1, 0xAA);
    mipi_dcs_write_cmd(0x34, 1, 0xBB);
    mipi_dcs_write_cmd(0x35, 1, 0x66);
    mipi_dcs_write_cmd(0x36, 1, 0x00);
    mipi_dcs_write_cmd(0x37, 1, 0x22);
    mipi_dcs_write_cmd(0x38, 1, 0x22);
    mipi_dcs_write_cmd(0x39, 1, 0x22);
    mipi_dcs_write_cmd(0x3A, 1, 0x22);
    mipi_dcs_write_cmd(0x3B, 1, 0x22);
    mipi_dcs_write_cmd(0x3C, 1, 0x22);
    mipi_dcs_write_cmd(0x3D, 1, 0x22);
    mipi_dcs_write_cmd(0x3E, 1, 0x22);
    mipi_dcs_write_cmd(0x3F, 1, 0x22);
    mipi_dcs_write_cmd(0x40, 1, 0x22);
    mipi_dcs_write_cmd(0x52, 1, 0x12);
    mipi_dcs_write_cmd(0x53, 1, 0x12); //VGLO refer VGL_REG   1A

    // //****************************************************************************//
    // //****************************** Page 0 Command ******************************//
    // //****************************************************************************//

    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x00); // Change to Page 0

    mipi_dcs_write_cmd(0x11, 1, 0x00); // Sleep-Out
    vTaskDelay(pdMS_TO_TICKS(120));
    mipi_dcs_write_cmd(0x29, 1, 0x00); // Display On

#endif

#if TEST_DSI_ILI9881C
    mipi_dcs_write_cmd(0xFF, 0x03, 0x98, 0x81, 0x01);

    mipi_dcs_read_cmd(0x00, 1, &ID1);
    mipi_dcs_read_cmd(0x01, 1, &ID2);
    mipi_dcs_read_cmd(0x02, 1, &ID3);
    ESP_LOGI(TAG, "ID1: 0x%x, ID2: 0x%x, ID3: 0x%x\n", ID1, ID2, ID3);

    //800x1280
    //VS_VBP_VFP=2/16/16
    //HS_HBP_HFP=4/138/24

    mipi_dcs_write_cmd(0xFF, 0x03, 0x98, 0x81, 0x03);

    //GIP_1
    mipi_dcs_write_cmd(0x01, 0x01, 0x00);
    mipi_dcs_write_cmd(0x02, 0x01, 0x00);
    mipi_dcs_write_cmd(0x03, 0x01, 0x5D);        //STVA
    mipi_dcs_write_cmd(0x04, 0x01, 0x17);        //STVB
    mipi_dcs_write_cmd(0x05, 0x01, 0x00);        //STVC
    mipi_dcs_write_cmd(0x06, 0x01, 0x0E);        //STVA_Rise
    mipi_dcs_write_cmd(0x07, 0x01, 0x00);        //STVB_Rise
    mipi_dcs_write_cmd(0x08, 0x01, 0x00);        //STVC_Rise
    mipi_dcs_write_cmd(0x09, 0x01, 0x21);        //FTI1R(A) 21
    mipi_dcs_write_cmd(0x0a, 0x01, 0x21);        //FTI2R(B)
    mipi_dcs_write_cmd(0x0b, 0x01, 0x00);        //FTI3R(C)
    mipi_dcs_write_cmd(0x0c, 0x01, 0x06);        //FTI1F(A) 06
    mipi_dcs_write_cmd(0x0d, 0x01, 0x06);        //FTI2F(B)
    mipi_dcs_write_cmd(0x0e, 0x01, 0x00);        //FTI2F(C)
    mipi_dcs_write_cmd(0x0f, 0x01, 0x22);        //CLW1(ALR)  «e«d
    mipi_dcs_write_cmd(0x10, 0x01, 0x22);        //CLW2(ARR)  «e«d
    mipi_dcs_write_cmd(0x11, 0x01, 0x00);
    mipi_dcs_write_cmd(0x12, 0x01, 0x00);
    mipi_dcs_write_cmd(0x13, 0x01, 0x05);        //CLWX(ATF)  ¦Z«d
    mipi_dcs_write_cmd(0x14, 0x01, 0x00);
    mipi_dcs_write_cmd(0x15, 0x01, 0x00);        //GPMRi(ALR)
    mipi_dcs_write_cmd(0x16, 0x01, 0x00);        //GPMRii(ARR)
    mipi_dcs_write_cmd(0x17, 0x01, 0x00);        //GPMFi(ALF)
    mipi_dcs_write_cmd(0x18, 0x01, 0x00);        //GPMFii(AFF)
    mipi_dcs_write_cmd(0x19, 0x01, 0x00);
    mipi_dcs_write_cmd(0x1a, 0x01, 0x00);
    mipi_dcs_write_cmd(0x1b, 0x01, 0x00);
    mipi_dcs_write_cmd(0x1c, 0x01, 0x00);
    mipi_dcs_write_cmd(0x1d, 0x01, 0x00);
    mipi_dcs_write_cmd(0x1e, 0x01, 0x40);        //CLKA 40¦Û°Ê¤Ï C0¤â°Ê¤Ï(X8°Ñ¦ÒCLKB)
    mipi_dcs_write_cmd(0x1f, 0x01, 0xC0);        //C0
    mipi_dcs_write_cmd(0x20, 0x01, 0x0E);        //CLKA_Rise
    mipi_dcs_write_cmd(0x21, 0x01, 0x09);        //CLKA_Fall
    mipi_dcs_write_cmd(0x22, 0x01, 0x0F);        //CLKB_Rise(keep toggle»Ý³]CLK A«á¤@®æ)
    mipi_dcs_write_cmd(0x23, 0x01, 0x00);        //CLKB_Fall
    mipi_dcs_write_cmd(0x24, 0x01, 0x8A);        //CLK keep toggle(AL) 8X©¹¥ª¬Ý
    mipi_dcs_write_cmd(0x25, 0x01, 0x8A);        //CLK keep toggle(AR) 8X©¹¥ª¬Ý
    mipi_dcs_write_cmd(0x26, 0x01, 0x00);
    mipi_dcs_write_cmd(0x27, 0x01, 0x00);
    mipi_dcs_write_cmd(0x28, 0x01, 0x77);       //CLK Phase  KEEP Toggle 7F
    mipi_dcs_write_cmd(0x29, 0x01, 0x77);       //CLK overlap
    mipi_dcs_write_cmd(0x2a, 0x01, 0x00);
    mipi_dcs_write_cmd(0x2b, 0x01, 0x00);
    mipi_dcs_write_cmd(0x2c, 0x01, 0x02);      //GCH R
    mipi_dcs_write_cmd(0x2d, 0x01, 0x02);      //GCL R
    mipi_dcs_write_cmd(0x2e, 0x01, 0x07);      //GCH F
    mipi_dcs_write_cmd(0x2f, 0x01, 0x07);      //GCL F
    mipi_dcs_write_cmd(0x30, 0x01, 0x00);
    mipi_dcs_write_cmd(0x31, 0x01, 0x00);
    mipi_dcs_write_cmd(0x32, 0x01, 0x22);       //GCH/L ext2/1¦æ¬°  5E 01:31   5E 00:42
    mipi_dcs_write_cmd(0x33, 0x01, 0x00);
    mipi_dcs_write_cmd(0x34, 0x01, 0x00);       //VDD1&2 non-overlap 04:2.62us
    mipi_dcs_write_cmd(0x35, 0x01, 0x0A);       //GCH/L °Ï¶¡ 00:VS«e 01:VS«á 10:¸óVS 11:frame¤¤
    mipi_dcs_write_cmd(0x36, 0x01, 0x00);
    mipi_dcs_write_cmd(0x37, 0x01, 0x08);       //GCH/L
    mipi_dcs_write_cmd(0x38, 0x01, 0x3C);      //VDD1&2 toggle 1sec
    mipi_dcs_write_cmd(0x39, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3a, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3b, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3c, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3d, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3e, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3f, 0x01, 0x00);
    mipi_dcs_write_cmd(0x40, 0x01, 0x00);
    mipi_dcs_write_cmd(0x41, 0x01, 0x00);
    mipi_dcs_write_cmd(0x42, 0x01, 0x00);
    mipi_dcs_write_cmd(0x43, 0x01, 0x08);       //GCH/L
    mipi_dcs_write_cmd(0x44, 0x01, 0x00);

    //GIP_2
    mipi_dcs_write_cmd(0x50, 0x01, 0x01);
    mipi_dcs_write_cmd(0x51, 0x01, 0x23);
    mipi_dcs_write_cmd(0x52, 0x01, 0x45);
    mipi_dcs_write_cmd(0x53, 0x01, 0x67);
    mipi_dcs_write_cmd(0x54, 0x01, 0x89);
    mipi_dcs_write_cmd(0x55, 0x01, 0xAB);
    mipi_dcs_write_cmd(0x56, 0x01, 0x01);
    mipi_dcs_write_cmd(0x57, 0x01, 0x23);
    mipi_dcs_write_cmd(0x58, 0x01, 0x45);
    mipi_dcs_write_cmd(0x59, 0x01, 0x67);
    mipi_dcs_write_cmd(0x5a, 0x01, 0x89);
    mipi_dcs_write_cmd(0x5b, 0x01, 0xAB);
    mipi_dcs_write_cmd(0x5c, 0x01, 0xCD);
    mipi_dcs_write_cmd(0x5d, 0x01, 0xEF);

    //GIP_3
    mipi_dcs_write_cmd(0x5e, 0x01, 0x00);
    mipi_dcs_write_cmd(0x5f, 0x01, 0x02);     //FW_CGOUT_L[1]    VGL
    mipi_dcs_write_cmd(0x60, 0x01, 0x02);     //FW_CGOUT_L[2]    VGL
    mipi_dcs_write_cmd(0x61, 0x01, 0x06);     //FW_CGOUT_L[3]    VST_R
    mipi_dcs_write_cmd(0x62, 0x01, 0x0F);     //FW_CGOUT_L[4]    XCLK_R4
    mipi_dcs_write_cmd(0x63, 0x01, 0x0F);     //FW_CGOUT_L[5]    XCLK_R4
    mipi_dcs_write_cmd(0x64, 0x01, 0x13);     //FW_CGOUT_L[6]    CLK_R4
    mipi_dcs_write_cmd(0x65, 0x01, 0x13);     //FW_CGOUT_L[7]    CLK_R4
    mipi_dcs_write_cmd(0x66, 0x01, 0x0E);     //FW_CGOUT_L[8]    XCLK_R3
    mipi_dcs_write_cmd(0x67, 0x01, 0x0E);     //FW_CGOUT_L[9]    XCLK_R3
    mipi_dcs_write_cmd(0x68, 0x01, 0x12);     //FW_CGOUT_L[10]   CLK_R3
    mipi_dcs_write_cmd(0x69, 0x01, 0x12);     //FW_CGOUT_L[11]   CLK_R3
    mipi_dcs_write_cmd(0x6a, 0x01, 0x0D);     //FW_CGOUT_L[12]   XCLK_R2
    mipi_dcs_write_cmd(0x6b, 0x01, 0x0D);     //FW_CGOUT_L[13]   XCLK_R2
    mipi_dcs_write_cmd(0x6c, 0x01, 0x11);     //FW_CGOUT_L[14]   CLK_R2
    mipi_dcs_write_cmd(0x6d, 0x01, 0x11);     //FW_CGOUT_L[15]   CLK_R2
    mipi_dcs_write_cmd(0x6e, 0x01, 0x0C);     //FW_CGOUT_L[16]   XCLK_R1
    mipi_dcs_write_cmd(0x6f, 0x01, 0x0C);     //FW_CGOUT_L[17]   XCLK_R1
    mipi_dcs_write_cmd(0x70, 0x01, 0x10);     //FW_CGOUT_L[18]   CLK_R1
    mipi_dcs_write_cmd(0x71, 0x01, 0x10);     //FW_CGOUT_L[19]   CLK_R1
    mipi_dcs_write_cmd(0x72, 0x01, 0x00);     //FW_CGOUT_L[20]   D2U  00
    mipi_dcs_write_cmd(0x73, 0x01, 0x16);     //FW_CGOUT_L[21]   U2D  01
    mipi_dcs_write_cmd(0x74, 0x01, 0x08);     //FW_CGOUT_L[22]   VEND

    mipi_dcs_write_cmd(0x75, 0x01, 0x02);     //BW_CGOUT_L[1]
    mipi_dcs_write_cmd(0x76, 0x01, 0x02);     //BW_CGOUT_L[2]
    mipi_dcs_write_cmd(0x77, 0x01, 0x08);     //BW_CGOUT_L[3]
    mipi_dcs_write_cmd(0x78, 0x01, 0x0f);     //BW_CGOUT_L[4]
    mipi_dcs_write_cmd(0x79, 0x01, 0x0f);     //BW_CGOUT_L[5]
    mipi_dcs_write_cmd(0x7a, 0x01, 0x13);     //BW_CGOUT_L[6]
    mipi_dcs_write_cmd(0x7b, 0x01, 0x13);     //BW_CGOUT_L[7]
    mipi_dcs_write_cmd(0x7c, 0x01, 0x0e);     //BW_CGOUT_L[8]
    mipi_dcs_write_cmd(0x7d, 0x01, 0x0e);     //BW_CGOUT_L[9]
    mipi_dcs_write_cmd(0x7e, 0x01, 0x12);     //BW_CGOUT_L[10]
    mipi_dcs_write_cmd(0x7f, 0x01, 0x12);     //BW_CGOUT_L[11]
    mipi_dcs_write_cmd(0x80, 0x01, 0x0d);     //BW_CGOUT_L[12]
    mipi_dcs_write_cmd(0x81, 0x01, 0x0d);     //BW_CGOUT_L[13]
    mipi_dcs_write_cmd(0x82, 0x01, 0x11);     //BW_CGOUT_L[14]
    mipi_dcs_write_cmd(0x83, 0x01, 0x11);     //BW_CGOUT_L[15]
    mipi_dcs_write_cmd(0x84, 0x01, 0x0c);     //BW_CGOUT_L[16]
    mipi_dcs_write_cmd(0x85, 0x01, 0x0c);     //BW_CGOUT_L[17]
    mipi_dcs_write_cmd(0x86, 0x01, 0x10);     //BW_CGOUT_L[18]
    mipi_dcs_write_cmd(0x87, 0x01, 0x10);     //BW_CGOUT_L[19]
    mipi_dcs_write_cmd(0x88, 0x01, 0x17);     //BW_CGOUT_L[20]
    mipi_dcs_write_cmd(0x89, 0x01, 0x01);     //BW_CGOUT_L[21]
    mipi_dcs_write_cmd(0x8a, 0x01, 0x06);     //BW_CGOUT_L[22]

    //CMD_Page 4
    mipi_dcs_write_cmd(0xFF, 0x03, 0x98, 0x81, 0x04);

    mipi_dcs_write_cmd(0x6E, 0x01, 0x2A);   //2B        /VGH 15V
    mipi_dcs_write_cmd(0x6F, 0x01, 0x37);       // reg vcl + pumping ratio VGH=3x VGL=-3x
    mipi_dcs_write_cmd(0x3A, 0x01, 0xA4);       //POWER SAVING
    mipi_dcs_write_cmd(0x8D, 0x01, 0x25);       //VGL -13V
    mipi_dcs_write_cmd(0x87, 0x01, 0xBA);       //ESD
    mipi_dcs_write_cmd(0xB2, 0x01, 0xD1);
    mipi_dcs_write_cmd(0x88, 0x01, 0x0B);
    mipi_dcs_write_cmd(0x38, 0x01, 0x01);
    mipi_dcs_write_cmd(0x39, 0x01, 0x00);
    mipi_dcs_write_cmd(0xB5, 0x01, 0x07);       //gamma bias
    mipi_dcs_write_cmd(0x31, 0x01, 0x75);       //source bias
    mipi_dcs_write_cmd(0x3B, 0x01, 0x98);

    //CMD_Page 1
    mipi_dcs_write_cmd(0xFF, 0x03, 0x98, 0x81, 0x01);
    mipi_dcs_write_cmd(0x22, 0x01, 0x0A);    //BGR, 0x SS
    mipi_dcs_write_cmd(0x31, 0x01, 0x0A);    //Z inversion
    mipi_dcs_write_cmd(0x35, 0x01, 0x07);   //chopper
    mipi_dcs_write_cmd(0x52, 0x01, 0x00);    //VCOM1[8]
    mipi_dcs_write_cmd(0x53, 0x01, 0x73);    //VCOM1[7:0]
    mipi_dcs_write_cmd(0x54, 0x01, 0x00);    //VCOM2[8]
    mipi_dcs_write_cmd(0x55, 0x01, 0x3D);    //VCOM2[7:0]
    mipi_dcs_write_cmd(0x50, 0x01, 0xD0);    //VREG1OUT 5.208V
    mipi_dcs_write_cmd(0x51, 0x01, 0xCB);    //VREG2OUT -5.208V
    mipi_dcs_write_cmd(0x60, 0x01, 0x14);    //SDT=2.0
    mipi_dcs_write_cmd(0x62, 0x01, 0x01);    //EQ
    mipi_dcs_write_cmd(0x63, 0x01, 0x01);    //PC

    //============Gamma START=============

    //Pos Register
    mipi_dcs_write_cmd(0xA0, 0x01, 0x00);
    mipi_dcs_write_cmd(0xA1, 0x01, 0x18);
    mipi_dcs_write_cmd(0xA2, 0x01, 0x28);
    mipi_dcs_write_cmd(0xA3, 0x01, 0x17);
    mipi_dcs_write_cmd(0xA4, 0x01, 0x1C);
    mipi_dcs_write_cmd(0xA5, 0x01, 0x30);
    mipi_dcs_write_cmd(0xA6, 0x01, 0x24);
    mipi_dcs_write_cmd(0xA7, 0x01, 0x24);
    mipi_dcs_write_cmd(0xA8, 0x01, 0x85);
    mipi_dcs_write_cmd(0xA9, 0x01, 0x1C);
    mipi_dcs_write_cmd(0xAA, 0x01, 0x26);
    mipi_dcs_write_cmd(0xAB, 0x01, 0x66);
    mipi_dcs_write_cmd(0xAC, 0x01, 0x19);
    mipi_dcs_write_cmd(0xAD, 0x01, 0x18);
    mipi_dcs_write_cmd(0xAE, 0x01, 0x4F);
    mipi_dcs_write_cmd(0xAF, 0x01, 0x24);
    mipi_dcs_write_cmd(0xB0, 0x01, 0x2B);
    mipi_dcs_write_cmd(0xB1, 0x01, 0x4A);
    mipi_dcs_write_cmd(0xB2, 0x01, 0x59);
    mipi_dcs_write_cmd(0xB3, 0x01, 0x23);

    mipi_dcs_write_cmd(0xB7, 0x01, 0x03);// SET MIPI DSI 2 Lane mode

    //Neg Register
    mipi_dcs_write_cmd(0xC0, 0x01, 0x00);
    mipi_dcs_write_cmd(0xC1, 0x01, 0x18);
    mipi_dcs_write_cmd(0xC2, 0x01, 0x28);
    mipi_dcs_write_cmd(0xC3, 0x01, 0x17);
    mipi_dcs_write_cmd(0xC4, 0x01, 0x1B);
    mipi_dcs_write_cmd(0xC5, 0x01, 0x2F);
    mipi_dcs_write_cmd(0xC6, 0x01, 0x22);
    mipi_dcs_write_cmd(0xC7, 0x01, 0x22);
    mipi_dcs_write_cmd(0xC8, 0x01, 0x87);
    mipi_dcs_write_cmd(0xC9, 0x01, 0x1C);
    mipi_dcs_write_cmd(0xCA, 0x01, 0x27);
    mipi_dcs_write_cmd(0xCB, 0x01, 0x66);
    mipi_dcs_write_cmd(0xCC, 0x01, 0x19);
    mipi_dcs_write_cmd(0xCD, 0x01, 0x1A);
    mipi_dcs_write_cmd(0xCE, 0x01, 0x4E);
    mipi_dcs_write_cmd(0xCF, 0x01, 0x24);
    mipi_dcs_write_cmd(0xD0, 0x01, 0x2A);
    mipi_dcs_write_cmd(0xD1, 0x01, 0x4C);
    mipi_dcs_write_cmd(0xD2, 0x01, 0x5A);
    mipi_dcs_write_cmd(0xD3, 0x01, 0x23);

    //============ Gamma END===========

    //CMD_Page 0
    mipi_dcs_write_cmd(0xFF, 0x03, 0x98, 0x81, 0x00);
    mipi_dcs_write_cmd(0x11, 0x01, 0x00);
    vTaskDelay(pdMS_TO_TICKS(120));
    mipi_dcs_write_cmd(0x29, 0x01, 0x00);
    mipi_dcs_write_cmd(0x35, 0x01, 0x00);
    switch (TEST_DSI_COLOR_WIDTH) {
    case 16: mipi_dcs_write_cmd(0x3A, 0x01, 0x05); break;
    case 18: mipi_dcs_write_cmd(0x3A, 0x01, 0x06); break;
    case 24: mipi_dcs_write_cmd(0x3A, 0x01, 0x07); break;
    }
    // mipi_dcs_write_cmd(0x0B, 0x01, 0x08);

#endif

#if TEST_DSI_NT35532

    mipi_dcs_write_cmd(0xFF, 1, 0x01);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0x02);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0x03);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0x04);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0x05);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xD7, 1, 0x31);
    mipi_dcs_write_cmd(0xD8, 1, 0x7E);
    vTaskDelay(pdMS_TO_TICKS(100));
    mipi_dcs_write_cmd(0xFF, 1, 0x00);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xBA, 1, 0x01);
    mipi_dcs_write_cmd(0x36, 1, 0x00);
    mipi_dcs_write_cmd(0xB0, 1, 0x00);
    mipi_dcs_write_cmd(0xD3, 1, 0x08);
    mipi_dcs_write_cmd(0xD4, 1, 0x0E);
    mipi_dcs_write_cmd(0xD5, 1, 0x0F);
    mipi_dcs_write_cmd(0xD6, 1, 0x48);
    mipi_dcs_write_cmd(0xD7, 1, 0x00);
    mipi_dcs_write_cmd(0xD9, 1, 0x00);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0xEE);
    mipi_dcs_write_cmd(0x40, 1, 0x00);
    mipi_dcs_write_cmd(0x41, 1, 0x00);
    mipi_dcs_write_cmd(0x42, 1, 0x00);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0x01);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0x01, 1, 0x55);
    mipi_dcs_write_cmd(0x04, 1, 0x0C);
    mipi_dcs_write_cmd(0x05, 1, 0x3A);
    mipi_dcs_write_cmd(0x06, 1, 0x50);
    mipi_dcs_write_cmd(0x07, 1, 0xD0);
    mipi_dcs_write_cmd(0x0A, 1, 0x0F);
    mipi_dcs_write_cmd(0x0C, 1, 0x06);
    mipi_dcs_write_cmd(0x0D, 1, 0x7F);
    mipi_dcs_write_cmd(0x0E, 1, 0x7F);
    mipi_dcs_write_cmd(0x0F, 1, 0x70);
    mipi_dcs_write_cmd(0x10, 1, 0x63);
    mipi_dcs_write_cmd(0x11, 1, 0x3C);
    mipi_dcs_write_cmd(0x12, 1, 0x5C);
    mipi_dcs_write_cmd(0x13, 1, 0x5A);
    mipi_dcs_write_cmd(0x14, 1, 0x5A);
    mipi_dcs_write_cmd(0x15, 1, 0x60);
    mipi_dcs_write_cmd(0x16, 1, 0x15);
    mipi_dcs_write_cmd(0x17, 1, 0x15);
    mipi_dcs_write_cmd(0x23, 1, 0x00);
    mipi_dcs_write_cmd(0x24, 1, 0x00);
    mipi_dcs_write_cmd(0x25, 1, 0x00);
    mipi_dcs_write_cmd(0x26, 1, 0x00);
    mipi_dcs_write_cmd(0x27, 1, 0x00);
    mipi_dcs_write_cmd(0x28, 1, 0x00);
    mipi_dcs_write_cmd(0x44, 1, 0x00);
    mipi_dcs_write_cmd(0x45, 1, 0x00);
    mipi_dcs_write_cmd(0x46, 1, 0x00);
    mipi_dcs_write_cmd(0x5B, 1, 0xCA);
    mipi_dcs_write_cmd(0x5C, 1, 0x00);
    mipi_dcs_write_cmd(0x5D, 1, 0x00);
    mipi_dcs_write_cmd(0x5E, 1, 0x2D);
    mipi_dcs_write_cmd(0x5F, 1, 0x1B);
    mipi_dcs_write_cmd(0x60, 1, 0xD5);
    mipi_dcs_write_cmd(0x61, 1, 0xF7);
    mipi_dcs_write_cmd(0x6C, 1, 0xAB);
    mipi_dcs_write_cmd(0x6D, 1, 0x44);
    mipi_dcs_write_cmd(0xFF, 1, 0x05);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0x00, 1, 0x3F);
    mipi_dcs_write_cmd(0x01, 1, 0x3F);
    mipi_dcs_write_cmd(0x02, 1, 0x3F);
    mipi_dcs_write_cmd(0x03, 1, 0x3F);
    mipi_dcs_write_cmd(0x04, 1, 0x38);
    mipi_dcs_write_cmd(0x05, 1, 0x3F);
    mipi_dcs_write_cmd(0x06, 1, 0x3F);
    mipi_dcs_write_cmd(0x07, 1, 0x19);
    mipi_dcs_write_cmd(0x08, 1, 0x1D);
    mipi_dcs_write_cmd(0x09, 1, 0x3F);
    mipi_dcs_write_cmd(0x0A, 1, 0x3F);
    mipi_dcs_write_cmd(0x0B, 1, 0x1B);
    mipi_dcs_write_cmd(0x0C, 1, 0x17);
    mipi_dcs_write_cmd(0x0D, 1, 0x3F);
    mipi_dcs_write_cmd(0x0E, 1, 0x3F);
    mipi_dcs_write_cmd(0x0F, 1, 0x08);
    mipi_dcs_write_cmd(0x10, 1, 0x3F);
    mipi_dcs_write_cmd(0x11, 1, 0x10);
    mipi_dcs_write_cmd(0x12, 1, 0x3F);
    mipi_dcs_write_cmd(0x13, 1, 0x3F);
    mipi_dcs_write_cmd(0x14, 1, 0x3F);
    mipi_dcs_write_cmd(0x15, 1, 0x3F);
    mipi_dcs_write_cmd(0x16, 1, 0x3F);
    mipi_dcs_write_cmd(0x17, 1, 0x3F);
    mipi_dcs_write_cmd(0x18, 1, 0x38);
    mipi_dcs_write_cmd(0x19, 1, 0x18);
    mipi_dcs_write_cmd(0x1A, 1, 0x1C);
    mipi_dcs_write_cmd(0x1B, 1, 0x3F);
    mipi_dcs_write_cmd(0x1C, 1, 0x3F);
    mipi_dcs_write_cmd(0x1D, 1, 0x1A);
    mipi_dcs_write_cmd(0x1E, 1, 0x16);
    mipi_dcs_write_cmd(0x1F, 1, 0x3F);
    mipi_dcs_write_cmd(0x20, 1, 0x3F);
    mipi_dcs_write_cmd(0x21, 1, 0x3F);
    mipi_dcs_write_cmd(0x22, 1, 0x3F);
    mipi_dcs_write_cmd(0x23, 1, 0x06);
    mipi_dcs_write_cmd(0x24, 1, 0x3F);
    mipi_dcs_write_cmd(0x25, 1, 0x0E);
    mipi_dcs_write_cmd(0x26, 1, 0x3F);
    mipi_dcs_write_cmd(0x27, 1, 0x3F);
    mipi_dcs_write_cmd(0x54, 1, 0x06);
    mipi_dcs_write_cmd(0x55, 1, 0x05);
    mipi_dcs_write_cmd(0x56, 1, 0x04);
    mipi_dcs_write_cmd(0x58, 1, 0x03);
    mipi_dcs_write_cmd(0x59, 1, 0x1B);
    mipi_dcs_write_cmd(0x5A, 1, 0x1B);
    mipi_dcs_write_cmd(0x5B, 1, 0x01);
    mipi_dcs_write_cmd(0x5C, 1, 0x32);
    mipi_dcs_write_cmd(0x5E, 1, 0x18);
    mipi_dcs_write_cmd(0x5F, 1, 0x20);
    mipi_dcs_write_cmd(0x60, 1, 0x2B);
    mipi_dcs_write_cmd(0x61, 1, 0x2C);
    mipi_dcs_write_cmd(0x62, 1, 0x18);
    mipi_dcs_write_cmd(0x63, 1, 0x01);
    mipi_dcs_write_cmd(0x64, 1, 0x32);
    mipi_dcs_write_cmd(0x65, 1, 0x00);
    mipi_dcs_write_cmd(0x66, 1, 0x44);
    mipi_dcs_write_cmd(0x67, 1, 0x11);
    mipi_dcs_write_cmd(0x68, 1, 0x01);
    mipi_dcs_write_cmd(0x69, 1, 0x01);
    mipi_dcs_write_cmd(0x6A, 1, 0x04);
    mipi_dcs_write_cmd(0x6B, 1, 0x2C);
    mipi_dcs_write_cmd(0x6C, 1, 0x08);
    mipi_dcs_write_cmd(0x6D, 1, 0x08);
    mipi_dcs_write_cmd(0x78, 1, 0x00);
    mipi_dcs_write_cmd(0x79, 1, 0x00);
    mipi_dcs_write_cmd(0x7E, 1, 0x00);
    mipi_dcs_write_cmd(0x7F, 1, 0x00);
    mipi_dcs_write_cmd(0x80, 1, 0x00);
    mipi_dcs_write_cmd(0x81, 1, 0x00);
    mipi_dcs_write_cmd(0x8D, 1, 0x00);
    mipi_dcs_write_cmd(0x8E, 1, 0x00);
    mipi_dcs_write_cmd(0x8F, 1, 0xC0);
    mipi_dcs_write_cmd(0x90, 1, 0x73);
    mipi_dcs_write_cmd(0x91, 1, 0x10);
    mipi_dcs_write_cmd(0x92, 1, 0x07);
    mipi_dcs_write_cmd(0x96, 1, 0x11);
    mipi_dcs_write_cmd(0x97, 1, 0x14);
    mipi_dcs_write_cmd(0x98, 1, 0x00);
    mipi_dcs_write_cmd(0x99, 1, 0x00);
    mipi_dcs_write_cmd(0x9A, 1, 0x00);
    mipi_dcs_write_cmd(0x9B, 1, 0x61);
    mipi_dcs_write_cmd(0x9C, 1, 0x15);
    mipi_dcs_write_cmd(0x9D, 1, 0x30);
    mipi_dcs_write_cmd(0x9F, 1, 0x0F);
    mipi_dcs_write_cmd(0xA2, 1, 0xB0);
    mipi_dcs_write_cmd(0xA7, 1, 0x0A);
    mipi_dcs_write_cmd(0xA9, 1, 0x00);
    mipi_dcs_write_cmd(0xAA, 1, 0x70);
    mipi_dcs_write_cmd(0xAB, 1, 0xDA);
    mipi_dcs_write_cmd(0xAC, 1, 0xFF);
    mipi_dcs_write_cmd(0xAE, 1, 0xF4);
    mipi_dcs_write_cmd(0xAF, 1, 0x40);
    mipi_dcs_write_cmd(0xB0, 1, 0x7F);
    mipi_dcs_write_cmd(0xB1, 1, 0x16);
    mipi_dcs_write_cmd(0xB2, 1, 0x53);
    mipi_dcs_write_cmd(0xB3, 1, 0x00);
    mipi_dcs_write_cmd(0xB4, 1, 0x2A);
    mipi_dcs_write_cmd(0xB5, 1, 0x3A);
    mipi_dcs_write_cmd(0xB6, 1, 0xF0);
    mipi_dcs_write_cmd(0xBC, 1, 0x85);
    mipi_dcs_write_cmd(0xBD, 1, 0xF4);
    mipi_dcs_write_cmd(0xBE, 1, 0x33);
    mipi_dcs_write_cmd(0xBF, 1, 0x13);
    mipi_dcs_write_cmd(0xC0, 1, 0x77);
    mipi_dcs_write_cmd(0xC1, 1, 0x77);
    mipi_dcs_write_cmd(0xC2, 1, 0x77);
    mipi_dcs_write_cmd(0xC3, 1, 0x77);
    mipi_dcs_write_cmd(0xC4, 1, 0x77);
    mipi_dcs_write_cmd(0xC5, 1, 0x77);
    mipi_dcs_write_cmd(0xC6, 1, 0x77);
    mipi_dcs_write_cmd(0xC7, 1, 0x77);
    mipi_dcs_write_cmd(0xC8, 1, 0xAA);
    mipi_dcs_write_cmd(0xC9, 1, 0x2A);
    mipi_dcs_write_cmd(0xCA, 1, 0x00);
    mipi_dcs_write_cmd(0xCB, 1, 0xAA);
    mipi_dcs_write_cmd(0xCC, 1, 0x92);
    mipi_dcs_write_cmd(0xCD, 1, 0x00);
    mipi_dcs_write_cmd(0xCE, 1, 0x18);
    mipi_dcs_write_cmd(0xCF, 1, 0x88);
    mipi_dcs_write_cmd(0xD0, 1, 0xAA);
    mipi_dcs_write_cmd(0xD1, 1, 0x00);
    mipi_dcs_write_cmd(0xD2, 1, 0x00);
    mipi_dcs_write_cmd(0xD3, 1, 0x00);
    mipi_dcs_write_cmd(0xD6, 1, 0x02);
    mipi_dcs_write_cmd(0xED, 1, 0x00);
    mipi_dcs_write_cmd(0xEE, 1, 0x00);
    mipi_dcs_write_cmd(0xEF, 1, 0x70);
    mipi_dcs_write_cmd(0xFA, 1, 0x03);
    mipi_dcs_write_cmd(0xFF, 1, 0x00);

    //page selection cmd start
    mipi_dcs_write_cmd(0xFF, 1, 0x01);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    //page selection cmd end
    //R(+) MCR cmd
    mipi_dcs_write_cmd(0x75, 1, 0x00);
    mipi_dcs_write_cmd(0x76, 1, 0x00);
    mipi_dcs_write_cmd(0x77, 1, 0x00);
    mipi_dcs_write_cmd(0x78, 1, 0x2C);
    mipi_dcs_write_cmd(0x79, 1, 0x00);
    mipi_dcs_write_cmd(0x7A, 1, 0x4F);
    mipi_dcs_write_cmd(0x7B, 1, 0x00);
    mipi_dcs_write_cmd(0x7C, 1, 0x69);
    mipi_dcs_write_cmd(0x7D, 1, 0x00);
    mipi_dcs_write_cmd(0x7E, 1, 0x7F);
    mipi_dcs_write_cmd(0x7F, 1, 0x00);
    mipi_dcs_write_cmd(0x80, 1, 0x92);
    mipi_dcs_write_cmd(0x81, 1, 0x00);
    mipi_dcs_write_cmd(0x82, 1, 0xA3);
    mipi_dcs_write_cmd(0x83, 1, 0x00);
    mipi_dcs_write_cmd(0x84, 1, 0xB3);
    mipi_dcs_write_cmd(0x85, 1, 0x00);
    mipi_dcs_write_cmd(0x86, 1, 0xC1);
    mipi_dcs_write_cmd(0x87, 1, 0x00);
    mipi_dcs_write_cmd(0x88, 1, 0xF3);
    mipi_dcs_write_cmd(0x89, 1, 0x01);
    mipi_dcs_write_cmd(0x8A, 1, 0x1B);
    mipi_dcs_write_cmd(0x8B, 1, 0x01);
    mipi_dcs_write_cmd(0x8C, 1, 0x5A);
    mipi_dcs_write_cmd(0x8D, 1, 0x01);
    mipi_dcs_write_cmd(0x8E, 1, 0x8B);
    mipi_dcs_write_cmd(0x8F, 1, 0x01);
    mipi_dcs_write_cmd(0x90, 1, 0xD9);
    mipi_dcs_write_cmd(0x91, 1, 0x02);
    mipi_dcs_write_cmd(0x92, 1, 0x16);
    mipi_dcs_write_cmd(0x93, 1, 0x02);
    mipi_dcs_write_cmd(0x94, 1, 0x18);
    mipi_dcs_write_cmd(0x95, 1, 0x02);
    mipi_dcs_write_cmd(0x96, 1, 0x4E);
    mipi_dcs_write_cmd(0x97, 1, 0x02);
    mipi_dcs_write_cmd(0x98, 1, 0x88);
    mipi_dcs_write_cmd(0x99, 1, 0x02);
    mipi_dcs_write_cmd(0x9A, 1, 0xAC);
    mipi_dcs_write_cmd(0x9B, 1, 0x02);
    mipi_dcs_write_cmd(0x9C, 1, 0xDD);
    mipi_dcs_write_cmd(0x9D, 1, 0x03);
    mipi_dcs_write_cmd(0x9E, 1, 0x01);
    mipi_dcs_write_cmd(0x9F, 1, 0x03);
    mipi_dcs_write_cmd(0xA0, 1, 0x2E);
    mipi_dcs_write_cmd(0xA2, 1, 0x03);
    mipi_dcs_write_cmd(0xA3, 1, 0x3C);
    mipi_dcs_write_cmd(0xA4, 1, 0x03);
    mipi_dcs_write_cmd(0xA5, 1, 0x4C);
    mipi_dcs_write_cmd(0xA6, 1, 0x03);
    mipi_dcs_write_cmd(0xA7, 1, 0x5D);
    mipi_dcs_write_cmd(0xA9, 1, 0x03);
    mipi_dcs_write_cmd(0xAA, 1, 0x70);
    mipi_dcs_write_cmd(0xAB, 1, 0x03);
    mipi_dcs_write_cmd(0xAC, 1, 0x88);
    mipi_dcs_write_cmd(0xAD, 1, 0x03);
    mipi_dcs_write_cmd(0xAE, 1, 0xA8);
    mipi_dcs_write_cmd(0xAF, 1, 0x03);
    mipi_dcs_write_cmd(0xB0, 1, 0xC8);
    mipi_dcs_write_cmd(0xB1, 1, 0x03);
    mipi_dcs_write_cmd(0xB2, 1, 0xFF);
    //R(-) MCR cmd
    mipi_dcs_write_cmd(0xB3, 1, 0x00);
    mipi_dcs_write_cmd(0xB4, 1, 0x00);
    mipi_dcs_write_cmd(0xB5, 1, 0x00);
    mipi_dcs_write_cmd(0xB6, 1, 0x2C);
    mipi_dcs_write_cmd(0xB7, 1, 0x00);
    mipi_dcs_write_cmd(0xB8, 1, 0x4F);
    mipi_dcs_write_cmd(0xB9, 1, 0x00);
    mipi_dcs_write_cmd(0xBA, 1, 0x69);
    mipi_dcs_write_cmd(0xBB, 1, 0x00);
    mipi_dcs_write_cmd(0xBC, 1, 0x7F);
    mipi_dcs_write_cmd(0xBD, 1, 0x00);
    mipi_dcs_write_cmd(0xBE, 1, 0x92);
    mipi_dcs_write_cmd(0xBF, 1, 0x00);
    mipi_dcs_write_cmd(0xC0, 1, 0xA3);
    mipi_dcs_write_cmd(0xC1, 1, 0x00);
    mipi_dcs_write_cmd(0xC2, 1, 0xB3);
    mipi_dcs_write_cmd(0xC3, 1, 0x00);
    mipi_dcs_write_cmd(0xC4, 1, 0xC1);
    mipi_dcs_write_cmd(0xC5, 1, 0x00);
    mipi_dcs_write_cmd(0xC6, 1, 0xF3);
    mipi_dcs_write_cmd(0xC7, 1, 0x01);
    mipi_dcs_write_cmd(0xC8, 1, 0x1B);
    mipi_dcs_write_cmd(0xC9, 1, 0x01);
    mipi_dcs_write_cmd(0xCA, 1, 0x5A);
    mipi_dcs_write_cmd(0xCB, 1, 0x01);
    mipi_dcs_write_cmd(0xCC, 1, 0x8B);
    mipi_dcs_write_cmd(0xCD, 1, 0x01);
    mipi_dcs_write_cmd(0xCE, 1, 0xD9);
    mipi_dcs_write_cmd(0xCF, 1, 0x02);
    mipi_dcs_write_cmd(0xD0, 1, 0x16);
    mipi_dcs_write_cmd(0xD1, 1, 0x02);
    mipi_dcs_write_cmd(0xD2, 1, 0x18);
    mipi_dcs_write_cmd(0xD3, 1, 0x02);
    mipi_dcs_write_cmd(0xD4, 1, 0x4E);
    mipi_dcs_write_cmd(0xD5, 1, 0x02);
    mipi_dcs_write_cmd(0xD6, 1, 0x88);
    mipi_dcs_write_cmd(0xD7, 1, 0x02);
    mipi_dcs_write_cmd(0xD8, 1, 0xAC);
    mipi_dcs_write_cmd(0xD9, 1, 0x02);
    mipi_dcs_write_cmd(0xDA, 1, 0xDD);
    mipi_dcs_write_cmd(0xDB, 1, 0x03);
    mipi_dcs_write_cmd(0xDC, 1, 0x01);
    mipi_dcs_write_cmd(0xDD, 1, 0x03);
    mipi_dcs_write_cmd(0xDE, 1, 0x2E);
    mipi_dcs_write_cmd(0xDF, 1, 0x03);
    mipi_dcs_write_cmd(0xE0, 1, 0x3C);
    mipi_dcs_write_cmd(0xE1, 1, 0x03);
    mipi_dcs_write_cmd(0xE2, 1, 0x4C);
    mipi_dcs_write_cmd(0xE3, 1, 0x03);
    mipi_dcs_write_cmd(0xE4, 1, 0x5D);
    mipi_dcs_write_cmd(0xE5, 1, 0x03);
    mipi_dcs_write_cmd(0xE6, 1, 0x70);
    mipi_dcs_write_cmd(0xE7, 1, 0x03);
    mipi_dcs_write_cmd(0xE8, 1, 0x88);
    mipi_dcs_write_cmd(0xE9, 1, 0x03);
    mipi_dcs_write_cmd(0xEA, 1, 0xA8);
    mipi_dcs_write_cmd(0xEB, 1, 0x03);
    mipi_dcs_write_cmd(0xEC, 1, 0xC8);
    mipi_dcs_write_cmd(0xED, 1, 0x03);
    mipi_dcs_write_cmd(0xEE, 1, 0xFF);
    //G(+) MCR cmd
    mipi_dcs_write_cmd(0xEF, 1, 0x00);
    mipi_dcs_write_cmd(0xF0, 1, 0x00);
    mipi_dcs_write_cmd(0xF1, 1, 0x00);
    mipi_dcs_write_cmd(0xF2, 1, 0x2C);
    mipi_dcs_write_cmd(0xF3, 1, 0x00);
    mipi_dcs_write_cmd(0xF4, 1, 0x4F);
    mipi_dcs_write_cmd(0xF5, 1, 0x00);
    mipi_dcs_write_cmd(0xF6, 1, 0x69);
    mipi_dcs_write_cmd(0xF7, 1, 0x00);
    mipi_dcs_write_cmd(0xF8, 1, 0x7F);
    mipi_dcs_write_cmd(0xF9, 1, 0x00);
    mipi_dcs_write_cmd(0xFA, 1, 0x92);
    //page selection cmd start
    mipi_dcs_write_cmd(0xFF, 1, 0x02);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    //page selection cmd end
    mipi_dcs_write_cmd(0x00, 1, 0x00);
    mipi_dcs_write_cmd(0x01, 1, 0xA3);
    mipi_dcs_write_cmd(0x02, 1, 0x00);
    mipi_dcs_write_cmd(0x03, 1, 0xB3);
    mipi_dcs_write_cmd(0x04, 1, 0x00);
    mipi_dcs_write_cmd(0x05, 1, 0xC1);
    mipi_dcs_write_cmd(0x06, 1, 0x00);
    mipi_dcs_write_cmd(0x07, 1, 0xF3);
    mipi_dcs_write_cmd(0x08, 1, 0x01);
    mipi_dcs_write_cmd(0x09, 1, 0x1B);
    mipi_dcs_write_cmd(0x0A, 1, 0x01);
    mipi_dcs_write_cmd(0x0B, 1, 0x5A);
    mipi_dcs_write_cmd(0x0C, 1, 0x01);
    mipi_dcs_write_cmd(0x0D, 1, 0x8B);
    mipi_dcs_write_cmd(0x0E, 1, 0x01);
    mipi_dcs_write_cmd(0x0F, 1, 0xD9);
    mipi_dcs_write_cmd(0x10, 1, 0x02);
    mipi_dcs_write_cmd(0x11, 1, 0x16);
    mipi_dcs_write_cmd(0x12, 1, 0x02);
    mipi_dcs_write_cmd(0x13, 1, 0x18);
    mipi_dcs_write_cmd(0x14, 1, 0x02);
    mipi_dcs_write_cmd(0x15, 1, 0x4E);
    mipi_dcs_write_cmd(0x16, 1, 0x02);
    mipi_dcs_write_cmd(0x17, 1, 0x88);
    mipi_dcs_write_cmd(0x18, 1, 0x02);
    mipi_dcs_write_cmd(0x19, 1, 0xAC);
    mipi_dcs_write_cmd(0x1A, 1, 0x02);
    mipi_dcs_write_cmd(0x1B, 1, 0xDD);
    mipi_dcs_write_cmd(0x1C, 1, 0x03);
    mipi_dcs_write_cmd(0x1D, 1, 0x01);
    mipi_dcs_write_cmd(0x1E, 1, 0x03);
    mipi_dcs_write_cmd(0x1F, 1, 0x2E);
    mipi_dcs_write_cmd(0x20, 1, 0x03);
    mipi_dcs_write_cmd(0x21, 1, 0x3C);
    mipi_dcs_write_cmd(0x22, 1, 0x03);
    mipi_dcs_write_cmd(0x23, 1, 0x4C);
    mipi_dcs_write_cmd(0x24, 1, 0x03);
    mipi_dcs_write_cmd(0x25, 1, 0x5D);
    mipi_dcs_write_cmd(0x26, 1, 0x03);
    mipi_dcs_write_cmd(0x27, 1, 0x70);
    mipi_dcs_write_cmd(0x28, 1, 0x03);
    mipi_dcs_write_cmd(0x29, 1, 0x88);
    mipi_dcs_write_cmd(0x2A, 1, 0x03);
    mipi_dcs_write_cmd(0x2B, 1, 0xA8);
    mipi_dcs_write_cmd(0x2D, 1, 0x03);
    mipi_dcs_write_cmd(0x2F, 1, 0xC8);
    mipi_dcs_write_cmd(0x30, 1, 0x03);
    mipi_dcs_write_cmd(0x31, 1, 0xFF);
    //G(-) MCR cmd
    mipi_dcs_write_cmd(0x32, 1, 0x00);
    mipi_dcs_write_cmd(0x33, 1, 0x00);
    mipi_dcs_write_cmd(0x34, 1, 0x00);
    mipi_dcs_write_cmd(0x35, 1, 0x2C);
    mipi_dcs_write_cmd(0x36, 1, 0x00);
    mipi_dcs_write_cmd(0x37, 1, 0x4F);
    mipi_dcs_write_cmd(0x38, 1, 0x00);
    mipi_dcs_write_cmd(0x39, 1, 0x69);
    mipi_dcs_write_cmd(0x3A, 1, 0x00);
    mipi_dcs_write_cmd(0x3B, 1, 0x7F);
    mipi_dcs_write_cmd(0x3D, 1, 0x00);
    mipi_dcs_write_cmd(0x3F, 1, 0x92);
    mipi_dcs_write_cmd(0x40, 1, 0x00);
    mipi_dcs_write_cmd(0x41, 1, 0xA3);
    mipi_dcs_write_cmd(0x42, 1, 0x00);
    mipi_dcs_write_cmd(0x43, 1, 0xB3);
    mipi_dcs_write_cmd(0x44, 1, 0x00);
    mipi_dcs_write_cmd(0x45, 1, 0xC1);
    mipi_dcs_write_cmd(0x46, 1, 0x00);
    mipi_dcs_write_cmd(0x47, 1, 0xF3);
    mipi_dcs_write_cmd(0x48, 1, 0x01);
    mipi_dcs_write_cmd(0x49, 1, 0x1B);
    mipi_dcs_write_cmd(0x4A, 1, 0x01);
    mipi_dcs_write_cmd(0x4B, 1, 0x5A);
    mipi_dcs_write_cmd(0x4C, 1, 0x01);
    mipi_dcs_write_cmd(0x4D, 1, 0x8B);
    mipi_dcs_write_cmd(0x4E, 1, 0x01);
    mipi_dcs_write_cmd(0x4F, 1, 0xD9);
    mipi_dcs_write_cmd(0x50, 1, 0x02);
    mipi_dcs_write_cmd(0x51, 1, 0x16);
    mipi_dcs_write_cmd(0x52, 1, 0x02);
    mipi_dcs_write_cmd(0x53, 1, 0x18);
    mipi_dcs_write_cmd(0x54, 1, 0x02);
    mipi_dcs_write_cmd(0x55, 1, 0x4E);
    mipi_dcs_write_cmd(0x56, 1, 0x02);
    mipi_dcs_write_cmd(0x58, 1, 0x88);
    mipi_dcs_write_cmd(0x59, 1, 0x02);
    mipi_dcs_write_cmd(0x5A, 1, 0xAC);
    mipi_dcs_write_cmd(0x5B, 1, 0x02);
    mipi_dcs_write_cmd(0x5C, 1, 0xDD);
    mipi_dcs_write_cmd(0x5D, 1, 0x03);
    mipi_dcs_write_cmd(0x5E, 1, 0x01);
    mipi_dcs_write_cmd(0x5F, 1, 0x03);
    mipi_dcs_write_cmd(0x60, 1, 0x2E);
    mipi_dcs_write_cmd(0x61, 1, 0x03);
    mipi_dcs_write_cmd(0x62, 1, 0x3C);
    mipi_dcs_write_cmd(0x63, 1, 0x03);
    mipi_dcs_write_cmd(0x64, 1, 0x4C);
    mipi_dcs_write_cmd(0x65, 1, 0x03);
    mipi_dcs_write_cmd(0x66, 1, 0x5D);
    mipi_dcs_write_cmd(0x67, 1, 0x03);
    mipi_dcs_write_cmd(0x68, 1, 0x70);
    mipi_dcs_write_cmd(0x69, 1, 0x03);
    mipi_dcs_write_cmd(0x6A, 1, 0x88);
    mipi_dcs_write_cmd(0x6B, 1, 0x03);
    mipi_dcs_write_cmd(0x6C, 1, 0xA8);
    mipi_dcs_write_cmd(0x6D, 1, 0x03);
    mipi_dcs_write_cmd(0x6E, 1, 0xC8);
    mipi_dcs_write_cmd(0x6F, 1, 0x03);
    mipi_dcs_write_cmd(0x70, 1, 0xFF);
    //B(+) MCR cmd
    mipi_dcs_write_cmd(0x71, 1, 0x00);
    mipi_dcs_write_cmd(0x72, 1, 0x00);
    mipi_dcs_write_cmd(0x73, 1, 0x00);
    mipi_dcs_write_cmd(0x74, 1, 0x2C);
    mipi_dcs_write_cmd(0x75, 1, 0x00);
    mipi_dcs_write_cmd(0x76, 1, 0x4F);
    mipi_dcs_write_cmd(0x77, 1, 0x00);
    mipi_dcs_write_cmd(0x78, 1, 0x69);
    mipi_dcs_write_cmd(0x79, 1, 0x00);
    mipi_dcs_write_cmd(0x7A, 1, 0x7F);
    mipi_dcs_write_cmd(0x7B, 1, 0x00);
    mipi_dcs_write_cmd(0x7C, 1, 0x92);
    mipi_dcs_write_cmd(0x7D, 1, 0x00);
    mipi_dcs_write_cmd(0x7E, 1, 0xA3);
    mipi_dcs_write_cmd(0x7F, 1, 0x00);
    mipi_dcs_write_cmd(0x80, 1, 0xB3);
    mipi_dcs_write_cmd(0x81, 1, 0x00);
    mipi_dcs_write_cmd(0x82, 1, 0xC1);
    mipi_dcs_write_cmd(0x83, 1, 0x00);
    mipi_dcs_write_cmd(0x84, 1, 0xF3);
    mipi_dcs_write_cmd(0x85, 1, 0x01);
    mipi_dcs_write_cmd(0x86, 1, 0x1B);
    mipi_dcs_write_cmd(0x87, 1, 0x01);
    mipi_dcs_write_cmd(0x88, 1, 0x5A);
    mipi_dcs_write_cmd(0x89, 1, 0x01);
    mipi_dcs_write_cmd(0x8A, 1, 0x8B);
    mipi_dcs_write_cmd(0x8B, 1, 0x01);
    mipi_dcs_write_cmd(0x8C, 1, 0xD9);
    mipi_dcs_write_cmd(0x8D, 1, 0x02);
    mipi_dcs_write_cmd(0x8E, 1, 0x16);
    mipi_dcs_write_cmd(0x8F, 1, 0x02);
    mipi_dcs_write_cmd(0x90, 1, 0x18);
    mipi_dcs_write_cmd(0x91, 1, 0x02);
    mipi_dcs_write_cmd(0x92, 1, 0x4E);
    mipi_dcs_write_cmd(0x93, 1, 0x02);
    mipi_dcs_write_cmd(0x94, 1, 0x88);
    mipi_dcs_write_cmd(0x95, 1, 0x02);
    mipi_dcs_write_cmd(0x96, 1, 0xAC);
    mipi_dcs_write_cmd(0x97, 1, 0x02);
    mipi_dcs_write_cmd(0x98, 1, 0xDD);
    mipi_dcs_write_cmd(0x99, 1, 0x03);
    mipi_dcs_write_cmd(0x9A, 1, 0x01);
    mipi_dcs_write_cmd(0x9B, 1, 0x03);
    mipi_dcs_write_cmd(0x9C, 1, 0x2E);
    mipi_dcs_write_cmd(0x9D, 1, 0x03);
    mipi_dcs_write_cmd(0x9E, 1, 0x3C);
    mipi_dcs_write_cmd(0x9F, 1, 0x03);
    mipi_dcs_write_cmd(0xA0, 1, 0x4C);
    mipi_dcs_write_cmd(0xA2, 1, 0x03);
    mipi_dcs_write_cmd(0xA3, 1, 0x5D);
    mipi_dcs_write_cmd(0xA4, 1, 0x03);
    mipi_dcs_write_cmd(0xA5, 1, 0x70);
    mipi_dcs_write_cmd(0xA6, 1, 0x03);
    mipi_dcs_write_cmd(0xA7, 1, 0x88);
    mipi_dcs_write_cmd(0xA9, 1, 0x03);
    mipi_dcs_write_cmd(0xAA, 1, 0xA8);
    mipi_dcs_write_cmd(0xAB, 1, 0x03);
    mipi_dcs_write_cmd(0xAC, 1, 0xC8);
    mipi_dcs_write_cmd(0xAD, 1, 0x03);
    mipi_dcs_write_cmd(0xAE, 1, 0xFF);
    //B(-) MCR cmd
    mipi_dcs_write_cmd(0xAF, 1, 0x00);
    mipi_dcs_write_cmd(0xB0, 1, 0x00);
    mipi_dcs_write_cmd(0xB1, 1, 0x00);
    mipi_dcs_write_cmd(0xB2, 1, 0x2C);
    mipi_dcs_write_cmd(0xB3, 1, 0x00);
    mipi_dcs_write_cmd(0xB4, 1, 0x4F);
    mipi_dcs_write_cmd(0xB5, 1, 0x00);
    mipi_dcs_write_cmd(0xB6, 1, 0x69);
    mipi_dcs_write_cmd(0xB7, 1, 0x00);
    mipi_dcs_write_cmd(0xB8, 1, 0x7F);
    mipi_dcs_write_cmd(0xB9, 1, 0x00);
    mipi_dcs_write_cmd(0xBA, 1, 0x92);
    mipi_dcs_write_cmd(0xBB, 1, 0x00);
    mipi_dcs_write_cmd(0xBC, 1, 0xA3);
    mipi_dcs_write_cmd(0xBD, 1, 0x00);
    mipi_dcs_write_cmd(0xBE, 1, 0xB3);
    mipi_dcs_write_cmd(0xBF, 1, 0x00);
    mipi_dcs_write_cmd(0xC0, 1, 0xC1);
    mipi_dcs_write_cmd(0xC1, 1, 0x00);
    mipi_dcs_write_cmd(0xC2, 1, 0xF3);
    mipi_dcs_write_cmd(0xC3, 1, 0x01);
    mipi_dcs_write_cmd(0xC4, 1, 0x1B);
    mipi_dcs_write_cmd(0xC5, 1, 0x01);
    mipi_dcs_write_cmd(0xC6, 1, 0x5A);
    mipi_dcs_write_cmd(0xC7, 1, 0x01);
    mipi_dcs_write_cmd(0xC8, 1, 0x8B);
    mipi_dcs_write_cmd(0xC9, 1, 0x01);
    mipi_dcs_write_cmd(0xCA, 1, 0xD9);
    mipi_dcs_write_cmd(0xCB, 1, 0x02);
    mipi_dcs_write_cmd(0xCC, 1, 0x16);
    mipi_dcs_write_cmd(0xCD, 1, 0x02);
    mipi_dcs_write_cmd(0xCE, 1, 0x18);
    mipi_dcs_write_cmd(0xCF, 1, 0x02);
    mipi_dcs_write_cmd(0xD0, 1, 0x4E);
    mipi_dcs_write_cmd(0xD1, 1, 0x02);
    mipi_dcs_write_cmd(0xD2, 1, 0x88);
    mipi_dcs_write_cmd(0xD3, 1, 0x02);
    mipi_dcs_write_cmd(0xD4, 1, 0xAC);
    mipi_dcs_write_cmd(0xD5, 1, 0x02);
    mipi_dcs_write_cmd(0xD6, 1, 0xDD);
    mipi_dcs_write_cmd(0xD7, 1, 0x03);
    mipi_dcs_write_cmd(0xD8, 1, 0x01);
    mipi_dcs_write_cmd(0xD9, 1, 0x03);
    mipi_dcs_write_cmd(0xDA, 1, 0x2E);
    mipi_dcs_write_cmd(0xDB, 1, 0x03);
    mipi_dcs_write_cmd(0xDC, 1, 0x3C);
    mipi_dcs_write_cmd(0xDD, 1, 0x03);
    mipi_dcs_write_cmd(0xDE, 1, 0x4C);
    mipi_dcs_write_cmd(0xDF, 1, 0x03);
    mipi_dcs_write_cmd(0xE0, 1, 0x5D);
    mipi_dcs_write_cmd(0xE1, 1, 0x03);
    mipi_dcs_write_cmd(0xE2, 1, 0x70);
    mipi_dcs_write_cmd(0xE3, 1, 0x03);
    mipi_dcs_write_cmd(0xE4, 1, 0x88);
    mipi_dcs_write_cmd(0xE5, 1, 0x03);
    mipi_dcs_write_cmd(0xE6, 1, 0xA8);
    mipi_dcs_write_cmd(0xE7, 1, 0x03);
    mipi_dcs_write_cmd(0xE8, 1, 0xC8);
    mipi_dcs_write_cmd(0xE9, 1, 0x03);
    mipi_dcs_write_cmd(0xEA, 1, 0xFF);

    mipi_dcs_write_cmd(0xFF, 1, 0x00);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);

    mipi_dcs_write_cmd(0x11, 0);
    vTaskDelay(pdMS_TO_TICKS(120));
    mipi_dcs_write_cmd(0x29, 0);

    // mipi_dcs_write_cmd(0x01, 0);
    // vTaskDelay(pdMS_TO_TICKS(200));

    // mipi_dcs_write_cmd(0x11, 0);
    // vTaskDelay(pdMS_TO_TICKS(120));
    // mipi_dcs_write_cmd(0x29, 0);
    // mipi_dcs_write_cmd(0x30, 4, 0x0, 0x0, (MIPI_DSI_IMAGE_VSIZE - 1) >> 8, (MIPI_DSI_IMAGE_VSIZE - 1) & 0xFF);
    // // Refresh
    // mipi_dcs_write_cmd(0x36, 1, 0x00);
    // // Pixel Format
    // mipi_dcs_write_cmd(0x3A, 1, 0x75);

    // mipi_dcs_write_cmd(0xBA, 1, 0x01);
    // // Normal Display Mode On
    // // mipi_dcs_write_cmd(0x12, 0);
    // mipi_dcs_write_cmd(0x13, 0);

#endif

#if TEST_DSI_ICN6211_800
    ESP_LOGI(TAG, "init ICN6211 800x480\n");
    vTaskDelay(pdMS_TO_TICKS(200));

    mipi_gen_write_cmd(0x7A, 1, 0xC1);
    mipi_gen_write_cmd(0x20, 1, 0x20);
    mipi_gen_write_cmd(0x21, 1, 0xE0);
    mipi_gen_write_cmd(0x22, 1, 0x13);
    mipi_gen_write_cmd(0x23, 1, 0x64);
    mipi_gen_write_cmd(0x24, 1, 0x50);
    mipi_gen_write_cmd(0x25, 1, 0x64);
    mipi_gen_write_cmd(0x26, 1, 0x00);
    mipi_gen_write_cmd(0x27, 1, 0x08);
    mipi_gen_write_cmd(0x28, 1, 0x04);
    mipi_gen_write_cmd(0x29, 1, 0x04);
    mipi_gen_write_cmd(0x34, 1, 0x80);
    mipi_gen_write_cmd(0x36, 1, 0x64);
    mipi_gen_write_cmd(0x86, 1, 0x29);
    mipi_gen_write_cmd(0xB5, 1, 0xA0);
    mipi_gen_write_cmd(0x5C, 1, 0xFF);
    mipi_gen_write_cmd(0x2A, 1, 0x01);
    mipi_gen_write_cmd(0x56, 1, 0x90);
    mipi_gen_write_cmd(0x6B, 1, 0x71);
    mipi_gen_write_cmd(0x69, 1, 0x28);
    mipi_gen_write_cmd(0x10, 1, 0x40);
    mipi_gen_write_cmd(0x11, 1, 0x88);
    mipi_gen_write_cmd(0xB6, 1, 0x20);
    mipi_gen_write_cmd(0x51, 1, 0x20);
    // mipi_gen_write_cmd(0x14, 1, 0x43);
    // mipi_gen_write_cmd(0x2A, 1, 0x49);
    mipi_gen_write_cmd(0x09, 1, 0x10);

    uint8_t cmd_data[16];
    for (int x = 0; x < 16; x++) {
        vTaskDelay(pdMS_TO_TICKS(200)); // must delay
        mipi_gen_read_cmd(0x20 + x, 1, &cmd_data[x]);
        ESP_LOGI(TAG, "cmd[0x%x]: 0x%x\n", 0x20 + x, cmd_data[x]);
    }
    vTaskDelay(pdMS_TO_TICKS(200));

#endif

#if TEST_DSI_ICN6211_1280
    ESP_LOGI(TAG, "init ICN6211 1280x800\n");
    vTaskDelay(pdMS_TO_TICKS(200));

    mipi_gen_write_cmd(0x7A, 1, 0xC1);
    mipi_gen_write_cmd(0x20, 1, 0x00);
    mipi_gen_write_cmd(0x21, 1, 0x20);
    mipi_gen_write_cmd(0x22, 1, 0x35);
    mipi_gen_write_cmd(0x23, 1, 0x78);
    mipi_gen_write_cmd(0x24, 1, 0x72);
    mipi_gen_write_cmd(0x25, 1, 0x78);
    mipi_gen_write_cmd(0x26, 1, 0x00);
    mipi_gen_write_cmd(0x27, 1, 0x08);
    mipi_gen_write_cmd(0x28, 1, 0x04);
    mipi_gen_write_cmd(0x29, 1, 0x04);
    mipi_gen_write_cmd(0x34, 1, 0x80);
    mipi_gen_write_cmd(0x36, 1, 0x78);
    mipi_gen_write_cmd(0x86, 1, 0x29);
    mipi_gen_write_cmd(0xB5, 1, 0xA0);
    mipi_gen_write_cmd(0x5C, 1, 0xFF);
    mipi_gen_write_cmd(0x2A, 1, 0x01);
    mipi_gen_write_cmd(0x56, 1, 0x90);
    mipi_gen_write_cmd(0x6B, 1, 0x51);
    mipi_gen_write_cmd(0x69, 1, 0x32);
    mipi_gen_write_cmd(0x10, 1, 0x40);
    mipi_gen_write_cmd(0x11, 1, 0x88);
    mipi_gen_write_cmd(0xB6, 1, 0x20);
    mipi_gen_write_cmd(0x51, 1, 0x20);
    // mipi_gen_write_cmd(0x14, 1, 0x43);
    // mipi_gen_write_cmd(0x2A, 1, 0x49);
    mipi_gen_write_cmd(0x09, 1, 0x10);

    uint8_t cmd_data[16];
    for (int x = 0; x < 16; x++) {
        vTaskDelay(pdMS_TO_TICKS(200)); // must delay
        mipi_gen_read_cmd(0x20 + x, 1, &cmd_data[x]);
        ESP_LOGI(TAG, "cmd[0x%x]: 0x%x\n", 0x20 + x, cmd_data[x]);
    }
    vTaskDelay(pdMS_TO_TICKS(200));

#endif

#if TEST_DSI_ESCAPE_MODE
    // 可以断开 clk lane, 观察data lane0 是否进入了escape mode
    uint16_t data[128];

    ESP_LOGI(TAG, "esacpe mode test\n");

    for (int x = 0; x < 128; x++) {
        data[x] = 0x001F;
    }

    for (int x = 0; x < 100; x++) {
        // Memory write
        mipi_dcs_write_data((uint8_t *)data, 128 * 2);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    mipi_dcs_write_cmd(0x29, 0); //Display ON
#endif
#endif

    // MIPI_DSI_HOST.phy_ulps_ctrl.phy_txexitulpsclk = 1;
    mipi_dsih_hal_gen_cmd_mode_en(0);

    MIPI_DSI_HOST.lpclk_ctrl.val = 0x00000000 |
                                   ((1 & 0x00000001) << 1) | // bit[    1] AUTO_CLKLANE_CTRL     = 0 (Automatic mechanism to stop providing clock in the clock lane when time allows.
                                   ((1 & 0x00000001) << 0);  // bit[    0] TXREQUESTCLKHS        = 1 (Requesting High Speed Clock Transmission.
#if TEST_DSI_PATTERN
    MIPI_DSI_HOST.vid_mode_cfg.val = 0x00000000 |
                                     ((1 & 0x00000001) << 24) | // bit[   24] VPG_ORIENTATION       = 1 (Video Pattern Generator Orientation - 0=Vertical / 1=Horizontal
                                     ((0 & 0x00000001) << 20) | // bit[   20] VPG_MODE              = 0 (Video Pattern Generator Mode        - 0=ColorBAr / 1=BER Testing
                                     ((1 & 0x00000001) << 16) | // bit[   16] VPG_EN                = 0 (Video Pattern Generator Activation
                                     ((0 & 0x00000001) << 15) | // bit[   15] LPCMDEN               = 0 (Enables Low Power command transmission.
                                     ((1 & 0x00000001) << 13) | // bit[   13] LP_HFP_EN             = 1 (Enables Low Power HFP.
                                     ((1 & 0x00000001) << 12) | // bit[   12] LP_HBP_EN             = 1 (Enables Low Power HBP.
                                     ((1 & 0x00000001) << 11) | // bit[   11] LP_VACT_EN            = 1 (Enables Low Power VACT.
                                     ((1 & 0x00000001) << 10) | // bit[   10] LP_VFP_EN             = 1 (Enables Low Power VFP.
                                     ((1 & 0x00000001) << 9) |  // bit[    9] LP_VBP_EN             = 1 (Enables Low Power VBP.
                                     ((1 & 0x00000001) << 8) |  // bit[    8] LP_VSA_EN             = 1 (Enables Low Power VSA.
                                     ((2 & 0x00000003) << 0);   // bit[ 1: 0] VIDEO_MODE            = 2 (Video transmission mode. - 2=Burst Mode
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
#endif

#if TEST_DISP_PATTERN
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
    mipi_dsi_data_t *tx_data = (mipi_dsi_data_t *)dsi_frame_buf
                               mipi_dsi_data_t line[MIPI_DSI_DISP_HSIZE];
    uint32_t dsi_size = sizeof(mipi_dsi_data_t);
    uint32_t data = 0;

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
#endif

    // Enable GDMA CH-2 transfer.
    GDMA.chen0.val = ((GDMA.chen0.val) | 0x202);
    esp_rom_delay_us(10);

    mipi_dsi_bridge_start();

    ESP_LOGI(TAG, "[MAIN] MIPI transfer start.\n");

    return ESP_OK;
}
