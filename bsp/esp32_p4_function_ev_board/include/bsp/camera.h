/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t width;
    uint16_t height;
    void **buffer;
} bsp_camera_config_t;

esp_err_t bsp_camera_new(const bsp_camera_config_t *config);

#ifdef __cplusplus
}
#endif
