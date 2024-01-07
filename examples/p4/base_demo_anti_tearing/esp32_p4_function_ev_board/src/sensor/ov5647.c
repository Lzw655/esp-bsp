/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "esp_check.h"
#include "driver/i2c.h"

#include "mipi_csi.h"

#include "bsp/esp-bsp.h"
#include "sensor.h"

#define I2C_MASTER_TIMEOUT_MS       (1000)
#define OV_ADDR                     (0x36)

static const char *TAG = "sensor.ov5647";

typedef struct {
    uint16_t reg;
    uint8_t val;
} reginfo_t;

#define SEQUENCE_END         0xFFFF
#define BIT(nr)              (1UL << (nr))

static reginfo_t ov5647_720p_init_data[] = {
    {0x3034, (TEST_CSI_COLOR_MODE == MIPI_CSI_RAW10_MODE) ? 0x1A : 0x18},
    {0x3035, 0x41},
    {0x3036, ((MIPI_IDI_CLOCK_RATE * 8 * 4 * 2) / 25000000)},
    {0x303c, 0x11},
    {0x3106, 0xf5},
    {0x3821, 0x01},
    {0x3820, 0x41},
    {0x3827, 0xec},
    {0x370c, 0x0f},
    {0x3612, 0x59},
    {0x3618, 0x00},
    {0x5000, 0x06},
    {0x5002, 0x41},
    {0x5003, 0x08},
    {0x5a00, 0x08},
    {0x3000, 0x00},
    {0x3001, 0x00},
    {0x3002, 0x00},
    {0x3016, 0x08},
    {0x3017, 0xe0},
    {0x3018, 0x44},
    {0x301c, 0xf8},
    {0x301d, 0xf0},
    {0x3a18, 0x00},
    {0x3a19, 0xf8},
    {0x3c01, 0x80},
    {0x3b07, 0x0c},
    //HTS line exposure time in # of pixels
    {0x380c, (1896 >> 8) & 0x1F},
    {0x380d, 1896 & 0xFF},
    //VTS frame exposure time in # lines
    {0x380e, (984 >> 8) & 0xFF},
    {0x380f, 984 & 0xFF},
    {0x3814, 0x31},
    {0x3815, 0x31},
    {0x3708, 0x64},
    {0x3709, 0x52},
    //[3:0]=0 X address start high byte
    {0x3800, (500 >> 8) & 0x0F},
    //[7:0]=0 X address start low byte
    {0x3801, 500 & 0xFF},
    //[2:0]=0 Y address start high byte
    {0x3802, (0 >> 8) & 0x07},
    //[7:0]=0 Y address start low byte
    {0x3803, 0 & 0xFF},
    //[3:0] X address end high byte
    {0x3804, ((2624 - 1) >> 8) & 0x0F},
    //[7:0] X address end low byte
    {0x3805, (2624 - 1) & 0xFF},
    //[2:0] Y address end high byte
    {0x3806, ((1954 - 1) >> 8) & 0x07},
    //[7:0] Y address end low byte
    {0x3807, (1954 - 1) & 0xFF},
    //[3:0] Output horizontal width high byte
    {0x3808, (MIPI_CSI_IMAGE_HSIZE >> 8) & 0x0F},
    //[7:0] Output horizontal width low byte
    {0x3809, MIPI_CSI_IMAGE_HSIZE & 0xFF},
    //[2:0] Output vertical height high byte
    {0x380a, (MIPI_CSI_IMAGE_VSIZE >> 8) & 0x7F},
    //[7:0] Output vertical height low byte
    {0x380b, MIPI_CSI_IMAGE_VSIZE & 0xFF},
    //[3:0]=0 timing hoffset high byte
    {0x3810, (8 >> 8) & 0x0F},
    //[7:0]=0 timing hoffset low byte
    {0x3811, 8 & 0xFF},
    //[2:0]=0 timing voffset high byte
    {0x3812, (0 >> 8) & 0x07},
    //[7:0]=0 timing voffset low byte
    {0x3813, 0 & 0xFF},
    {0x3630, 0x2e},
    {0x3632, 0xe2},
    {0x3633, 0x23},
    {0x3634, 0x44},
    {0x3636, 0x06},
    {0x3620, 0x64},
    {0x3621, 0xe0},
    {0x3600, 0x37},
    {0x3704, 0xa0},
    {0x3703, 0x5a},
    {0x3715, 0x78},
    {0x3717, 0x01},
    {0x3731, 0x02},
    {0x370b, 0x60},
    {0x3705, 0x1a},
    {0x3f05, 0x02},
    {0x3f06, 0x10},
    {0x3f01, 0x0a},
    {0x3a08, 0x01},
    {0x3a09, 0x27},
    {0x3a0a, 0x00},
    {0x3a0b, 0xf6},
    {0x3a0d, 0x04},
    {0x3a0e, 0x03},
    {0x3a0f, 0x58},
    {0x3a10, 0x50},
    {0x3a1b, 0x58},
    {0x3a1e, 0x50},
    {0x3a11, 0x60},
    {0x3a1f, 0x28},
    {0x4001, 0x02},
    {0x4004, 0x02},
    {0x4000, 0x09},
    {0x4837, (1000000000 / (MIPI_IDI_CLOCK_RATE / 2))},
    {0x4050, 0x6e},
    {0x4051, 0x8f},
#if (TEST_CSI_PATTERN)
    {0x503D, 0xA0},
#endif
    {SEQUENCE_END, 0x00}
};

