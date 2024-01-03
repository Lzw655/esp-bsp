#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

#include "lv_100ask_2048.h"

class Game2048: public ESP_UiApp
{
public:
	Game2048();
	~Game2048();

    void run(void);
    void back(void);
    void close(void);

    void init(void) override;
};
