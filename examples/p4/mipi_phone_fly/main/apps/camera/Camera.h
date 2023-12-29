/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

class Camera: public ESP_UiApp {
public:
    Camera(uint16_t hor_res, uint16_t ver_res, uint16_t refresh_hz);
    ~Camera();

    void run(void);
    void pause(void);
    void resume(void);
    void back(void);
    void close(void);

    void init(void) override;

private:
    static void onScreenCameraShotBtnClick(lv_event_t *e);
    static void onScreenCameraShotAlbumClick(lv_event_t *e);
    static void onScreenCameraShotLoad(lv_event_t *e);
    static void onScreenCameraPhotoLoad(lv_event_t *e);
    static void onTimerImageRefresh(lv_timer_t *timer);

    enum {
        SCREEN_CAMERA_SHOT,
        SCREEN_CAMERA_PHOTO,
    } _screen_index;
    uint16_t _hor_res;
    uint16_t _ver_res;
    uint16_t _img_album_dsc_size;
    uint32_t _img_album_buf_bytes;
    uint16_t _refresh_period_ms;
    uint8_t *_img_album_buffer;
    lv_img_dsc_t _img_refresh_dsc;
    lv_img_dsc_t _img_album_dsc;
    lv_img_dsc_t _img_photo_dsc;
    lv_obj_t *_img_album;
    lv_timer_t *_img_refresh_timer;
};
