#pragma once
#include "esp32p4/rom/ets_sys.h"
#include "mipi_dsi_host_struct.h"
#include "mipi_dsi_bridge_struct.h"

#define MIPI_DSI_RGB888_MODE (0)
#define MIPI_DSI_RGB666_MODE (1)
#define MIPI_DSI_RGB565_MODE (2)
#define MIPI_DSI_YUV422_MODE (3)

#define TEST_DSI_FPGA         (0)
#define TEST_DSI_NT35516      (0)
#define TEST_DSI_ILI9806E     (0)
#define TEST_DSI_ILI9881C     (1)
#define TEST_DSI_ST7796       (0)
#define TEST_DSI_NT35532      (0)
#define TEST_DSI_ICN6211_800  (0)
#define TEST_DSI_ICN6211_1280 (0)
#define TEST_DSI_ESCAPE_MODE  (0)
#define TEST_DSI_PATTERN      (0)
#define TEST_DSI_LOOPBACK     (0)
#define TEST_DSI_COLOR_MODE  (MIPI_DSI_RGB565_MODE)
#if (TEST_DSI_COLOR_MODE==MIPI_DSI_RGB888_MODE)
#define TEST_DSI_COLOR_WIDTH (24)
#elif (TEST_DSI_COLOR_MODE==MIPI_DSI_RGB666_MODE)
#define TEST_DSI_COLOR_WIDTH (18)
#elif (TEST_DSI_COLOR_MODE==MIPI_DSI_RGB565_MODE)
#define TEST_DSI_COLOR_WIDTH (16)
#endif
#define TEST_DSI_TR_WIDTH    (64)

#define  MIPI_DSI_RGB888_RED     (0xFF0000)
#define  MIPI_DSI_RGB888_GREEN   (0x00FF00)
#define  MIPI_DSI_RGB888_BLUE    (0x0000FF)
#define  MIPI_DSI_RGB888_YELLOW  (0xFFFF00)
#define  MIPI_DSI_RGB888_CYAN    (0x00FFFF)
#define  MIPI_DSI_RGB888_MAGENTA (0xFF00FF)

#define  MIPI_DSI_RGB565_RED     (0xF800)
#define  MIPI_DSI_RGB565_GREEN   (0x07E0)
#define  MIPI_DSI_RGB565_BLUE    (0x001F)
#define  MIPI_DSI_RGB565_YELLOW  (0xFFE0)
#define  MIPI_DSI_RGB565_CYAN    (0x07FF)
#define  MIPI_DSI_RGB565_MAGENTA (0xF81F)

#if TEST_DSI_NT35516
#define  MIPI_DSI_LINE_RATE  (800000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (60000000)

#define  MIPI_DSI_IMAGE_HSIZE           540
#define  MIPI_DSI_IMAGE_VSIZE           960

#if (TEST_DSI_COLOR_MODE==MIPI_DSI_RGB888_MODE)
#define  MIPI_DSI_IMAGE_HSYNC           200
#define  MIPI_DSI_IMAGE_HBP             1000
#define  MIPI_DSI_IMAGE_HFP             1000
#define  MIPI_DSI_IMAGE_VSYNC           16
#define  MIPI_DSI_IMAGE_VBP             32
#define  MIPI_DSI_IMAGE_VFP             32 + (960 - MIPI_DSI_IMAGE_VSIZE)
#else
#if 1
#define  MIPI_DSI_IMAGE_HSYNC           80
#define  MIPI_DSI_IMAGE_HBP             200
#define  MIPI_DSI_IMAGE_HFP             200
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             8
#define  MIPI_DSI_IMAGE_VFP             8 + (960 - MIPI_DSI_IMAGE_VSIZE)
#else
#define  MIPI_DSI_IMAGE_HSYNC           80
#define  MIPI_DSI_IMAGE_HBP             1000
#define  MIPI_DSI_IMAGE_HFP             1000
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             8
#define  MIPI_DSI_IMAGE_VFP             8 + (960 - MIPI_DSI_IMAGE_VSIZE)
#endif
#endif

#endif


#if TEST_DSI_ILI9806E
#define  MIPI_DSI_LINE_RATE  (1000000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (120000000)

