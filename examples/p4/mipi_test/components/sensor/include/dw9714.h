#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DW9714_METHOD_DIRECT 0x00
#define DW9714_METHOD_LINEAR 0X01
#define DW9714_METHOD_DUAL   0X02

#define DW9714_T_SRC 0X00
#define DW9714_MCLK  0X01

#define DW9714_STEP_0 0x00
#define DW9714_STEP_1 0X04
#define DW9714_STEP_2 0X08
#define DW9714_STEP_4 0X0C
 
#define DW9714_PERIOD_80US  0x00
#define DW9714_PERIOD_160US 0X01
#define DW9714_PERIOD_320US 0X02
#define DW9714_PERIOD_640US 0X03

int dw9714_get_current(float *current);

int dw9714_set_current(float current);

int dw9714_init(int method);

#ifdef __cplusplus
}
#endif