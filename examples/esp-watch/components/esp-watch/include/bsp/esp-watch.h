/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/gpio.h"
#include "lvgl.h"

#include "sdkconfig.h"

/**************************************************************************************************
 *  ESP-WATCH Pinout
 **************************************************************************************************/
/* I2C */
#define BSP_I2C_SCL             (GPIO_NUM_18)
#define BSP_I2C_SDA             (GPIO_NUM_8)

/* Display */
#define BSP_LCD_DATA0           (GPIO_NUM_6)
#define BSP_LCD_PCLK            (GPIO_NUM_7)
#define BSP_LCD_CS              (GPIO_NUM_5)
#define BSP_LCD_DC              (GPIO_NUM_4)
#define BSP_LCD_RST             (GPIO_NUM_48)
#define BSP_LCD_BACKLIGHT       (GPIO_NUM_45)
#define BSP_LCD_TOUCH_INT       (GPIO_NUM_3)

/**************************************************************************************************
 *
 * I2C Interface
 *
 * There are multiple devices connected to I2C peripheral:
 *  - Codec ES8311 (configuration only)
 *  - ADC ES7210 (configuration only)
 *  - LCD Touch controller
 *
 * After initialization of I2C, use `BSP_I2C_NUM` macro when creating I2C devices drivers ie.:
 * \code{.c}
 * es8311_handle_t es8311_dev = es8311_create(BSP_I2C_NUM, ES8311_ADDRRES_0);
 * \endcode
 *
 **************************************************************************************************/
#define BSP_I2C_NUM             (CONFIG_BSP_I2C_NUM)

/**
 * @brief Init I2C driver
 *
 * @return
 *      - ESP_OK:               On success
 *      - ESP_ERR_INVALID_ARG:  I2C parameter error
 *      - ESP_FAIL:             I2C driver installation error
 *
 */
esp_err_t bsp_i2c_init(void);

/**
 * @brief Deinit I2C driver and free its resources
 *
 * @return
 *      - ESP_OK:               On success
 *      - ESP_ERR_INVALID_ARG:  I2C parameter error
 *
 */
esp_err_t bsp_i2c_deinit(void);

/**************************************************************************************************
 *
 * LCD interface
 *
 * ESP-WATCH is shipped with 2.4inch ST7789 display controller.
 * It features 16-bit colors, 320x240 resolution and capacitive touch controller.
 *
 * LVGL is used as graphics library. LVGL is NOT thread safe, therefore the user must take LVGL mutex
 * by calling bsp_display_lock() before calling and LVGL API (lv_...) and then give the mutex with
 * bsp_display_unlock().
 *
 * Display's backlight must be enabled explicitly by calling bsp_display_backlight_on()
 **************************************************************************************************/
#define BSP_LCD_H_RES               (240)
#define BSP_LCD_V_RES               (280)
#define BSP_LCD_PIXEL_CLOCK_HZ      (80 * 1000 * 1000)
#define BSP_LCD_SPI_NUM             (SPI2_HOST)

/**
 * @brief Initialize display
 *
 * @note This function initializes display controller and starts LVGL handling task.
 * @note Users can get LCD panel handle from `user_data` in returned display.
 *
 * @return Pointer to LVGL display or NULL when error occured
 */
lv_disp_t *bsp_display_start(void);

/**
 * @brief Take LVGL mutex
 *
 * @param[in] timeout_ms: Timeout in [ms]. 0 will block indefinitely.
 *
 * @return
 *      - true:  Mutex was taken
 *      - false: Mutex was NOT taken
 */
bool bsp_display_lock(uint32_t timeout_ms);

/**
 * @brief Give LVGL mutex
 *
 */
void bsp_display_unlock(void);
