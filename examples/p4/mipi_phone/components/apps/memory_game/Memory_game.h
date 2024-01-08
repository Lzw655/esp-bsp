#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

#include "lv_100ask_memory_game.h"

class Memory_game: public ESP_UiApp
{
public:
	Memory_game();
	~Memory_game();

    bool run(void);
    bool back(void);
    bool close(void);

    bool init(void) override;
};