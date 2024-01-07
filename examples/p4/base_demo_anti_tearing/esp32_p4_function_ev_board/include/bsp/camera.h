/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdio.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t hor_res;
    uint16_t ver_res;
    void **buffer_ptr;
    size_t *buffer_size_ptr;
} bsp_camera_config_t;

typedef bool (*bsp_camera_trans_done_cb_t)(void);

esp_err_t bsp_camera_new(const bsp_camera_config_t *config);

esp_err_t bsp_camera_register_trans_done_callback(bsp_camera_trans_done_cb_t callback);

esp_err_t bsp_camera_set_frame_buffer(void *buffer);

#ifdef __cplusplus
}
#endif
