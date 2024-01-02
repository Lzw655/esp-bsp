/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#pragma once

#include <stdint.h>
#include "esp_lcd_panel_vendor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  MIPI_DSI_LINE_RATE         (1000000000)
#define  MIPI_DSI_LINE_NUM          (2)
#define  MIPI_DPI_CLOCK_RATE        (40000000)

#define  MIPI_DSI_IMAGE_HSIZE       (800)
#define  MIPI_DSI_IMAGE_VSIZE       (1280)

#define  MIPI_DSI_IMAGE_HSYNC       (40)
#define  MIPI_DSI_IMAGE_HBP         (140)
#define  MIPI_DSI_IMAGE_HFP         (40)
#define  MIPI_DSI_IMAGE_VSYNC       (4)
#define  MIPI_DSI_IMAGE_VBP         (16)
#define  MIPI_DSI_IMAGE_VFP         (16)

esp_err_t esp_lcd_new_panel_ili9881(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config,
                                    esp_lcd_panel_handle_t *ret_panel);

#ifdef __cplusplus
}
#endif
