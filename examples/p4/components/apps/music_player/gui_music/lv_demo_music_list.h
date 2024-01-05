/**
 * @file lv_demo_music_list.h
 *
 */

#ifndef MUSIC_DEMO_MUSIC_LIST_H
#define MUSIC_DEMO_MUSIC_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lv_demo_music.h"
#if APP_USE_DEMO_MUSIC

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
lv_obj_t *_lv_demo_music_list_create(lv_obj_t *parent);
void _lv_demo_music_list_btn_check(uint32_t track_id, bool state);

/**********************
 *      MACROS
 **********************/

#endif /*APP_USE_DEMO_MUSIC*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*MUSIC_DEMO_MUSIC_LIST_H*/
