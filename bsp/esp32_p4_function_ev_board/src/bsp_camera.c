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
#include "mipi_csi.h"
#include "mipi_dsi.h"

#include "sdkconfig.h"
#include "bsp_err_check.h"
#include "bsp/esp32_p4_function_ev_board.h"
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

static esp_err_t bsp_camera_init(uint16_t hor_res, uint16_t ver_res, void **buffer)
{
    uint32_t csi_block_bytes = 0;
    uint32_t csi_block_ts = 0;
    void *dma_buffer = NULL;
    bool dma_buffer_allocated = false;

    if (TEST_CSI_COLOR_MODE >= MIPI_CSI_RAW8_MODE) {
        csi_block_bytes = hor_res * ver_res * TEST_DSI_COLOR_WIDTH / 8;
    } else {
        csi_block_bytes = hor_res * ver_res * TEST_CSI_COLOR_WIDTH / 8;
    }
    csi_block_ts = csi_block_bytes * 8 / TEST_CSI_TR_WIDTH;

    if (*buffer == NULL) {
        dma_buffer = heap_caps_aligned_alloc(TEST_CSI_TR_WIDTH, csi_block_bytes, MALLOC_CAP_SPIRAM);
        if (dma_buffer == NULL) {
            ESP_LOGE(TAG, "Failed to allocate DMA buffer");
            return ESP_ERR_NO_MEM;
        }
        dma_buffer_allocated = true;
    } else {
        dma_buffer = *buffer;
    }

    return ESP_OK;
}
