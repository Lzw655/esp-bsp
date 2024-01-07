/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#pragma once

#include <stdio.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*dw_gdma_callback_t)(void *arg);

esp_err_t dw_gdma_mipi_dsi_init(void *buffer, size_t buffer_size, uint8_t tr_width);
esp_err_t dw_gdma_mipi_dsi_start(void);
void dw_gdma_mipi_dsi_set_buffer(void *buffer);
uint32_t dw_gdma_mipi_dsi_get_frame_count(void);
void dw_gdma_mipi_dsi_register_callback(dw_gdma_callback_t callback, void *user_data);

esp_err_t dw_gdma_mipi_csi_init(void *buffer, size_t buffer_size, uint8_t tr_width);
esp_err_t dw_gdma_mipi_csi_start(void);
void dw_gdma_mipi_csi_set_buffer(void *buffer);
uint32_t dw_gdma_mipi_csi_get_frame_count(void);
void dw_gdma_mipi_csi_register_callback(dw_gdma_callback_t callback, void *user_data);

#ifdef __cplusplus
}
#endif
