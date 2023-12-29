#pragma once

#include "lvgl.h"
#include "lv_demos.h"
#include "draw/sw/lv_draw_sw.h"

#ifdef __cplusplus
extern "C" {
#endif

int gui_init(lv_disp_t **disp_array, lv_indev_t **indev_array, lv_theme_t * th);

#ifdef __cplusplus
}
#endif