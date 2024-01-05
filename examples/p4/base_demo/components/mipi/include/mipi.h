#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "dsi.h"
#include "csi.h"
#include "sccb.h"
#include "ov5640.h"
#include "ov5647.h"
#include "imx219.h"
#include "dw9714.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Don't modify the following macros */
#define BOARD_TYPE_FLY_LINE     (0)
#define BOARD_TYPE_FIB          (1)
#define BOARD_TYPE_SAMPLE       (2)

/* Change this macro to choose the target board */
#define BOARD_TYPE              (BOARD_TYPE_SAMPLE)

#define TEST_DISP_PATTERN   (0)

#define MIPI_CSI_PROBE_IO   (32)
#define MIPI_DSI_PROBE_IO   (33)
#define MIPI_TP_RST_IO      (45)
#define MIPI_LCD_RST_IO     (47)
#define MIPI_LCD_LED_IO     (48)
#define MIPI_SCCB_FRE       (100000)

#if BOARD_TYPE == BOARD_TYPE_FLY_LINE
#define MIPI_SCCB_SCL_IO             (22)
#define MIPI_SCCB_SDA_IO             (23)
#elif BOARD_TYPE == BOARD_TYPE_FIB
#define MIPI_SCCB_SCL_IO             (8)
#define MIPI_SCCB_SDA_IO             (7)
#elif BOARD_TYPE == BOARD_TYPE_SAMPLE
#define MIPI_SCCB_SCL_IO             (34)
#define MIPI_SCCB_SDA_IO             (31)
#endif

#define MIPI_DSI_DISP_HSIZE (MIPI_DSI_IMAGE_HSIZE)
#define MIPI_DSI_DISP_VSIZE (MIPI_DSI_IMAGE_VSIZE)
#if TEST_DISP_PATTERN
#define MIPI_CSI_DISP_HSIZE (0)
#define MIPI_CSI_DISP_VSIZE (0)
#else
#define MIPI_CSI_DISP_HSIZE (MIPI_CSI_IMAGE_HSIZE)
#define MIPI_CSI_DISP_VSIZE (MIPI_CSI_IMAGE_VSIZE)
#endif

int mipi_init(uint32_t dma_addr, uint32_t cache_addr);

#ifdef __cplusplus
}
#endif
