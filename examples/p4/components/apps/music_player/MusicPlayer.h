/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

class MusicPlayer: public ESP_UiApp {
public:
    MusicPlayer(bool use_statusbar = false, bool use_navigation = false);
    ~MusicPlayer();

    void run(void);
    void back(void);
    void close(void);

    void init(void) override;
};
