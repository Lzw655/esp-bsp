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

esp_err_t dw_gdma_mipi_dsi_init(void *buffer, size_t buffer_size, uint8_t tr_width);

esp_err_t dw_gdma_mipi_csi_init(void *buffer, size_t buffer_size, uint8_t tr_width);

esp_err_t dw_gdma_mipi_dsi_start(void);

esp_err_t dw_gdma_mipi_csi_start(void);

#ifdef __cplusplus
}
#endif
