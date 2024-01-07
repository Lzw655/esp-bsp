/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <array>
#include "lvgl.h"
#include "ESP_UI.h"

class Setting: public ESP_UiApp {
public:
    Setting();
    ~Setting();

    bool run(void);
    bool back(void);
    bool close(void);

    bool init(void) override;

private:
    typedef enum {
        UI_MAIN_SETTING_INDEX = 0,
        UI_WIFI_SETTING_INDEX,
        UI_BLUETOOTH_SETTING_INDEX,
        UI_VOLUME_SETTING_INDEX,
        UI_BRIGHTNESS_SETTING_INDEX,
        UI_ABOUT_SETTING_INDEX,
        UI_MAX_INDEX,
    } SettingScreenIndex_t;

    static void homeRefreshTask(void *arg);

    static void onScreenLoadEventCallback( lv_event_t * e);
    static void onSliderPanelVolumeSwitchValueChangeEventCallback( lv_event_t * e);
    static void onSliderPanelLightSwitchValueChangeEventCallback( lv_event_t * e);

    int _lcd_brightness = -1;
    int _speaker_volume = -1;
    SettingScreenIndex_t _scr_index;
    std::array<lv_obj_t *, UI_MAX_INDEX> _scr_list;
};
