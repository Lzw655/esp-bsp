/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "esp_check.h"
#include "esp_err.h"

#include "hp_sys_clkrst_reg.h"
#include "interrupt_core0_reg.h"
#include "gdma_struct.h"
#include "mipi_dsi.h"
#include "mipi_csi.h"

#define MIPI_LCD_DMA_TRANS_DAR      (0x50105000)
#define MIPI_CAM_DMA_TRANS_SAR      (0x50104000)
#define DW_GDMA_INTR_SOURCE         (((INTERRUPT_CORE0_GDMA_INT_MAP_REG - DR_REG_INTERRUPT_CORE0_BASE) / 4))
#define DW_GDMA_BUFFER_MASTER       (1)

static const char *TAG = "dw_gdma";

static bool dw_gdma_initialized = false;
static bool dw_gdma_started = false;

static volatile uint8_t *dsi_dma_buf = NULL;
static volatile uint32_t dsi_frame_cnt = 0;
static volatile uint8_t *csi_dma_buf = NULL;
static volatile uint32_t csi_frame_cnt = 0;

static void dw_gdma_isr()
{
    uint32_t rtn = 0;

    // ESP_LOGI(TAG, "[ISR] Enter into DW_GDMA ISR.\n");

    // Clear interrupt.
    rtn = (DW_GDMA.int_st0.val);

    // If is channel 1 (mipi-csi) interrupt event.
    if (rtn & 0x1) {
        if (DW_GDMA.ch[0].int_st0.val) { // Is channel 1 "transfer done" interrupt event.
            typeof(DW_GDMA.ch[0].int_st0) status = DW_GDMA.ch[0].int_st0;

            if (status.dma_tfr_done) {
                // gpio_probe(MIPI_CSI_PROBE_IO, 1);

                // if (TEST_MULTI_FRAME) {
                //     switch (buf_wr_ptr) {
                //         case 0 :
                //             buf_wr_ptr = 1;
                //             break;

                //         case 1 :
                //             buf_wr_ptr = 2;
                //             break;

                //         case 2 :
                //             buf_wr_ptr = 0;
                //             break;

                //         default:
                //             ;
                //     }
                // }

                DW_GDMA.ch[0].dar0 = (uint32_t)csi_dma_buf;
                DW_GDMA.chen0.val = ((DW_GDMA.chen0.val) | 0x101);

                // csi_frame_cnt++;
                // gpio_probe(MIPI_CSI_PROBE_IO, 0);
            }

            DW_GDMA.ch[0].int_clr0.val = 0xffffffff;
        }
    }

    // If is channel 2 (mipi-dsi) interrupt event.
    if (rtn & 0x2) {
        if (DW_GDMA.ch[1].int_st0.val) { // Is channel 2 "transfer done" interrupt event.
            typeof(DW_GDMA.ch[1].int_st0) status = DW_GDMA.ch[1].int_st0;

            if (status.dma_tfr_done) {
                // gpio_probe(MIPI_DSI_PROBE_IO, 1);

                DW_GDMA.ch[1].sar0 = (uint32_t)dsi_dma_buf;
                DW_GDMA.chen0.val = ((DW_GDMA.chen0.val) | 0x202);

                // dsi_frame_cnt++;
                // gpio_probe(MIPI_DSI_PROBE_IO, 0);
            }

            // Clear all interrupt of ch1
            DW_GDMA.ch[1].int_clr0.val = 0xffffffff;
        }
    }
}

static esp_err_t dw_gdma_common_init(void)
{
    intr_handle_t intr_handle = NULL;

    if (dw_gdma_initialized) {
        return ESP_OK;
    }

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_GDMA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_GDMA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_GDMA);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_GDMA);

    // Enable DW_GDMA.
    DW_GDMA.cfg0.val = 0x0;
    DW_GDMA.cfg0.dmac_en = 0x1;
    DW_GDMA.cfg0.int_en = 0x1;

    ESP_RETURN_ON_ERROR(esp_intr_alloc(DW_GDMA_INTR_SOURCE, ESP_INTR_FLAG_LEVEL2, dw_gdma_isr, NULL, &intr_handle), TAG,
                        "Allocate DW_GDMA interrupt failed");

    dw_gdma_initialized = true;

    ESP_LOGI(TAG, "DW_GDMA common initialization done.");

    return ESP_OK;
}

