/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

class Setting: public ESP_UiApp {
public:
    Setting();
    ~Setting();

    bool run(void);
    bool back(void);
    bool close(void);

    bool init(void) override;
};
