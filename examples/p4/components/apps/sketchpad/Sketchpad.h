#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

#include "lv_100ask_sketchpad.h"

class Sketchpad: public ESP_UiApp
{
public:
	Sketchpad();
	~Sketchpad();

    void run(void);
    void back(void);
    void close(void);

    void init(void) override;
};
