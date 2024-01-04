/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "ui/ui.h"
#include "Setting.h"

using namespace std;

LV_IMG_DECLARE(img_app_setting);

typedef enum {
    UI_MAIN_SETTING_INDEX = 0,
    UI_WIFI_SETTING_INDEX,
    UI_BLUETOOTH_SETTING_INDEX,
    UI_VOLUME_SETTING_INDEX,
    UI_BRIGHTNESS_SETTING_INDEX,
    UI_ABOUT_SETTING_INDEX,
    UI_MAX_INDEX,
} ui_src_index_t;

static ui_src_index_t scr_index;

void ui_event_ScreenSettingMain( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        if(target == ui_ScreenSettingMain) {
            scr_index = UI_MAIN_SETTING_INDEX;
        } else if(target == ui_ScreenSettingWiFi) {
            scr_index = UI_WIFI_SETTING_INDEX;
        } else if(target == ui_ScreenSettingBLE) {
            scr_index = UI_BLUETOOTH_SETTING_INDEX;
        } else if(target == ui_ScreenSettingVolume) {
            scr_index = UI_VOLUME_SETTING_INDEX;
        } else if(target == ui_ScreenSettingLight) {
            scr_index = UI_BRIGHTNESS_SETTING_INDEX;
        } else if(target == ui_ScreenSettingAbout) {
            scr_index = UI_ABOUT_SETTING_INDEX;
        }
    }
}

Setting::Setting():
    ESP_UiApp(
        "Setting",              // name
        0,                      // app_table_index
        true,                   // enable_resource_recycle
        true,                   // use_scr_act
        &img_app_setting,       // icon
        false,                  // use_statusbar
        true)                   // use_navigation

{
}

Setting::~Setting()
{
}

void Setting::run(void)
{
    ui_setting_init();

    lv_obj_add_event_cb(ui_ScreenSettingMain, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSettingWiFi, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSettingBLE, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSettingVolume, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSettingLight, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSettingAbout, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
}

void Setting::back(void)
{
    if (scr_index != UI_MAIN_SETTING_INDEX) {
        _ui_screen_change(&ui_ScreenSettingMain, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenSettingMain_screen_init);
    } else {
        close();
    }
}

void Setting::close(void)
{
    notifyManagerClosed();
}

void Setting::init(void)
{
    _status_icon_vector.push_back(&img_app_setting);
}
