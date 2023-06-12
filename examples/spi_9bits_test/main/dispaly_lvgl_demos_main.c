/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: 330-1.0
 */

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_lcd_panel_io_interface.h"

#include "esp_io_expander_tca9554.h"
#include "esp_lcd_panel_io_9bits_spi.h"

static char *TAG = "app_main";

void app_main(void)
{
    const i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 8,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_io_num = 18,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = 400000,
    };
    ESP_ERROR_CHECK(i2c_param_config(1, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(1, i2c_conf.mode, 0, 0, 0));

    esp_io_expander_handle_t io_expander = NULL;
    ESP_ERROR_CHECK(esp_io_expander_new_i2c_tca9554(1, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, &io_expander));

    esp_lcd_panel_io_9bits_spi_config_t config = {
        .cs_io_type = IO_TYPE_EXPANDER,
        .cs_gpio_num = IO_EXPANDER_PIN_NUM_1,
        .scl_io_type = IO_TYPE_EXPANDER,
        .scl_gpio_num = IO_EXPANDER_PIN_NUM_2,
        .sda_io_type = IO_TYPE_EXPANDER,
        .sda_gpio_num = IO_EXPANDER_PIN_NUM_3,
        .io_expander = io_expander,
        .expect_clk_speed = PANEL_IO_9BITS_SPI_CLK_MAX,
        .spi_mode = 0,
        .lcd_cmd_bytes = 4,
        .lcd_param_bytes = 4,
        .flags = {
            .dc_zero_on_data = 0,
            .lsb_first = 0,
            .cs_high_active = 0,
            .del_keep_cs_inactive = 0,
        },
    };
    esp_lcd_panel_io_handle_t io = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_9bits_spi(&config, &io));

    esp_io_expander_print_state(io_expander);

    uint8_t data_8 = 0x11;
    uint8_t param_8[] = {0x11, 0x22};

    uint16_t param_16[] = {0x1122, 0x3344};
    uint16_t data_16 = 0x1122;

    uint8_t param_24[] = {0x33, 0x22, 0x11, 0x66, 0x55, 0x44};
    uint32_t data_24 = 0x112233;

    uint32_t param_32[] = {0x11223344, 0x55667788};
    uint32_t data_32 = 0x11223344;

    int data = 0;
    void *param = NULL;
    if (config.lcd_param_bytes == 1) {
        param = param_8;
    } else if (config.lcd_param_bytes == 2) {
        param = param_16;
    } else if (config.lcd_param_bytes == 3) {
        param = param_24;
    } else if (config.lcd_param_bytes == 4) {
        param = param_32;
    }
    if (config.lcd_cmd_bytes == 1) {
        data = data_8;
    } else if (config.lcd_cmd_bytes == 2) {
        data = data_16;
    } else if (config.lcd_cmd_bytes == 3) {
        data = data_24;
    } else if (config.lcd_cmd_bytes == 4) {
        data = data_32;
    }

    while (1) {
        esp_lcd_panel_io_tx_param(io, data, param, config.lcd_param_bytes * 2);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
