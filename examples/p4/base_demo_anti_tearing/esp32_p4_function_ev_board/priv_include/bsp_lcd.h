/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t bsp_lcd_init(void);

esp_err_t bsp_lcd_start(void);

esp_err_t bsp_lcd_get_frame_buffer(uint32_t fb_num, void **fb0, ...);

esp_err_t bsp_lcd_drawbitmap(int x_start, int y_start, int x_end, int y_end, const void *color_data);

#ifdef __cplusplus
}
#endif
