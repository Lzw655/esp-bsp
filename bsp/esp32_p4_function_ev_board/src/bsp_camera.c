/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "esp_err.h"
#include "esp_check.h"
#include "esp_log.h"

#include "interrupt_core0_reg.h"
#include "mipi_csi_host_struct.h"
#include "gdma_struct.h"

#include "sdkconfig.h"
#include "bsp_err_check.h"
#include "bsp/esp32_p4_function_ev_board.h"
#include "mipi_csi.h"
#include "mipi_dsi.h"
#include "bsp/camera.h"

#define MIPI_CAM_DMA_TRANS_SAR  (0x50104000)
#define GDMA_INTR_SOURCE        (((INTERRUPT_CORE0_GDMA_INT_MAP_REG - DR_REG_INTERRUPT_CORE0_BASE) / 4))
#define GDMA_BUFFER_MASTER      (1)

static const char *TAG = "bsp_camera";

esp_err_t bsp_camera_new(const bsp_camera_config_t *config)
{
    mipi_csi_init();

    vTaskDelay(pdMS_TO_TICKS(1));

    return ESP_OK;
}

static esp_err_t bsp_camera_init(void *buffer, size_t size)
{
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
    GDMA.ch[0].dar0 = buffer;
    GDMA.ch[0].block_ts0.val = size - 1;
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

    return ESP_OK;
}
