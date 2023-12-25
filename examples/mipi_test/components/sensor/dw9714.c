#include "dw9714.h"
#include "csi.h"
#include "sccb.h"

#define DW9714_ADDR           0xC
#define DW9714_RETRY          5
#define DW9714_TEST           0

typedef union {
    struct {
        uint16_t s    : 4;
        uint16_t d    : 10;
        uint16_t flag : 1;
        uint16_t pd   : 1;
    };
    struct {
        uint16_t byte2 : 8;
        uint16_t byte1 : 8;
    };
    uint16_t val;
} dw9714_reg_t;

#define PROTECTION_OFF_REG 0xECA3
#define PROTECTION_ON_REG  0xDC51

static void delay_us(uint32_t t)
{
#if TEST_CSI_FPGA
    for (uint32_t tu = 0; tu < t; tu++);
#else
    ets_delay_us(t);
#endif
}

static int dw9714_get_reg(dw9714_reg_t *reg)
{
    int ret = -1;
    uint8_t data[2] = {0};

    for (int x = 0; x < DW9714_RETRY; x++) {
        ret = sccb_read(DW9714_ADDR, 2, data);
        if (ret == 0) {
            break;
        }
    }
    
    reg->byte1 = data[0];
    reg->byte2 = data[1];

    return ret;
}


static int dw9714_set_reg(dw9714_reg_t reg)
{
    int ret = -1;
    uint8_t data[2] = {0};
    data[0] = reg.byte1;
    data[1] = reg.byte2;

    for (int x = 0; x < DW9714_RETRY; x++) {
        ret = sccb_write(DW9714_ADDR, 2, data);
        if (ret == 0) {
            break;
        }
    }

    return ret;
}

int dw9714_get_current(float *current)
{
    int ret = -1;
    dw9714_reg_t reg = {0};
    ret = dw9714_get_reg(&reg);
    *current = (float)(((float)reg.d / 1023.0) * 120.0);

    return ret;
}

int dw9714_set_current(float current) 
{
    if (current > 120) {
        return -1;
    }
    dw9714_reg_t reg = {0};
    reg.d = (uint16_t)((current / 120.0) * 1023.0);

    return dw9714_set_reg(reg);
}

int dw9714_set_current_linear(float current, uint8_t step, uint8_t period) 
{
    if (current > 120) {
        return -1;
    }
    dw9714_reg_t reg = {0};
    // Data[9:0], S[3:0]
    reg.d = (uint16_t)((current / 120.0) * 1023.0);
    reg.s = step | period;

    return dw9714_set_reg(reg);
}

int dw9714_init(int method)
{
    dw9714_reg_t reg = {0};
    delay_us(1000);
    if (method == DW9714_METHOD_DUAL) {
        // Protection OFF
        reg.val = PROTECTION_OFF_REG;
        dw9714_set_reg(reg);
        // DLC/MCLK setting
        reg.byte1 = 0xA1;
        reg.byte2 = (0x11 << 2) | DW9714_MCLK; 
        dw9714_set_reg(reg);
        // T_SRC setting
        reg.byte1 = 0xF2;
        reg.byte2 = ((DW9714_T_SRC & 0x1F) << 3); 
        dw9714_set_reg(reg);
        // Protection ON
        reg.val = PROTECTION_ON_REG;
        dw9714_set_reg(reg);
    } else if (method == DW9714_METHOD_LINEAR) {
        // Protection OFF
        reg.val = PROTECTION_OFF_REG;
        dw9714_set_reg(reg);
        // T_SRC setting
        reg.byte1 = 0xF2;
        reg.byte2 = ((DW9714_T_SRC & 0x1F) << 3); 
        dw9714_set_reg(reg);
        // Protection ON
        reg.val = PROTECTION_ON_REG;
        dw9714_set_reg(reg);
    }

    float current;
    dw9714_get_current(&current);
    printf("init current: %d.%d\n", (int)current, (int)(current * 1000));
    dw9714_set_current(60);

#if DW9714_TEST
    while (1) {
        for (int x = 0; x < 120 ; x+=5) {
            dw9714_get_current(&current);
            printf("current: %d.%d\n", (int)current, (int)(current * 1000));
            dw9714_set_current(x);
        }
    }
#endif

    return 0;
}