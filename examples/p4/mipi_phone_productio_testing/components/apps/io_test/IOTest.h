/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

class IOTest: public ESP_UiApp {
public:
    IOTest();
    ~IOTest();

    bool run(void);
    bool back(void);
    bool close(void);

    bool init(void) override;

private:
    static void ButtonScreenIOtestONEventCallback(lv_event_t * e);
    static void ButtonScreenIOtestOFFEventCallback(lv_event_t * e);
};
