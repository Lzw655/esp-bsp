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
        "Weather Forecast",         // name
        0,                      // app_table_index
        true,                   // enable_resource_recycle
        true,                   // use_scr_act
        &img_app_weather,       // icon
        false,                  // use_statusbar
        true)                  // use_navigation

{
}

WeatherForecast::~WeatherForecast()
{
}

void WeatherForecast::run(void)
{
    ui_weather_init();
}

void WeatherForecast::back(void)
{
    close();
}

void WeatherForecast::close(void)
{
    notifyManagerClosed();
}

void WeatherForecast::init(void)
{
    _status_icon_vector.push_back(&img_app_weather);
}
