/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sdkconfig.h"
#include "bsp/esp-bsp.h"
#include "bsp/bsp_board_extra.h"
#include "gui_music/lv_example_pub.h"
#include "gui_music/lv_demo_music.h"

#include "MusicPlayer.h"

using namespace std;

LV_IMG_DECLARE(img_music_player_icon);

MusicPlayer::MusicPlayer(bool use_statusbar, bool use_navigation):
    ESP_UiApp(
        "Music Player",         // name
        0,                      // app_table_index
        true,                   // enable_resource_recycle
        true,                   // use_scr_act
        use_statusbar,          // use_statusbar
        use_navigation,         // use_navigation
        &img_music_player_icon) // icon
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
#if CONFIG_EXAMPLE_USE_SD_CARD
    ESP_ERROR_CHECK(bsp_extra_player_init(BSP_SD_MOUNT_POINT "/music"));
#else
    ESP_ERROR_CHECK(bsp_extra_player_init(BSP_SPIFFS_MOUNT_POINT "/music"));
#endif

    _status_icon_vector.push_back(&img_music_player_icon);
}
