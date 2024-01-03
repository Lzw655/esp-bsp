#pragma once

#include "lvgl.h"
#include "ESP_UI.h"

#include "lv_100ask_memory_game.h"

class Memory_game: public ESP_UiApp
{
public:
	Memory_game();
	~Memory_game();

    void run(void);
    void back(void);
    void close(void);

    void init(void) override;
};