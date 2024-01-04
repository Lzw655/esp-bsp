#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

#include "lv_100ask_sketchpad.h"

class Sketchpad: public ESP_UiApp
{
public:
	Sketchpad();
	~Sketchpad();

    bool run(void);
    bool back(void);
    bool close(void);

    bool init(void) override;
};
