/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "ui/ui.h"
#include "WeatherForecast.h"

using namespace std;

LV_IMG_DECLARE(img_app_weather);

WeatherForecast::WeatherForecast():
    ESP_UiApp(
        "Weather Forecast",     // name
        &img_app_weather,       // icon
        0,                      // app_table_index
        true,                   // use_scr_act
        true,                   // enable_resource_recycle
        false,                  // use_statusbar
        true,                   // use_navigation
        false)                  // auto_resize_visual_area
{
}

WeatherForecast::~WeatherForecast()
{
}

bool WeatherForecast::run(void)
{
    ui_weather_init();

    return true;
}

bool WeatherForecast::back(void)
{
    close();

    return true;
}

bool WeatherForecast::close(void)
{
    notifyManagerClosed();

    return true;
}

bool WeatherForecast::init(void)
{
    _status_icon_vector.push_back(&img_app_weather);

    return true;
}
