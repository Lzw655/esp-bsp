
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "gt911.h"
#include "sccb.h"

typedef struct
{
	uint8_t TouchCount;
 
	uint8_t Touchkeytrackid[CT_MAX_TOUCH];
	uint16_t X[CT_MAX_TOUCH];
	uint16_t Y[CT_MAX_TOUCH];
	uint16_t S[CT_MAX_TOUCH];
} gt911_dev_t;

static gt911_dev_t gt911_dev;

static esp_err_t gt911_read(uint16_t start_addr, uint8_t read_num, uint8_t *data_buf)
{
    return sccb_read_mem16(GT911_I2C_ADDR, start_addr, read_num, data_buf);
}

static esp_err_t gt911_write(uint16_t start_addr, uint8_t write_num, uint8_t *data_buf)
{
    return sccb_write_mem16(GT911_I2C_ADDR, start_addr, write_num, data_buf);
}


void gt911_read_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    if (gt911_read(reg, len, buf)) {
        printf("Read failed\n");
    };
}

void gt911_write_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    if (gt911_write(reg, len, buf)) {
        printf("Write REG %0X failed\n", reg);
    }
}

void gt911_touch_init(void)
{
    uint8_t buf[4];

    gt911_read_reg(GT911_PRODUCT_ID_REG, (uint8_t *)&buf, 3);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    gt911_read_reg(GT911_CONFIG_REG, (uint8_t *)&buf[3], 1);

    printf("TouchPad_ID:%c,%c,%c TouchPad_Config_Version:%2x\n", buf[0], buf[1], buf[2], buf[3]);

    gt911_read_reg(GT911_FIRMWARE_VERSION_REG, (uint8_t *)&buf, 2);
    printf("FirmwareVersion:%2x\n", (((uint16_t)buf[1] << 8) + buf[0]));
}

void gt911_scan(void)
{
    uint8_t buf[41]; // 5*8bit + 1 byte
    uint8_t Clearbuf = 0;

    gt911_read_reg(GT911_READ_XY_REG, buf, 1);

    if ((buf[0] & 0x80) == 0x00) {
        gt911_write_reg(GT911_READ_XY_REG, (uint8_t *)&Clearbuf, 1);
    } else {
        gt911_dev.TouchCount = buf[0] & 0x0f;

        if ((gt911_dev.TouchCount > 5) || (gt911_dev.TouchCount == 0)) {
            gt911_write_reg(GT911_READ_XY_REG, (uint8_t *)&Clearbuf, 1);
            return ;
        }

        gt911_read_reg(GT911_READ_XY_REG + 1, &buf[1], gt911_dev.TouchCount * 8);
        gt911_write_reg(GT911_READ_XY_REG, (uint8_t *)&Clearbuf, 1);

        for (uint8_t i = 0; i < gt911_dev.TouchCount; i++) {
            gt911_dev.Touchkeytrackid[i] = buf[1 + (8 * i)];
            gt911_dev.X[i] = ((uint16_t)buf[3 + (8 * i)] << 8) + buf[2 + (8 * i)];
            gt911_dev.Y[i] = ((uint16_t)buf[5 + (8 * i)] << 8) + buf[4 + (8 * i)];
            gt911_dev.S[i] = ((uint16_t)buf[7 + (8 * i)] << 8) + buf[6 + (8 * i)]; 


            if (gt911_dev.Y[i] < 20) {
                gt911_dev.Y[i] = 20;
            }

            if (gt911_dev.Y[i] > GT911_MAX_HEIGHT - 20) {
                gt911_dev.Y[i] = GT911_MAX_HEIGHT - 20;
            }

            if (gt911_dev.X[i] < 20) {
                gt911_dev.X[i] = 20;
            }

            if (gt911_dev.X[i] > GT911_MAX_WIDTH - 20) {
                gt911_dev.X[i] = GT911_MAX_WIDTH - 20;
            }

            printf("Now touch %d X:%d Y:%d S:%d\n", i, gt911_dev.X[i], gt911_dev.Y[i], gt911_dev.S[i]);
        }

    }

}

int gt911_pos_read(uint16_t *xpos, uint16_t *ypos)
{
    uint8_t buf[41]; // 5*8bit + 1 byte
    uint8_t Clearbuf = 0;

    gt911_read_reg(GT911_READ_XY_REG, buf, 1);

    if ((buf[0] & 0x80) == 0x00) {
        gt911_write_reg(GT911_READ_XY_REG, (uint8_t *)&Clearbuf, 1);
    } else {
        gt911_dev.TouchCount = buf[0] & 0x0f;

        if ((gt911_dev.TouchCount > 5) || (gt911_dev.TouchCount == 0)) {
            gt911_write_reg(GT911_READ_XY_REG, (uint8_t *)&Clearbuf, 1);
            return -1;
        }

        gt911_read_reg(GT911_READ_XY_REG + 1, &buf[1], gt911_dev.TouchCount * 8);
        gt911_write_reg(GT911_READ_XY_REG, (uint8_t *)&Clearbuf, 1);

        for (uint8_t i = 0; i < gt911_dev.TouchCount; i++) {
            gt911_dev.Touchkeytrackid[i] = buf[1 + (8 * i)];
            gt911_dev.X[i] = ((uint16_t)buf[3 + (8 * i)] << 8) + buf[2 + (8 * i)];
            gt911_dev.Y[i] = ((uint16_t)buf[5 + (8 * i)] << 8) + buf[4 + (8 * i)];
            gt911_dev.S[i] = ((uint16_t)buf[7 + (8 * i)] << 8) + buf[6 + (8 * i)]; 

            if (gt911_dev.Y[i] > GT911_MAX_HEIGHT - 20) {
                gt911_dev.Y[i] = GT911_MAX_HEIGHT - 20;
            }

            if (gt911_dev.X[i] > GT911_MAX_WIDTH - 20) {
                gt911_dev.X[i] = GT911_MAX_WIDTH - 20;
            }
        }

        if (gt911_dev.TouchCount >= 1) {
            *xpos = gt911_dev.X[0];
            *ypos = gt911_dev.Y[0];
            return 1;
        }

    }

    return 0;
}