esp_err_t dw_gdma_mipi_dsi_init(void *buffer, size_t buffer_size, uint8_t tr_width)
{
    ESP_RETURN_ON_FALSE(buffer != NULL, ESP_ERR_INVALID_ARG, TAG, "Invalid buffer");
    ESP_RETURN_ON_FALSE(buffer_size > 0, ESP_ERR_INVALID_ARG, TAG, "Invalid buffer size");

    ESP_RETURN_ON_ERROR(dw_gdma_common_init(), TAG, "DW_GDMA common initialization failed");

    uint32_t block_ts = buffer_size * 8 / tr_width;
    dsi_dma_buf = buffer;

    DW_GDMA.ch[1].int_st_ena0.val = 0x0;
    DW_GDMA.ch[1].int_sig_ena0.val = 0x0;
    DW_GDMA.ch[1].int_st_ena0.dma_tfr_done = 0x1;
    DW_GDMA.ch[1].int_sig_ena0.dma_tfr_done = 0x1;
    DW_GDMA.ch[1].cfg0.val = 0x0;
    DW_GDMA.ch[1].cfg0.wr_uid = 0x3;
    DW_GDMA.ch[1].cfg0.rd_uid = 0x3;
    DW_GDMA.ch[1].cfg0.src_multblk_type = 0x0;
    DW_GDMA.ch[1].cfg0.dst_multblk_type = 0x0;
    DW_GDMA.ch[1].cfg1.val = 0x0;
    DW_GDMA.ch[1].cfg1.tt_fc = 0x6;
    DW_GDMA.ch[1].cfg1.hs_sel_src = 0x0;
    DW_GDMA.ch[1].cfg1.hs_sel_dst = 0x0;
    DW_GDMA.ch[1].cfg1.src_per = 0x0;
    DW_GDMA.ch[1].cfg1.dst_per = 0x0;
    DW_GDMA.ch[1].cfg1.src_osr_lmt = 0x4;
    DW_GDMA.ch[1].cfg1.dst_osr_lmt = 0x1;
    DW_GDMA.ch[1].sar0 = (uint32_t)buffer;
    DW_GDMA.ch[1].dar0 = MIPI_LCD_DMA_TRANS_DAR;
    DW_GDMA.ch[1].block_ts0.val =  block_ts - 1;
    DW_GDMA.ch[1].ctl0.val = 0x0;
    DW_GDMA.ch[1].ctl0.sms = DW_GDMA_BUFFER_MASTER;
    DW_GDMA.ch[1].ctl0.dms = 0x0;
    DW_GDMA.ch[1].ctl0.sinc = 0x0;
    DW_GDMA.ch[1].ctl0.dinc = 0x1;
    DW_GDMA.ch[1].ctl0.src_tr_width = (tr_width == 64 ? 0x3 : 0x2);
    DW_GDMA.ch[1].ctl0.dst_tr_width = (tr_width == 64 ? 0x3 : 0x2);
    DW_GDMA.ch[1].ctl0.src_msize = 0x8;
    DW_GDMA.ch[1].ctl0.dst_msize = 0x7;
    DW_GDMA.ch[1].ctl1.ioc_blktfr = 0;
    DW_GDMA.ch[1].ctl1.arlen = 16;
    DW_GDMA.ch[1].ctl1.arlen_en = 1;
    DW_GDMA.ch[1].ctl1.awlen = 16;
    DW_GDMA.ch[1].ctl1.awlen_en = 1;

    ESP_LOGI(TAG, "DW_GDMA mipi-dsi initialization done.");

    return ESP_OK;
}

