#pragma once

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "i2c.h"
#include "i3c.h"
#include "esp_log.h"

#define TEST_I3C (1)

#if TEST_I3C
#define sccb_write i3c_write

#define sccb_read i3c_read

#define sccb_write_mem i3c_write_mem

#define sccb_read_mem i3c_read_mem

#define sccb_write_mem16 i3c_write_mem16

#define sccb_read_mem16 i3c_read_mem16

#define sccb_write_reg i3c_write_reg

#define sccb_read_reg i3c_read_reg

#define sccb_write_reg16 i3c_write_reg16

#define sccb_read_reg16 i3c_read_reg16

#define sccb_prob i3c_prob

#define sccb_init i3c_init
#else
#define sccb_write i2c_write

#define sccb_read i2c_read

#define sccb_write_mem i2c_write_mem

#define sccb_read_mem i2c_read_mem

#define sccb_write_mem16 i2c_write_mem16

#define sccb_read_mem16 i2c_read_mem16

#define sccb_write_reg i2c_write_reg

#define sccb_read_reg i2c_read_reg

#define sccb_write_reg16 i2c_write_reg16

#define sccb_read_reg16 i2c_read_reg16

#define sccb_prob i2c_prob

#define sccb_init i2c_init
#endif


#ifdef __cplusplus
}
#endif