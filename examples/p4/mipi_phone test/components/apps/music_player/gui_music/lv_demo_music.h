/**
 * @file lv_demo_music.h
 *
 */

#ifndef MUSIC_DEMO_MUSIC_H
#define MUSIC_DEMO_MUSIC_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

#define APP_USE_DEMO_MUSIC       1
#define APP_DEMO_MUSIC_LARGE     1

#if APP_USE_DEMO_MUSIC

/*********************
 *      DEFINES
 *********************/

#if APP_DEMO_MUSIC_LARGE
#  define MUSIC_DEMO_MUSIC_HANDLE_SIZE  40
#else
#  define MUSIC_DEMO_MUSIC_HANDLE_SIZE  20
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void lv_demo_music(lv_obj_t *parent);
const char *_lv_demo_music_get_title(uint32_t track_id);
const char *_lv_demo_music_get_artist(uint32_t track_id);
const char *_lv_demo_music_get_genre(uint32_t track_id);
uint32_t _lv_demo_music_get_track_length(uint32_t track_id);

/**********************
 *      MACROS
 **********************/

#endif /*APP_USE_DEMO_MUSIC*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*MUSIC_DEMO_MUSIC_H*/
