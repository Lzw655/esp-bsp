/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "ui/ui.h"
#include "Setting.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_memory_utils.h"
#include "bsp/esp-bsp.h"
#include "bsp/bsp_board_extra.h"

using namespace std;

LV_IMG_DECLARE(img_app_setting);

static int light_intensity = 100;
static int pre_light_intensity = 100;

static int vloume_intensity = CODEC_DEFAULT_VOLUME;
static int pre_vloume_intensity = CODEC_DEFAULT_VOLUME;

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

void ui_event_SliderPanelScreenSettingLightSwitch1( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    pre_light_intensity = light_intensity;

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        light_intensity = lv_slider_get_value(ui_SliderPanelScreenSettingLightSwitch1);

        if (pre_light_intensity != light_intensity && light_intensity >= 20)
        {
            pre_light_intensity = light_intensity;
            bsp_display_brightness_set(light_intensity);
        }
    }
}

void ui_event_SliderPanelScreenSettingVolumeSwitch1( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    pre_vloume_intensity = vloume_intensity;

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        vloume_intensity = lv_slider_get_value(ui_SliderPanelScreenSettingVolumeSwitch);

        if (pre_vloume_intensity != vloume_intensity)
        {
            pre_vloume_intensity = vloume_intensity;
            bsp_extra_codec_volume_set(vloume_intensity, NULL);
            setVolumeIntensity(vloume_intensity);
        }
    }
}

void setting_task(void *arg)
{
    ESP_UiHome *home = (ESP_UiHome *)arg;

    while (1)
    {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);
        home->setClockStatusIconTime(timeinfo.tm_hour, timeinfo.tm_min);

        home->setBackstageMemoryLabel(heap_caps_get_free_size(MALLOC_CAP_INTERNAL)/1024, heap_caps_get_free_size(MALLOC_CAP_SPIRAM)/1024, heap_caps_get_total_size(MALLOC_CAP_INTERNAL)/1024, heap_caps_get_total_size(MALLOC_CAP_SPIRAM)/1024);

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

Setting::Setting():
    ESP_UiApp(
        "Setting",              // name
        &img_app_setting,       // icon
        0,                      // app_table_index
        true,                   // use_scr_act
        true,                   // enable_resource_recycle
        false,                  // use_statusbar
        true,                   // use_navigation
        false)                  // auto_resize_visual_area
{
}

Setting::~Setting()
{
}

bool Setting::run(void)
{
    ui_setting_init();

    lv_obj_add_event_cb(ui_ScreenSettingMain, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSettingWiFi, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSettingBLE, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSettingVolume, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSettingLight, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSettingAbout, ui_event_ScreenSettingMain, LV_EVENT_ALL, NULL);

    lv_obj_add_event_cb(ui_SliderPanelScreenSettingLightSwitch1, ui_event_SliderPanelScreenSettingLightSwitch1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SliderPanelScreenSettingVolumeSwitch, ui_event_SliderPanelScreenSettingVolumeSwitch1, LV_EVENT_ALL, NULL);

    vloume_intensity = getVolumeIntensity();

    lv_slider_set_value(ui_SliderPanelScreenSettingLightSwitch1, light_intensity, LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderPanelScreenSettingVolumeSwitch, vloume_intensity, LV_ANIM_OFF);

    return true;
}

bool Setting::back(void)
{
    if (scr_index != UI_MAIN_SETTING_INDEX) {
        _ui_screen_change(&ui_ScreenSettingMain, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenSettingMain_screen_init);
    } else {
        close();
    }

    return true;
}

bool Setting::close(void)
{
    notifyManagerClosed();

    return true;
}

bool Setting::init(void)
{
    _status_icon_vector.push_back(&img_app_setting);

    xTaskCreate(setting_task, "setting_task", 1024 * 4, _home, 5, NULL);

    return true;
}
