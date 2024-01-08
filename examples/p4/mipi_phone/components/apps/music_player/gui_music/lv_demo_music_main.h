/**
 * @file lv_demo_music_main.h
 *
 */

#ifndef MUSIC_DEMO_MUSIC_MAIN_H
#define MUSIC_DEMO_MUSIC_MAIN_H

#include "lvgl.h"

#include "lv_demo_music.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#if APP_USE_DEMO_MUSIC

/*********************
 *      DEFINES
 *********************/
#define ACTIVE_TRACK_CNT    2

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
lv_obj_t *_lv_demo_music_main_create(lv_obj_t *parent);
void _lv_demo_music_play(uint32_t id);
void _lv_demo_music_resume(void);
void _lv_demo_music_pause(void);
void _lv_demo_music_album_next(bool next);

/**********************
 *      MACROS
 **********************/
#endif /*APP_USE_DEMO_MUSIC*/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*MUSIC_DEMO_MUSIC_MAIN_H*/
