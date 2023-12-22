#pragma once

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct reginfo
{
    uint16_t reg;
    uint8_t val;
};

#define SEQUENCE_INIT        0x00
#define SEQUENCE_NORMAL      0x01

#define SEQUENCE_PROPERTY    0xFFFD
#define SEQUENCE_WAIT_MS     0xFFFE
#define SEQUENCE_END	     0xFFFF
int ov5640_init();

int ov5640_read(uint16_t addr,uint8_t *read_buf);

int ov5640_write(uint16_t addr,uint8_t data);

#ifdef __cplusplus
}
#endif