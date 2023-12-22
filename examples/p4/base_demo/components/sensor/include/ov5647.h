#pragma once

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int ov5647_read(uint16_t addr,uint8_t *read_buf);

int ov5647_write(uint16_t addr,uint8_t data);

int ov5647_init();

#ifdef __cplusplus
}
#endif