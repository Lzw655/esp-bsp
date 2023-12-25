/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "gui_music/lv_example_pub.h"
#include "gui_music/lv_demo_music.h"

#include "MusicPlayer.h"

using namespace std;

LV_IMG_DECLARE(img_music_player_icon);

MusicPlayer::MusicPlayer():
    ESP_UiApp(
        "Music Player",         // name
        0,                      // app_table_index
        true,                  // enable_resource_recycle
        true,                   // use_scr_act
        false,                  // use_statusbar
        false,                  // use_navigation
        &img_music_player_icon)  // icon
{
}

MusicPlayer::~MusicPlayer()
{
}

void MusicPlayer::run(void)
{
    lv_demo_music(lv_scr_act());
}

void MusicPlayer::back(void)
{
    close();
}

void MusicPlayer::close(void)
{
    notifyManagerClosed();
}

void MusicPlayer::init(void)
{
    _status_icon_vector.push_back(&img_music_player_icon);
}
