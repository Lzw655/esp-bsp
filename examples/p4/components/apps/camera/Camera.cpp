/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_check.h"
#include "esp_heap_caps.h"

#include "bsp/camera.h"
#include "ui/ui.h"
#include "Camera.h"

#define IMG_TIMER_DEFAULT_INTERVAL_MS       (LV_DISP_DEF_REFR_PERIOD)

using namespace std;

static const char *TAG = "Camera";

LV_IMG_DECLARE(img_app_camera);

Camera::Camera(uint16_t hor_res, uint16_t ver_res, uint16_t refresh_hz, bool use_statusbar, bool use_navigation):
    ESP_UiApp(
        "Camera",               // name
        &img_app_camera,       // icon
        0,                      // app_table_index
        false,                  // use_scr_act
        true,                   // enable_resource_recycle
        use_statusbar,          // use_statusbar
        use_navigation,         // use_navigation
        use_navigation || use_statusbar), // auto_resize_visual_area
    _screen_index(SCREEN_CAMERA_SHOT),
    _hor_res(hor_res),
    _ver_res(ver_res),
    _img_album_dsc_size(hor_res > ver_res ? ver_res : hor_res),
    _img_album_buffer(NULL),
    _img_refresh_timer(NULL)
{
    if (refresh_hz == 0) {
        _refresh_period_ms = IMG_TIMER_DEFAULT_INTERVAL_MS;
    } else {
        _refresh_period_ms = 1000 / refresh_hz;
        if (_refresh_period_ms == 0) {
            _refresh_period_ms = IMG_TIMER_DEFAULT_INTERVAL_MS;
        }
    }
    _img_album_buf_bytes = _img_album_dsc_size * _img_album_dsc_size * sizeof(lv_color_t);
}

Camera::~Camera()
{
}

