#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "isp_struct.h"
#include "mipi.h"

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

int isp_init();

#ifdef __cplusplus
}
#endif