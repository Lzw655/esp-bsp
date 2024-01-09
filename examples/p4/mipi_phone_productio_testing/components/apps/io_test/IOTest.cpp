/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "ui/ui.h"
#include "IOTest.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_memory_utils.h"
#include "bsp/esp-bsp.h"
#include "bsp/bsp_board_extra.h"
#include "bsp/display.h"
#include "bsp_lcd.h"

using namespace std;

LV_IMG_DECLARE(img_app_IOtest);

gpio_num_t gpio_pins[] = {GPIO_NUM_20, GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_23,
                            GPIO_NUM_32, GPIO_NUM_33,
                            GPIO_NUM_36, GPIO_NUM_37, GPIO_NUM_38,
                            GPIO_NUM_45, GPIO_NUM_46, GPIO_NUM_47, GPIO_NUM_48, GPIO_NUM_51, GPIO_NUM_52, GPIO_NUM_53, GPIO_NUM_54};

void IOTest::ButtonScreenIOtestONEventCallback( lv_event_t * e)
{
    IOTest *app = (IOTest *)lv_event_get_user_data(e);
    CHECK_NULL_RETURN(app, "Invalid app pointer");

    for(int i = 0; i < sizeof(gpio_pins) / sizeof(gpio_pins[0]); i++) {
        gpio_set_direction(gpio_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(gpio_pins[i], 1);
    }

}

void IOTest::ButtonScreenIOtestOFFEventCallback( lv_event_t * e)
{
    IOTest *app = (IOTest *)lv_event_get_user_data(e);
    CHECK_NULL_RETURN(app, "Invalid app pointer");

    for(int i = 0; i < sizeof(gpio_pins) / sizeof(gpio_pins[0]); i++) {
        gpio_set_direction(gpio_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(gpio_pins[i], 0);
    }
}

IOTest::IOTest():
    ESP_UiApp(
        "IO Test",     // name
        &img_app_IOtest,       // icon
        0,                      // app_table_index
        true,                   // use_scr_act
        true,                   // enable_resource_recycle
        false,                  // use_statusbar
        true,                   // use_navigation
        false)                  // auto_resize_visual_area
{
}

IOTest::~IOTest()
{
}

bool IOTest::run(void)
{
    ui_IO_test_init();

    lv_obj_add_event_cb(ui_ButtonScreenIOtestON, ButtonScreenIOtestONEventCallback, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(ui_ButtonScreenIOtestOFF, ButtonScreenIOtestOFFEventCallback, LV_EVENT_CLICKED, this);

    return true;
}

bool IOTest::back(void)
{
    close();

    return true;
}

bool IOTest::close(void)
{
    notifyManagerClosed();

    // for(int i = 0; i < sizeof(gpio_pins) / sizeof(gpio_pins[0]); i++) {
    //     gpio_reset_pin(gpio_pins[i]);
    // }
    for(int i = 0; i < sizeof(gpio_pins) / sizeof(gpio_pins[0]); i++) {
        gpio_set_direction(gpio_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(gpio_pins[i], 0);
    }

    return true;
}

bool IOTest::init(void)
{
    _status_icon_vector.push_back(&img_app_IOtest);

    return true;
}
