/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "esp_log.h"
#include "esp_lcd_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "mipi_dsi.h"
#include "bsp_lcd.h"
#include "bsp/display.h"

static const char *TAG = "bsp_display";

esp_err_t bsp_display_new(const bsp_display_config_t *config, esp_lcd_panel_handle_t *ret_panel, esp_lcd_panel_io_handle_t *ret_io)
{
    mipi_dsi_phy_host_init();
    bsp_lcd_init();
    mipi_dsi_bridge_init();
    bsp_lcd_start();
    mipi_dsi_bridge_start();

    return ESP_OK;
}
