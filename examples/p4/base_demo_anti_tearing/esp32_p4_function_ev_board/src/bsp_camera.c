/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "esp_err.h"
#include "esp_check.h"
#include "esp_log.h"

#include "mipi_csi_host_struct.h"
#include "dw_gdma.h"
#include "mipi_csi.h"
#include "mipi_dsi.h"
#include "sensor.h"
#include "isp.h"

#include "sdkconfig.h"
#include "bsp_err_check.h"
#include "bsp/esp32_p4_function_ev_board.h"
#include "bsp/camera.h"
#include "bsp/esp-bsp.h"

#define MIPI_CAM_DMA_TRANS_SAR  (0x50104000)
#define GDMA_INTR_SOURCE        (((INTERRUPT_CORE0_GDMA_INT_MAP_REG - DR_REG_INTERRUPT_CORE0_BASE) / 4))
#define GDMA_BUFFER_MASTER      (1)

static const char *TAG = "bsp_camera";
static bsp_camera_trans_done_cb_t trans_done = NULL;

static void csi_on_vsync_event(void *arg);

esp_err_t bsp_camera_new(const bsp_camera_config_t *config)
{
    ESP_RETURN_ON_FALSE(config != NULL, ESP_ERR_INVALID_ARG, TAG, "Invalid configuration");
    ESP_RETURN_ON_FALSE((config->buffer_ptr != NULL) && (config->buffer_size_ptr != NULL), ESP_ERR_INVALID_ARG, TAG,
                        "Invalid buffer pointer");
    ESP_RETURN_ON_FALSE((*config->buffer_size_ptr == 0) || (*config->buffer_ptr != NULL), ESP_ERR_INVALID_ARG, TAG,
                        "Invalid buffer configuration");

    ESP_RETURN_ON_ERROR(bsp_i2c_init(), TAG, "Initialize I2C failed");

    uint32_t csi_block_bytes = 0;
    void *dma_buffer = *config->buffer_ptr;
    bool external_dma_buffer = (*config->buffer_ptr != NULL);

    if (TEST_CSI_COLOR_MODE >= MIPI_CSI_RAW8_MODE) {
        csi_block_bytes = config->hor_res * config->ver_res * TEST_DSI_COLOR_WIDTH / 8;
    } else {
        csi_block_bytes = config->hor_res * config->ver_res * TEST_CSI_COLOR_WIDTH / 8;
    }

    if (!external_dma_buffer) {
        dma_buffer = heap_caps_aligned_alloc(TEST_CSI_TR_WIDTH, csi_block_bytes, MALLOC_CAP_SPIRAM);
        ESP_RETURN_ON_FALSE(dma_buffer != NULL, ESP_ERR_NO_MEM, TAG, "Allocate DMA buffer_ptr failed");

        *config->buffer_ptr = dma_buffer;
        *config->buffer_size_ptr = csi_block_bytes;

        ESP_LOGI(TAG, "Allocate DMA buffer at %p, size: %d", dma_buffer, csi_block_bytes);
    } else {
        ESP_RETURN_ON_FALSE(*config->buffer_size_ptr >= csi_block_bytes, ESP_ERR_INVALID_ARG, TAG,
                            "External DMA buffer_ptr is too small");
        ESP_LOGI(TAG, "Use external DMA buffer %p, size: %d", dma_buffer, csi_block_bytes);
    }

    ESP_RETURN_ON_ERROR(mipi_csi_clock_init(), TAG, "Initialize MIPI CSI clock failed");

    ESP_RETURN_ON_ERROR(isp_init(config->hor_res, config->ver_res), TAG, "Initialize ISP failed");

    ESP_RETURN_ON_ERROR(mipi_csi_host_phy_init(), TAG, "Initialize MIPI CSI Host PHY failed");

    ESP_RETURN_ON_ERROR(dw_gdma_mipi_csi_init(dma_buffer, csi_block_bytes, TEST_CSI_TR_WIDTH), TAG,
                        "Initialize GDMA for MIPI CSI failed");

    ESP_RETURN_ON_ERROR(dw_gdma_mipi_csi_start(), TAG, "Start GDMA for MIPI CSI failed");

    ESP_RETURN_ON_ERROR(mipi_csi_bridge_init(), TAG, "Initialize MIPI CSI Bridge failed");

#if TEST_CSI_OV5647
    ESP_RETURN_ON_ERROR(sensor_ov5647_init(), TAG, "Initialize OV5647 failed");
#endif

    dw_gdma_mipi_csi_register_callback(csi_on_vsync_event, NULL);

    return ESP_OK;
}

esp_err_t bsp_camera_register_trans_done_callback(bsp_camera_trans_done_cb_t callback)
{
    trans_done = callback;

    return ESP_OK;
}

esp_err_t bsp_camera_set_frame_buffer(void *buffer)
{
    ESP_RETURN_ON_FALSE(buffer != NULL, ESP_ERR_INVALID_ARG, TAG, "Invalid buffer_ptr");

    dw_gdma_mipi_csi_set_buffer(buffer);

    return ESP_OK;
}

esp_err_t bsp_camera_del(void)
{

    return ESP_OK;
}

IRAM_ATTR static void csi_on_vsync_event(void *arg)
{
    BaseType_t need_yield = pdFALSE;
    if (trans_done) {
        if (trans_done()) {
            need_yield = pdTRUE;
        }
    }

    if (need_yield) {
        portYIELD_FROM_ISR();
    }
}