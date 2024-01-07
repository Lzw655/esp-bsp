/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cmath>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_memory_utils.h"
#include "bsp/esp-bsp.h"
#include "bsp/bsp_board_extra.h"

#include "ui/ui.h"
#include "Setting.h"

#define HOME_REFRESH_TASK_STACK_SIZE     (1024 * 4)
#define HOME_REFRESH_TASK_PRIORITY       (1)
#define HOME_REFRESH_TASK_PERIOD_MS      (500)

#define SCREEN_BRIGHTNESS_MIN           (20)
#define SCREEN_BRIGHTNESS_MAX           (100)

#define SPEAKER_VOLUME_MIN              (0)
#define SPEAKER_VOLUME_MAX              (100)

using namespace std;

LV_IMG_DECLARE(img_app_setting);

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

    // Record screen index by loaded event
    _scr_list[UI_MAIN_SETTING_INDEX] = ui_ScreenSettingMain;
    _scr_list[UI_WIFI_SETTING_INDEX] = ui_ScreenSettingWiFi;
    _scr_list[UI_BLUETOOTH_SETTING_INDEX] = ui_ScreenSettingBLE;
    _scr_list[UI_VOLUME_SETTING_INDEX] = ui_ScreenSettingVolume;
    _scr_list[UI_BRIGHTNESS_SETTING_INDEX] = ui_ScreenSettingLight;
    _scr_list[UI_ABOUT_SETTING_INDEX] = ui_ScreenSettingAbout;
    lv_obj_add_event_cb(ui_ScreenSettingMain, onScreenLoadEventCallback, LV_EVENT_SCREEN_LOADED, this);
    lv_obj_add_event_cb(ui_ScreenSettingWiFi, onScreenLoadEventCallback, LV_EVENT_SCREEN_LOADED, this);
    lv_obj_add_event_cb(ui_ScreenSettingBLE, onScreenLoadEventCallback, LV_EVENT_SCREEN_LOADED, this);
    lv_obj_add_event_cb(ui_ScreenSettingVolume, onScreenLoadEventCallback, LV_EVENT_SCREEN_LOADED, this);
    lv_obj_add_event_cb(ui_ScreenSettingLight, onScreenLoadEventCallback, LV_EVENT_SCREEN_LOADED, this);
    lv_obj_add_event_cb(ui_ScreenSettingAbout, onScreenLoadEventCallback, LV_EVENT_SCREEN_LOADED, this);

    // Initialize and configure page `Volume`
    lv_slider_set_range(ui_SliderPanelScreenSettingVolumeSwitch, SPEAKER_VOLUME_MIN, SPEAKER_VOLUME_MAX);
    _speaker_volume = bsp_extra_codec_volume_get();
    _speaker_volume = max(min(_speaker_volume, SPEAKER_VOLUME_MAX), SPEAKER_VOLUME_MIN);
    lv_slider_set_value(ui_SliderPanelScreenSettingVolumeSwitch, _speaker_volume, LV_ANIM_OFF);
    lv_obj_add_event_cb(ui_SliderPanelScreenSettingVolumeSwitch, onSliderPanelVolumeSwitchValueChangeEventCallback,
                        LV_EVENT_VALUE_CHANGED, this);

    // Initialize and configure page `Brightness`
    lv_slider_set_range(ui_SliderPanelScreenSettingLightSwitch1, SCREEN_BRIGHTNESS_MIN, SCREEN_BRIGHTNESS_MAX);
    _lcd_brightness = bsp_display_brightness_get();
    _lcd_brightness = max(min(_lcd_brightness, SCREEN_BRIGHTNESS_MAX), SCREEN_BRIGHTNESS_MIN);
    lv_slider_set_value(ui_SliderPanelScreenSettingLightSwitch1, _lcd_brightness, LV_ANIM_OFF);
    lv_obj_add_event_cb(ui_SliderPanelScreenSettingLightSwitch1, onSliderPanelLightSwitchValueChangeEventCallback,
                        LV_EVENT_VALUE_CHANGED, this);

    return true;
}