bool Camera::run(void)
{
    ui_camera_init();

    _img_album_buffer = (uint8_t *)heap_caps_aligned_alloc(64, _img_refresh_dsc.data_size, MALLOC_CAP_SPIRAM);
    if (_img_album_buffer == NULL) {
        ESP_LOGE(TAG, "Allocate memory for album buffer failed");
        notifyManagerClosed();
        return false;
    }
    lv_img_dsc_t img_dsc = {
        .header = {
            .cf = LV_IMG_CF_TRUE_COLOR,
            .always_zero = 0,
            .reserved = 0,
            .w = _img_album_dsc_size,
            .h = _img_album_dsc_size,
        },
        .data_size = _img_album_buf_bytes,
        .data = (const uint8_t *)_img_album_buffer,
    };
    memcpy(&_img_album_dsc, &img_dsc, sizeof(lv_img_dsc_t));

    lv_obj_refr_size(ui_PanelCameraShotAlbum);
    lv_obj_clear_flag(ui_PanelCameraShotAlbum, LV_OBJ_FLAG_CLICKABLE);
    _img_album = lv_img_create(ui_PanelCameraShotAlbum);
    lv_obj_add_flag(_img_album, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_size(_img_album, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_center(_img_album);
    // Calculate the multiple of the size between the target and the image.
    float h_factor = (float)lv_obj_get_height(ui_PanelCameraShotAlbum) / (_img_album_dsc_size);
    float w_factor = (float)lv_obj_get_width(ui_PanelCameraShotAlbum) / (_img_album_dsc_size);
    // Scale the image to a suitable size.
    // So you don’t have to consider the size of the source image.
    if (h_factor < w_factor) {
        lv_img_set_zoom(_img_album, (int)(h_factor * LV_IMG_ZOOM_NONE));
    } else {
        lv_img_set_zoom(_img_album, (int)(w_factor * LV_IMG_ZOOM_NONE));
    }
    lv_img_set_size_mode(_img_album, LV_IMG_SIZE_MODE_REAL);
    lv_obj_refr_size(_img_album);
    lv_obj_add_event_cb(_img_album, onScreenCameraShotAlbumClick, LV_EVENT_CLICKED, this);

    lv_obj_set_height(ui_ImageCameraShotImage, _ver_res);
    lv_img_set_src(ui_ImageCameraShotImage, &_img_refresh_dsc);

    img_dsc.header.w = _hor_res;
    img_dsc.header.h = _ver_res;
    img_dsc.data_size = _img_refresh_dsc.data_size;
    memcpy(&_img_photo_dsc, &img_dsc, sizeof(lv_img_dsc_t));
    memcpy(_img_album_buffer, _img_refresh_dsc.data, _img_refresh_dsc.data_size);
    lv_obj_set_height(ui_ImageCameraPhotoImage, _ver_res);
    lv_img_set_src(ui_ImageCameraPhotoImage, &_img_photo_dsc);

    lv_obj_add_event_cb(ui_ScreenCameraShot, onScreenCameraShotLoad, LV_EVENT_SCREEN_LOADED, this);
    lv_obj_add_event_cb(ui_ScreenCameraPhoto, onScreenCameraPhotoLoad, LV_EVENT_SCREEN_LOADED, this);
    lv_obj_add_event_cb(ui_ButtonCameraShotBtn, onScreenCameraShotBtnClick, LV_EVENT_CLICKED, this);

    _img_refresh_timer = lv_timer_create(onTimerImageRefresh, _refresh_period_ms, this);

    return true;
}

bool Camera::pause(void)
{
    lv_timer_pause(_img_refresh_timer);

    return true;
}

bool Camera::resume(void)
{
    lv_timer_resume(_img_refresh_timer);

    return true;
}

bool Camera::back(void)
{
    if (_screen_index == SCREEN_CAMERA_PHOTO) {
        lv_scr_load(ui_ScreenCameraShot);
        resume();
    } else {
        close();
    }

    return true;
}

bool Camera::close(void)
{
    notifyManagerClosed();

    if (_img_album_buffer != NULL) {
        heap_caps_free(_img_album_buffer);
        _img_album_buffer = NULL;
    }

    return true;
}

bool Camera::init(void)
{
    _status_icon_vector.push_back(&img_app_camera);

    size_t frame_buffer_size = _hor_res * _ver_res * sizeof(lv_color_t);
    uint8_t *frame_buffer = (uint8_t *)heap_caps_aligned_alloc(64, frame_buffer_size, MALLOC_CAP_SPIRAM);
    assert(frame_buffer != NULL);

    bsp_camera_config_t camera_cfg = {
        .hor_res = _hor_res,
        .ver_res = _ver_res,
        .buffer_ptr = (void **)&frame_buffer,
        .buffer_size_ptr = &frame_buffer_size,
    };
    ESP_ERROR_CHECK(bsp_camera_new(&camera_cfg));

    lv_img_dsc_t img_dsc = {
        .header = {
            .cf = LV_IMG_CF_TRUE_COLOR,
            .always_zero = 0,
            .reserved = 0,
            .w = camera_cfg.hor_res,
            .h = camera_cfg.ver_res,
        },
        .data_size = frame_buffer_size,
        .data = (const uint8_t *)frame_buffer,
    };
    memcpy(&_img_refresh_dsc, &img_dsc, sizeof(lv_img_dsc_t));

    return true;
}

void Camera::onScreenCameraShotAlbumClick(lv_event_t *e)
{
    lv_obj_invalidate(ui_ImageCameraPhotoImage);
}

void Camera::onScreenCameraShotBtnClick(lv_event_t *e)
{
    Camera *camera = (Camera *)e->user_data;

    if (camera == NULL) {
        return;
    }

    lv_obj_add_flag(camera->_img_album, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(ui_PanelCameraShotAlbum, LV_OBJ_FLAG_CLICKABLE);
    memcpy(camera->_img_album_buffer, camera->_img_refresh_dsc.data, camera->_img_refresh_dsc.data_size);
    lv_img_set_src(camera->_img_album, &camera->_img_album_dsc);
}

void Camera::onScreenCameraShotLoad(lv_event_t *e)
{
    Camera *camera = (Camera *)e->user_data;

    if (camera) {
        camera->_screen_index = SCREEN_CAMERA_SHOT;
    }
}

void Camera::onScreenCameraPhotoLoad(lv_event_t *e)
{
    Camera *camera = (Camera *)e->user_data;

    if (camera) {
        camera->_screen_index = SCREEN_CAMERA_PHOTO;
    }
}

void Camera::onTimerImageRefresh(lv_timer_t *timer)
{
    lv_obj_invalidate(ui_ImageCameraShotImage);
}