#define  MIPI_DSI_IMAGE_HSIZE           480
#define  MIPI_DSI_IMAGE_VSIZE           864

#define  MIPI_DSI_IMAGE_HSYNC           1
#define  MIPI_DSI_IMAGE_HBP             2
#define  MIPI_DSI_IMAGE_HFP             2
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             8
#define  MIPI_DSI_IMAGE_VFP             8
#endif

#if TEST_DSI_ILI9881C
#define  MIPI_DSI_LINE_RATE  (1000000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (40000000)

#define  MIPI_DSI_IMAGE_HSIZE           800
#define  MIPI_DSI_IMAGE_VSIZE           1280

#define  MIPI_DSI_IMAGE_HSYNC           40
#define  MIPI_DSI_IMAGE_HBP             140
#define  MIPI_DSI_IMAGE_HFP             40
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             16
#define  MIPI_DSI_IMAGE_VFP             16
#endif

#if  TEST_DSI_ST7796
#define  MIPI_DSI_LINE_RATE  (400000000)
#define  MIPI_DPI_LINE_NUM   (1)
#define  MIPI_DPI_CLOCK_RATE (20000000)

#define  MIPI_DSI_IMAGE_HSIZE           320
#define  MIPI_DSI_IMAGE_VSIZE           480

#define  MIPI_DSI_IMAGE_HSYNC           100
#define  MIPI_DSI_IMAGE_HBP             120
#define  MIPI_DSI_IMAGE_HFP             120
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             8
#define  MIPI_DSI_IMAGE_VFP             8 + (480 - MIPI_DSI_IMAGE_VSIZE)
#endif

#if TEST_DSI_NT35532
#define  MIPI_DSI_LINE_RATE  (1000000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (64000000)

#define  MIPI_DSI_IMAGE_HSIZE           1080
#define  MIPI_DSI_IMAGE_VSIZE           1920

#define  MIPI_DSI_IMAGE_HSYNC           400
#define  MIPI_DSI_IMAGE_HBP             400
#define  MIPI_DSI_IMAGE_HFP             400
#define  MIPI_DSI_IMAGE_VSYNC           2
#define  MIPI_DSI_IMAGE_VBP             2
#define  MIPI_DSI_IMAGE_VFP             4 + (1920 - MIPI_DSI_IMAGE_VSIZE)
#endif

#if TEST_DSI_ICN6211_800
#define  MIPI_DSI_LINE_RATE  (800000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (32000000)

#define  MIPI_DSI_IMAGE_HSIZE           800
#define  MIPI_DSI_IMAGE_VSIZE           480

// HSYNC 1.407us HT 33.29us, 30.04KHz
#define  MIPI_DSI_IMAGE_HSYNC           80
#define  MIPI_DSI_IMAGE_HBP             100
#define  MIPI_DSI_IMAGE_HFP             100

// VSYNC 100us, VT 18ms, 56.88Hz
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             4
#define  MIPI_DSI_IMAGE_VFP             8
#endif

#if TEST_DSI_ICN6211_1280
#define  MIPI_DSI_LINE_RATE  (800000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (80000000)

#define  MIPI_DSI_IMAGE_HSIZE           1280
#define  MIPI_DSI_IMAGE_VSIZE           800

// HSYNC 1.407us HT 33.29us, 30.04KHz
#define  MIPI_DSI_IMAGE_HSYNC           114
#define  MIPI_DSI_IMAGE_HBP             120
#define  MIPI_DSI_IMAGE_HFP             120

// VSYNC 100us, VT 18ms, 56.88Hz
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             4
#define  MIPI_DSI_IMAGE_VFP             8
#endif

#define  MIPI_DSI_BYTE_RATE  (MIPI_DSI_LINE_RATE / 8)

#define  MIPI_DPI_TIME_FACTOR ((float)MIPI_DSI_BYTE_RATE / MIPI_DPI_CLOCK_RATE)

/*
 * @brief  : Initialization of MIPI-DSI, including Bridge,
 *           Host-Controller and D-PHY.
 * @params : void
 * @ret    : int
 **/
int mipi_dsi_initialization(void);
