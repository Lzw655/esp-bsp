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

#define TEST_ISP_3A          (0)
#if TEST_CSI_OV5647 && TEST_ISP_3A
#define TEST_CSI_AWB         (1)
#define TEST_CSI_AE          (0)
#define TEST_CSI_AF          (1)
#define TEST_CSI_HIST        (0)
#endif
#if TEST_CSI_IMX219 && TEST_ISP_3A
#define TEST_CSI_AWB         (1)
#define TEST_CSI_HIST        (0)
#endif

esp_err_t isp_init(uint16_t hor_res, uint16_t ver_res);

#ifdef __cplusplus
}
#endif
