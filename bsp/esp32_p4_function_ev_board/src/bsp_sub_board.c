/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_check.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "esp_lcd_touch_gt911.h"

#include "sdkconfig.h"
#include "bsp_err_check.h"
#include "bsp/display.h"
#include "bsp/esp32_p4_function_ev_board.h"
#include "esp_lcd_panel_dsi.h"
#include "bsp/touch.h"

#define TEST_DISP_PATTERN     (0)

#define TEST_DSI_NT35516      (0)
#define TEST_DSI_ILI9806E     (0)
#define TEST_DSI_ILI9881C     (1)
#define TEST_DSI_ST7796       (0)
#define TEST_DSI_NT35532      (0)
#define TEST_DSI_ICN6211_800  (0)
#define TEST_DSI_ICN6211_1280 (0)

#if TEST_DSI_ILI9881C
#include "esp_lcd_ili9881.h"
#endif

static const char *TAG = "bsp_sub_board";
static bsp_display_trans_done_cb_t trans_done = NULL;

/**************************************************************************************************
 *
 * Display Panel Function
 *
 **************************************************************************************************/
IRAM_ATTR static bool dsi_lcd_on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_dsi_panel_event_data_t *edata, void *user_ctx)
{
    BaseType_t need_yield = pdFALSE;
    if (trans_done) {
        if (trans_done(panel)) {
            need_yield = pdTRUE;
        }
    }

    return (need_yield == pdTRUE);
}

esp_err_t bsp_display_new(const bsp_display_config_t *config, esp_lcd_panel_handle_t *ret_panel, esp_lcd_panel_io_handle_t *ret_io)
{
    esp_err_t ret = ESP_OK;

    ESP_LOGD(TAG, "Install LCD driver");
    const esp_lcd_dsi_panel_config_t panel_config = {
        .timings = {
            .dpi_clk_hz = MIPI_DPI_CLOCK_RATE,
            .dsi_line_hz = MIPI_DSI_LINE_RATE,
            .dsi_line_num = MIPI_DSI_LINE_NUM,
            .h_res = MIPI_DSI_IMAGE_HSIZE,
            .v_res = MIPI_DSI_IMAGE_VSIZE,
            .hsync_sync_active = MIPI_DSI_IMAGE_HSYNC,
            .hsync_back_porch = MIPI_DSI_IMAGE_HBP,
            .hsync_front_porch = MIPI_DSI_IMAGE_HFP,
            .vsync_sync_active = MIPI_DSI_IMAGE_VSYNC,
            .vsync_back_porch = MIPI_DSI_IMAGE_VBP,
            .vsync_front_porch = MIPI_DSI_IMAGE_VFP,
        },
        .bits_per_pixel = 16,
        .num_fbs = CONFIG_BSP_DISPLAY_LCD_BUFFER_NUMS,
        .flags = {
            .fb_in_psram = 1,
        },
    };
    BSP_ERROR_CHECK_RETURN_ERR(esp_lcd_new_dsi_panel(&panel_config, ret_panel));
#if CONFIG_BSP_DISPLAY_LVGL_AVOID_TEAR
    esp_lcd_dsi_panel_event_callbacks_t panel_event_callbacks = {
        .on_vsync = dsi_lcd_on_vsync_event,
    };
    esp_lcd_dsi_panel_register_event_callbacks(*ret_panel, &panel_event_callbacks, NULL);
#endif
    esp_lcd_panel_reset(*ret_panel);

#if !TEST_DSI_LOOPBACK
    esp_lcd_new_panel_ili9881(NULL, NULL, NULL);
#endif

    esp_lcd_panel_init(*ret_panel);

    // esp_lcd_panel_mirror(*ret_panel, true, true);

    return ret;

// err:
//     if (*ret_panel) {
//         esp_lcd_panel_del(*ret_panel);
//     }
//     return ret;
}

esp_err_t bsp_display_register_trans_done_callback(bsp_display_trans_done_cb_t callback)
{
#if CONFIG_LCD_RGB_ISR_IRAM_SAFE
    if (callback) {
        ESP_RETURN_ON_FALSE(esp_ptr_in_iram(callback), ESP_ERR_INVALID_ARG, TAG, "Callback not in IRAM");
    }
#endif
    trans_done = callback;

    return ESP_OK;
}

/**************************************************************************************************
 *
 * Touch Panel Function
 *
 **************************************************************************************************/
esp_err_t bsp_touch_new(const bsp_touch_config_t *config, esp_lcd_touch_handle_t *ret_touch)
{
    /* Initilize I2C */
    BSP_ERROR_CHECK_RETURN_ERR(bsp_i2c_init());

    /* Initialize touch */
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = BSP_LCD_H_RES,
        .y_max = BSP_LCD_V_RES,
        .rst_gpio_num = GPIO_NUM_NC,
        .int_gpio_num = GPIO_NUM_NC,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();

    BSP_ERROR_CHECK_RETURN_ERR(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)BSP_I2C_NUM, &tp_io_config, &tp_io_handle));
    BSP_ERROR_CHECK_RETURN_ERR(esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, ret_touch));

    return ESP_OK;
}

/**************************************************************************************************
 *
 * Other Function
 *
 **************************************************************************************************/
uint16_t bsp_display_get_h_res(void)
{
    return MIPI_DSI_IMAGE_HSIZE;
}

uint16_t bsp_display_get_v_res(void)
{
    return MIPI_DSI_IMAGE_VSIZE;
}
