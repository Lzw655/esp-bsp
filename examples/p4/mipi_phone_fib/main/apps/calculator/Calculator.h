#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

#include "lv_100ask_calc.h"

class Calculator: public ESP_UiApp
{
public:
	Calculator();
	~Calculator();

    void run(void);
    void back(void);
    void close(void);

    void init(void) override;
};