bool Setting::back(void)
{
    if (_scr_index != UI_MAIN_SETTING_INDEX) {
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

    xTaskCreate(homeRefreshTask, "home refresh", HOME_REFRESH_TASK_STACK_SIZE, this, HOME_REFRESH_TASK_PRIORITY, NULL);

    return true;
}

void Setting::homeRefreshTask(void *arg)
{
    time_t now;
    struct tm timeinfo;
    uint16_t free_sram_size_kb = 0;
    uint16_t total_sram_size_kb = 0;
    uint16_t free_psram_size_kb = 0;
    uint16_t total_psram_size_kb = 0;

    Setting *app = (Setting *)arg;
    CHECK_NULL_GOTO(app, err, "Invalid app pointer");

    while (1) {
        // Update time in status bar
        time(&now);
        localtime_r(&now, &timeinfo);

        bsp_display_lock(0);
        if (!app->_home->setClockStatusIconTime(timeinfo.tm_hour, timeinfo.tm_min)) {
            ESP_LOGE(app->name().c_str(), "Set clock status icon time failed");
        }
        bsp_display_unlock();

        // Update memory in backstage
        if (app->_home->isBackstageVisible()) {
            free_sram_size_kb = heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024;
            total_sram_size_kb = heap_caps_get_total_size(MALLOC_CAP_INTERNAL) / 1024;
            free_psram_size_kb = heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024;
            total_psram_size_kb = heap_caps_get_total_size(MALLOC_CAP_SPIRAM) / 1024;
            ESP_LOGI(app->name().c_str(), "Free sram size: %d KB, total sram size: %d KB, "
                     "free psram size: %d KB, total psram size: %d KB",
                     free_sram_size_kb, total_sram_size_kb, free_psram_size_kb, total_psram_size_kb);

            bsp_display_lock(0);
            if (!app->_home->setBackstageMemoryLabel(free_sram_size_kb, free_psram_size_kb,
                                                    total_sram_size_kb, total_psram_size_kb)) {
                ESP_LOGE(app->name().c_str(), "Set backstage memory label failed");
            }
            bsp_display_unlock();
        }

        vTaskDelay(pdMS_TO_TICKS(HOME_REFRESH_TASK_PERIOD_MS));
    }

err:
    vTaskDelete(NULL);
}

void Setting::onScreenLoadEventCallback( lv_event_t * e)
{
    Setting *app = (Setting *)lv_event_get_user_data(e);
    CHECK_NULL_RETURN(app, "Invalid app pointer");

    for (int i = 0; i < UI_MAX_INDEX; i++) {
        if (app->_scr_list[i] == lv_event_get_target(e)) {
            app->_scr_index = (SettingScreenIndex_t)i;
            break;
        }
    }
}

void Setting::onSliderPanelVolumeSwitchValueChangeEventCallback( lv_event_t * e) {
    Setting *app = (Setting *)lv_event_get_user_data(e);
    CHECK_NULL_RETURN(app, "Invalid app pointer");

    int volume = lv_slider_get_value(ui_SliderPanelScreenSettingVolumeSwitch);

    if (volume != app->_speaker_volume) {
        if ((bsp_extra_codec_volume_set(volume, NULL) != ESP_OK) && (bsp_extra_codec_volume_get() != volume)) {
            ESP_LOGE(app->name().c_str(), "Set volume failed");
            lv_slider_set_value(ui_SliderPanelScreenSettingVolumeSwitch, app->_speaker_volume, LV_ANIM_OFF);
            return;
        }
        app->_speaker_volume = volume;
    }
}

void Setting::onSliderPanelLightSwitchValueChangeEventCallback( lv_event_t * e) {
    Setting *app = (Setting *)lv_event_get_user_data(e);
    CHECK_NULL_RETURN(app, "Invalid app pointer");

    int brightness = lv_slider_get_value(ui_SliderPanelScreenSettingLightSwitch1);

    if (brightness != app->_lcd_brightness) {
        if ((bsp_display_brightness_set(brightness) != ESP_OK) && (bsp_display_brightness_get() != brightness)) {
            ESP_LOGE(app->name().c_str(), "Set brightness failed");
            lv_slider_set_value(ui_SliderPanelScreenSettingLightSwitch1, app->_lcd_brightness, LV_ANIM_OFF);
            return;
        }
        app->_lcd_brightness = brightness;
    }
}
