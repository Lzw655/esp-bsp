/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "mipi_dsi.h"

#include "esp_lcd_ili9881.h"

static const char *TAG = "ili9881";

esp_err_t esp_lcd_new_panel_ili9881(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config,
                                    esp_lcd_panel_handle_t *ret_panel)
{

    // RDDID: Read Display ID(DAH)
    // This read byte returns 24-bit display identification information.
    // The 1 parameter (ID1): the module’s manufacture ID.
    // The 2 parameter (ID2): the module/driver version ID.
    // The 3 parameter (ID3): the module/driver ID
    uint8_t ID1, ID2, ID3;
    mipi_dcs_read_cmd(0xDA, 3, &ID1, &ID2, &ID3);
    ESP_LOGI(TAG, "ID1: 0x%x, ID2: 0x%x, ID3: 0x%x", ID1, ID2, ID3);

    mipi_dcs_write_cmd(0xFF, 0x5, 0xFF, 0x98, 0x06, 0x04, 0x00); // Change to Page 0
    mipi_dcs_write_cmd(0x20, 0x1, 0x00);

    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x01); // Change to Page 1
    mipi_dcs_write_cmd(0x08, 1, 0x10);                         // output SDA
    mipi_dcs_write_cmd(0x21, 1, 0x01);                         // DE = 1 Active
    mipi_dcs_write_cmd(0x30, 1, 0x02);                         // 480 X 800
    mipi_dcs_write_cmd(0x31, 1, 0x02);                         // 2-Dot Inversion
    mipi_dcs_write_cmd(0x60, 1, 0x07);                         // SDTI
    mipi_dcs_write_cmd(0x61, 1, 0x06);                         // CRTI
    mipi_dcs_write_cmd(0x62, 1, 0x06);                         // EQTI
    mipi_dcs_write_cmd(0x63, 1, 0x04);                         // PCTI
    mipi_dcs_write_cmd(0x40, 1, 0x14);                         // BT  +2.5/-2.5 pump for DDVDH-L
    mipi_dcs_write_cmd(0x41, 1, 0x55);                         // DVDDH DVDDL clamp
    mipi_dcs_write_cmd(0x42, 1, 0x11);                         // VGH/VGL
    mipi_dcs_write_cmd(0x43, 1, 0x09);                         // VGH clamp
    mipi_dcs_write_cmd(0x44, 1, 0x0C);                         // VGL clamp
    mipi_dcs_write_cmd(0x45, 1, 0x14);                         // VGL_REG  -10V
    mipi_dcs_write_cmd(0x50, 1, 0x50);                         // VGMP
    mipi_dcs_write_cmd(0x51, 1, 0x50);                         // VGMN
    mipi_dcs_write_cmd(0x52, 1, 0x00);                         //Flicker
    mipi_dcs_write_cmd(0x53, 1, 0x47);                         //Flicker4F

    //++++++++++++++++++ Gamma Setting ++++++++++++++++++//

    mipi_dcs_write_cmd(0xA0, 1, 0x00);  // Gamma 0 /255
    mipi_dcs_write_cmd(0xA1, 1, 0x09);  // Gamma 4 /251
    mipi_dcs_write_cmd(0xA2, 1, 0x0C);  // Gamma 8 /247
    mipi_dcs_write_cmd(0xA3, 1, 0x0F);  // Gamma 16    /239
    mipi_dcs_write_cmd(0xA4, 1, 0x06);  // Gamma 24 /231
    mipi_dcs_write_cmd(0xA5, 1, 0x09);  // Gamma 52 / 203
    mipi_dcs_write_cmd(0xA6, 1, 0x07);  // Gamma 80 / 175
    mipi_dcs_write_cmd(0xA7, 1, 0x016); // Gamma 108 /147
    mipi_dcs_write_cmd(0xA8, 1, 0x06);  // Gamma 147 /108
    mipi_dcs_write_cmd(0xA9, 1, 0x09);  // Gamma 175 / 80
    mipi_dcs_write_cmd(0xAA, 1, 0x11);  // Gamma 203 / 52
    mipi_dcs_write_cmd(0xAB, 1, 0x06);  // Gamma 231 / 24
    mipi_dcs_write_cmd(0xAC, 1, 0x0E);  // Gamma 239 / 16
    mipi_dcs_write_cmd(0xAD, 1, 0x19);  // Gamma 247 / 8
    mipi_dcs_write_cmd(0xAE, 1, 0x0C);  // Gamma 251 / 4
    mipi_dcs_write_cmd(0xAF, 1, 0x00);  // Gamma 255 / 0

    ///==============Nagitive
    mipi_dcs_write_cmd(0xC0, 1, 0x00); // Gamma 0 /255
    mipi_dcs_write_cmd(0xC1, 1, 0x09); // Gamma 4  /251
    mipi_dcs_write_cmd(0xC2, 1, 0x0C); // Gamma 8  /247
    mipi_dcs_write_cmd(0xC3, 1, 0x0F); // Gamma 16  /239
    mipi_dcs_write_cmd(0xC4, 1, 0x06); // Gamma 24  /231
    mipi_dcs_write_cmd(0xC5, 1, 0x09); // Gamma 52  /203
    mipi_dcs_write_cmd(0xC6, 1, 0x07); // Gamma 80  /175
    mipi_dcs_write_cmd(0xC7, 1, 0x16); // Gamma 108  /147
    mipi_dcs_write_cmd(0xC8, 1, 0x06); // Gamma 147  /108
    mipi_dcs_write_cmd(0xC9, 1, 0x09); // Gamma 175  /80
    mipi_dcs_write_cmd(0xCA, 1, 0x11); // Gamma 203  /52
    mipi_dcs_write_cmd(0xCB, 1, 0x06); // Gamma 231  /24
    mipi_dcs_write_cmd(0xCC, 1, 0x0E); // Gamma 239  /16
    mipi_dcs_write_cmd(0xCD, 1, 0x19); // Gamma 247  /8
    mipi_dcs_write_cmd(0xCE, 1, 0x0C); // Gamma 251  /4
    mipi_dcs_write_cmd(0xCF, 1, 0x00); // Gamma 255

    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x07); // Change to Page 7

    mipi_dcs_write_cmd(0x17, 1, 0x32);
    vTaskDelay(pdMS_TO_TICKS(10));
    mipi_dcs_write_cmd(0x06, 1, 0x13);
    vTaskDelay(pdMS_TO_TICKS(10));

    vTaskDelay(pdMS_TO_TICKS(10));
    mipi_dcs_write_cmd(0x02, 1, 0x77);
    vTaskDelay(pdMS_TO_TICKS(10));

    mipi_dcs_write_cmd(0x18, 1, 0x1D);
    vTaskDelay(pdMS_TO_TICKS(10));
    mipi_dcs_write_cmd(0xE1, 1, 0x79);
    vTaskDelay(pdMS_TO_TICKS(10));

    //+++++++++++++++++++++++++++++++++++++++++++++++++++//

    //****************************************************************************//
    //****************************** Page 6 Command ******************************//
    //****************************************************************************//
    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x06); // Change to Page 6
    mipi_dcs_write_cmd(0x00, 1, 0xA0);
    mipi_dcs_write_cmd(0x01, 1, 0x05);
    mipi_dcs_write_cmd(0x02, 1, 0x00);
    mipi_dcs_write_cmd(0x03, 1, 0x00);
    mipi_dcs_write_cmd(0x04, 1, 0x01);
    mipi_dcs_write_cmd(0x05, 1, 0x01);
    mipi_dcs_write_cmd(0x06, 1, 0x88);
    mipi_dcs_write_cmd(0x07, 1, 0x04);
    mipi_dcs_write_cmd(0x08, 1, 0x01);
    mipi_dcs_write_cmd(0x09, 1, 0x90);
    mipi_dcs_write_cmd(0x0A, 1, 0x04);
    mipi_dcs_write_cmd(0x0B, 1, 0x01);
    mipi_dcs_write_cmd(0x0C, 1, 0x01);
    mipi_dcs_write_cmd(0x0D, 1, 0x01);
    mipi_dcs_write_cmd(0x0E, 1, 0x00);
    mipi_dcs_write_cmd(0x0F, 1, 0x00);
    mipi_dcs_write_cmd(0x10, 1, 0x55);
    mipi_dcs_write_cmd(0x11, 1, 0x50);
    mipi_dcs_write_cmd(0x12, 1, 0x01);
    mipi_dcs_write_cmd(0x13, 1, 0x85);
    mipi_dcs_write_cmd(0x14, 1, 0x85);
    mipi_dcs_write_cmd(0x15, 1, 0xC0);
    mipi_dcs_write_cmd(0x16, 1, 0x0B);
    mipi_dcs_write_cmd(0x17, 1, 0x00);
    mipi_dcs_write_cmd(0x18, 1, 0x00);
    mipi_dcs_write_cmd(0x19, 1, 0x00);
    mipi_dcs_write_cmd(0x1A, 1, 0x00);
    mipi_dcs_write_cmd(0x1B, 1, 0x00);
    mipi_dcs_write_cmd(0x1C, 1, 0x00);
    mipi_dcs_write_cmd(0x1D, 1, 0x00);

    mipi_dcs_write_cmd(0x20, 1, 0x01);
    mipi_dcs_write_cmd(0x21, 1, 0x23);
    mipi_dcs_write_cmd(0x22, 1, 0x45);
    mipi_dcs_write_cmd(0x23, 1, 0x67);
    mipi_dcs_write_cmd(0x24, 1, 0x01);
    mipi_dcs_write_cmd(0x25, 1, 0x23);
    mipi_dcs_write_cmd(0x26, 1, 0x45);
    mipi_dcs_write_cmd(0x27, 1, 0x67);

    mipi_dcs_write_cmd(0x30, 1, 0x02);
    mipi_dcs_write_cmd(0x31, 1, 0x22);
    mipi_dcs_write_cmd(0x32, 1, 0x11);
    mipi_dcs_write_cmd(0x33, 1, 0xAA);
    mipi_dcs_write_cmd(0x34, 1, 0xBB);
    mipi_dcs_write_cmd(0x35, 1, 0x66);
    mipi_dcs_write_cmd(0x36, 1, 0x00);
    mipi_dcs_write_cmd(0x37, 1, 0x22);
    mipi_dcs_write_cmd(0x38, 1, 0x22);
    mipi_dcs_write_cmd(0x39, 1, 0x22);
    mipi_dcs_write_cmd(0x3A, 1, 0x22);
    mipi_dcs_write_cmd(0x3B, 1, 0x22);
    mipi_dcs_write_cmd(0x3C, 1, 0x22);
    mipi_dcs_write_cmd(0x3D, 1, 0x22);
    mipi_dcs_write_cmd(0x3E, 1, 0x22);
    mipi_dcs_write_cmd(0x3F, 1, 0x22);
    mipi_dcs_write_cmd(0x40, 1, 0x22);
    mipi_dcs_write_cmd(0x52, 1, 0x12);
    mipi_dcs_write_cmd(0x53, 1, 0x12); //VGLO refer VGL_REG   1A

    // //****************************************************************************//
    // //****************************** Page 0 Command ******************************//
    // //****************************************************************************//

    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x00); // Change to Page 0

    mipi_dcs_write_cmd(0x11, 1, 0x00); // Sleep-Out
    vTaskDelay(pdMS_TO_TICKS(120));
    mipi_dcs_write_cmd(0x29, 1, 0x00); // Display On

    ESP_LOGI(TAG, "Init done");

    return ESP_OK;
}