static reginfo_t ov5647_1080p_init_data[] = {
    {0x3034, (TEST_CSI_COLOR_MODE == MIPI_CSI_RAW10_MODE) ? 0x1A : 0x18},
    {0x3035, 0x41},
    {0x3036, ((MIPI_IDI_CLOCK_RATE * 8 * 4 * 2) / 25000000)},
    {0x303c, 0x11},
    {0x3106, 0xf5},
    {0x3821, 0x07},
    {0x3820, 0x41},
    {0x3827, 0xec},
    {0x370c, 0x0f},
    {0x3612, 0x59},
    {0x3618, 0x00},
    {0x5000, 0x06},
    {0x5002, 0x41},
    {0x5003, 0x08},
    {0x5a00, 0x08},
    {0x3000, 0x00},
    {0x3001, 0x00},
    {0x3002, 0x00},
    {0x3016, 0x08},
    {0x3017, 0xe0},
    {0x3018, 0x44},
    {0x301c, 0xf8},
    {0x301d, 0xf0},
    {0x3a18, 0x00},
    {0x3a19, 0xf8},
    {0x3c01, 0x80},
    {0x3b07, 0x0c},
    //HTS line exposure time in # of pixels
    {0x380c, (2844 >> 8) & 0x1F},
    {0x380d, 2844 & 0xFF},
    //VTS frame exposure time in # lines
    {0x380e, (1968 >> 8) & 0xFF},
    {0x380f, 1968 & 0xFF},
    {0x3814, 0x31},
    {0x3815, 0x31},
    {0x3708, 0x64},
    {0x3709, 0x52},
    //[3:0]=0 X address start high byte
    {0x3800, (0 >> 8) & 0x0F},
    //[7:0]=0 X address start low byte
    {0x3801, 0 & 0xFF},
    //[2:0]=0 Y address start high byte
    {0x3802, (0 >> 8) & 0x07},
    //[7:0]=0 Y address start low byte
    {0x3803, 0 & 0xFF},
    //[3:0] X address end high byte
    {0x3804, ((2624 - 1) >> 8) & 0x0F},
    //[7:0] X address end low byte
    {0x3805, (2624 - 1) & 0xFF},
    //[2:0] Y address end high byte
    {0x3806, ((1954 - 1) >> 8) & 0x07},
    //[7:0] Y address end low byte
    {0x3807, (1954 - 1) & 0xFF},
    //[3:0] Output horizontal width high byte
    {0x3808, (MIPI_CSI_IMAGE_HSIZE >> 8) & 0x0F},
    //[7:0] Output horizontal width low byte
    {0x3809, MIPI_CSI_IMAGE_HSIZE & 0xFF},
    //[2:0] Output vertical height high byte
    {0x380a, (MIPI_CSI_IMAGE_VSIZE >> 8) & 0x7F},
    //[7:0] Output vertical height low byte
    {0x380b, MIPI_CSI_IMAGE_VSIZE & 0xFF},
    //[3:0]=0 timing hoffset high byte
    {0x3810, (8 >> 8) & 0x0F},
    //[7:0]=0 timing hoffset low byte
    {0x3811, 8 & 0xFF},
    //[2:0]=0 timing voffset high byte
    {0x3812, (0 >> 8) & 0x07},
    //[7:0]=0 timing voffset low byte
    {0x3813, 0 & 0xFF},
    {0x3630, 0x2e},
    {0x3632, 0xe2},
    {0x3633, 0x23},
    {0x3634, 0x44},
    {0x3636, 0x06},
    {0x3620, 0x64},
    {0x3621, 0xe0},
    {0x3600, 0x37},
    {0x3704, 0xa0},
    {0x3703, 0x5a},
    {0x3715, 0x78},
    {0x3717, 0x01},
    {0x3731, 0x02},
    {0x370b, 0x60},
    {0x3705, 0x1a},
    {0x3f05, 0x02},
    {0x3f06, 0x10},
    {0x3f01, 0x0a},
    {0x3a08, 0x01},
    {0x3a09, 0x27},
    {0x3a0a, 0x00},
    {0x3a0b, 0xf6},
    {0x3a0d, 0x04},
    {0x3a0e, 0x03},
    {0x3a0f, 0x58},
    {0x3a10, 0x50},
    {0x3a1b, 0x58},
    {0x3a1e, 0x50},
    {0x3a11, 0x60},
    {0x3a1f, 0x28},
    {0x4001, 0x02},
    {0x4004, 0x02},
    {0x4000, 0x09},
    {0x4837, (1000000000 / (MIPI_IDI_CLOCK_RATE / 2))},
    {0x4050, 0x6e},
    {0x4051, 0x8f},
#if (TEST_CSI_PATTERN)
    {0x503D, 0xA0},
#endif
    {SEQUENCE_END, 0x00}
};