esp_err_t dw_gdma_mipi_csi_init(void *buffer, size_t buffer_size, uint8_t tr_width)
{
    ESP_RETURN_ON_FALSE(buffer != NULL, ESP_ERR_INVALID_ARG, TAG, "Invalid buffer");
    ESP_RETURN_ON_FALSE(buffer_size > 0, ESP_ERR_INVALID_ARG, TAG, "Invalid buffer size");

    ESP_RETURN_ON_ERROR(dw_gdma_common_init(), TAG, "DW_GDMA common initialization failed");

    uint32_t block_ts = buffer_size * 8 / tr_width;
    csi_dma_buf = buffer;

    DW_GDMA.ch[0].int_st_ena0.val = 0x0;
    DW_GDMA.ch[0].int_sig_ena0.val = 0x0;
    DW_GDMA.ch[0].int_st_ena0.dma_tfr_done = 0x1;
    DW_GDMA.ch[0].int_sig_ena0.dma_tfr_done = 0x1;
    DW_GDMA.ch[0].cfg0.val = 0x0;
    DW_GDMA.ch[0].cfg0.wr_uid = 0x2;
    DW_GDMA.ch[0].cfg0.rd_uid = 0x2;
    DW_GDMA.ch[0].cfg0.src_multblk_type = 0x0;
    DW_GDMA.ch[0].cfg0.dst_multblk_type = 0x0;
    DW_GDMA.ch[0].cfg1.val = 0x0;
    DW_GDMA.ch[0].cfg1.tt_fc = 0x4;
    DW_GDMA.ch[0].cfg1.hs_sel_src = 0x0;
    DW_GDMA.ch[0].cfg1.hs_sel_dst = 0x0;
    DW_GDMA.ch[0].cfg1.src_per = 0x1;
    DW_GDMA.ch[0].cfg1.dst_per = 0x0;
    DW_GDMA.ch[0].cfg1.src_osr_lmt = 0x1;
    DW_GDMA.ch[0].cfg1.dst_osr_lmt = 0x4;
    DW_GDMA.ch[0].sar0 = MIPI_CAM_DMA_TRANS_SAR;
    DW_GDMA.ch[0].dar0 = (uint32_t)buffer;
    DW_GDMA.ch[0].block_ts0.val = block_ts - 1;
    DW_GDMA.ch[0].ctl0.val = 0x0;
    DW_GDMA.ch[0].ctl0.sms = 0x0;
    DW_GDMA.ch[0].ctl0.dms = DW_GDMA_BUFFER_MASTER;
    DW_GDMA.ch[0].ctl0.sinc = 0x1;
    DW_GDMA.ch[0].ctl0.dinc = 0x0;
    DW_GDMA.ch[0].ctl0.src_tr_width = (TEST_CSI_TR_WIDTH == 64 ? 0x3 : 0x2);
    DW_GDMA.ch[0].ctl0.dst_tr_width = (TEST_CSI_TR_WIDTH == 64 ? 0x3 : 0x2);
    DW_GDMA.ch[0].ctl0.src_msize = 0x7;
    DW_GDMA.ch[0].ctl0.dst_msize = 0x8;
    DW_GDMA.ch[0].ctl1.ioc_blktfr = 0;
    DW_GDMA.ch[0].sstatar0 = MIPI_CAM_DMA_TRANS_SAR + 16;
    DW_GDMA.ch[0].ctl1.arlen = 16;
    DW_GDMA.ch[0].ctl1.arlen_en = 1;
    DW_GDMA.ch[0].ctl1.awlen = 16;
    DW_GDMA.ch[0].ctl1.awlen_en = 1;

    ESP_LOGI(TAG, "DW_GDMA mipi-csi initialization done.");

    return ESP_OK;
}

esp_err_t dw_gdma_start(void)
{
    if (dw_gdma_started) {
        return ESP_OK;
    }

    DW_GDMA.chen0.val = ((DW_GDMA.chen0.val) | 0x202);
    dw_gdma_started = true;

    ESP_LOGI(TAG, "DW_GDMA start.");

    return ESP_OK;
}
