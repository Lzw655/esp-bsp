#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

#include "lv_100ask_calc.h"

class Calculator: public ESP_UiApp
{
public:
	Calculator();
	~Calculator();

    bool run(void);
    bool back(void);
    bool close(void);

    bool init(void) override;
};