esp_err_t ov5647_read(uint16_t addr, uint8_t *read_buf)
{
    // return sccb_read_reg16(OV_ADDR, addr, 1, read_buf);
    uint8_t addr_buf[2] = {addr >> 8, addr & 0xff};
    return i2c_master_write_read_device(BSP_I2C_NUM, OV_ADDR, addr_buf, sizeof(addr_buf), read_buf, 1, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
}

esp_err_t ov5647_write(uint16_t addr, uint8_t data)
{
    uint8_t write_buf[3] = {addr >> 8, addr & 0xff, data};
    // return sccb_write_reg16(OV_ADDR, addr, 1, data);
    return i2c_master_write_to_device(BSP_I2C_NUM, OV_ADDR, write_buf, sizeof(write_buf), pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
}

/* write a array of registers  */
static esp_err_t ov5647_write_array( reginfo_t *regarray)
{
    int i = 0;
    while (regarray[i].reg != SEQUENCE_END) {
        ESP_RETURN_ON_ERROR(ov5647_write(regarray[i].reg, regarray[i].val), TAG, "Write register failed");
        i++;
    }

    return ESP_OK;
}

esp_err_t sensor_ov5647_init()
{
    uint8_t sensor_id[2] ;

    printf("ADDR: 0x%x\n", OV_ADDR);

    ESP_RETURN_ON_ERROR(ov5647_read( 0x300a, &sensor_id[0]), TAG, "Read register failed");
    ESP_RETURN_ON_ERROR(ov5647_read( 0x300b, &sensor_id[1]), TAG, "Read register failed");

    printf("id, %x %x\r\n", sensor_id[0], sensor_id[1]);

    ESP_RETURN_ON_ERROR(ov5647_write(0x0100, 0x00), TAG, "Write register failed");
    ESP_RETURN_ON_ERROR(ov5647_write(0x0103, 0x01), TAG, "Write register failed");
    vTaskDelay(pdMS_TO_TICKS(10));
    // Ensure streaming off to make clock lane go into LP-11 state.
    ESP_RETURN_ON_ERROR(ov5647_write(0x4800, BIT(0)), TAG, "Write register failed");
#if TEST_CSI_720P
    ESP_RETURN_ON_ERROR(ov5647_write_array(ov5647_720p_init_data), TAG, "Write register failed");
#else
    ESP_RETURN_ON_ERROR(ov5647_write_array(ov5647_1080p_init_data), TAG, "Write register failed");
#endif

    ESP_RETURN_ON_ERROR(ov5647_write(0x4800, TEST_CSI_LINESYNC ? 0x14 : 0x04), TAG, "Write register failed");
#if TEST_CSI_AE
    ESP_RETURN_ON_ERROR(ov5647_write(0x3503, 0x3), TAG, "Write register failed");
#endif
#if 1
    // AF DW5714 XSD <-> set OV5647 GPIO0 to high
    ESP_RETURN_ON_ERROR(ov5647_write(0x3002, 0x01), TAG, "Write register failed");
    ESP_RETURN_ON_ERROR(ov5647_write(0x3010, 0x01), TAG, "Write register failed");
    ESP_RETURN_ON_ERROR(ov5647_write(0x300D, 0x01), TAG, "Write register failed");
    vTaskDelay(pdMS_TO_TICKS(10));
#endif
    ESP_RETURN_ON_ERROR(ov5647_write(0x0100, 0x01), TAG, "Write register failed");

    return ESP_OK;
}
