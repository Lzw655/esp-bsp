/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "esp32p4/rom/ets_sys.h"
#include "mipi_csi_host_struct.h"
#include "mipi_csi_bridge_struct.h"
#include "isp_struct.h"

#define MIPI_CSI_RGB888_MODE (0)
#define MIPI_CSI_RGB666_MODE (1)
#define MIPI_CSI_RGB565_MODE (2)
#define MIPI_CSI_YUV422_MODE (3)
#define MIPI_CSI_YUV420_MODE (4)
#define MIPI_CSI_RAW8_MODE   (8)
#define MIPI_CSI_RAW10_MODE  (10)
#define MIPI_CSI_RAW12_MODE  (12)

#define TEST_CSI_FPGA        (0)
#define TEST_CSI_COLOR_MODE  (MIPI_CSI_RAW8_MODE)
#if (TEST_CSI_COLOR_MODE==MIPI_CSI_RGB888_MODE)
#define TEST_CSI_COLOR_WIDTH (24)
#elif (TEST_CSI_COLOR_MODE==MIPI_CSI_RGB666_MODE)
#define TEST_CSI_COLOR_WIDTH (18)
#elif (TEST_CSI_COLOR_MODE==MIPI_CSI_RGB565_MODE)
#define TEST_CSI_COLOR_WIDTH (16)
#elif (TEST_CSI_COLOR_MODE==MIPI_CSI_YUV422_MODE)
#define TEST_CSI_COLOR_WIDTH (16)
#elif (TEST_CSI_COLOR_MODE==MIPI_CSI_YUV420_MODE)
#define TEST_CSI_COLOR_WIDTH (12)
#elif (TEST_CSI_COLOR_MODE==MIPI_CSI_RAW8_MODE)
#define TEST_CSI_COLOR_WIDTH (8)
#elif (TEST_CSI_COLOR_MODE==MIPI_CSI_RAW10_MODE)
#define TEST_CSI_COLOR_WIDTH (10)
#elif (TEST_CSI_COLOR_MODE==MIPI_CSI_RAW12_MODE)
#define TEST_CSI_COLOR_WIDTH (12)
#endif
#define TEST_CSI_TR_WIDTH    (64)
#define TEST_CSI_PATTERN     (0)
#define TEST_CSI_LINESYNC    (0)
#define TEST_CSI_OV5640      (0)
#define TEST_CSI_OV5647      (1)
#define TEST_CSI_IMX219      (0)
#define TEST_CSI_720P        ((MIPI_CSI_IMAGE_VSIZE <= 960) ? 1 : 0)

#if TEST_CSI_IMX219
#define MIPI_IDI_CLOCK_RATE (57000000 * 1)
#elif TEST_CSI_OV5647
#define MIPI_IDI_CLOCK_RATE (50000000)
#else
#define MIPI_IDI_CLOCK_RATE (30000000)
#endif

#if (TEST_CSI_COLOR_MODE == MIPI_CSI_RAW12_MODE)
#define MIPI_CSI_LINE_RATE (MIPI_IDI_CLOCK_RATE * 6) //line_rate = pclk * 6
#elif (TEST_CSI_COLOR_MODE == MIPI_CSI_RAW10_MODE)
#define MIPI_CSI_LINE_RATE (MIPI_IDI_CLOCK_RATE * 5) //line_rate = pclk * 5
#else
#define MIPI_CSI_LINE_RATE (MIPI_IDI_CLOCK_RATE * 4) //line_rate = pclk * 4
#endif

#define MIPI_CSI_IMAGE_HSIZE           800
// #define MIPI_CSI_IMAGE_VSIZE           1280 / 2
#define MIPI_CSI_IMAGE_VSIZE           0

int mipi_csi_bridge_initialization (void) ;

/*
 * @brief  : Initialization of MIPI-DSI, including Bridge,
 *           Host-Controller and D-PHY.
 * @params : void
 * @ret    : int
 **/
int mipi_csi_initialization(void);
