#include "Memory_game.h"

using namespace std;

#define MEMORY_GAME_DEFAULT_HEIGHT         600
#define MEMORY_GAME_DEFAULT_WIDTH          600

lv_obj_t * level_label;

LV_IMG_DECLARE(img_app_memory_game);


Memory_game::Memory_game():
    ESP_UiApp(
        "Memory game",          // name
        &img_app_memory_game,   // icon
        0,                      // app_table_index
        true,                   // use_scr_act
        true,                   // enable_resource_recycle
        false,                  // use_statusbar
        true,                   // use_navigation
        false)                  // auto_resize_visual_area

{
}

Memory_game::~Memory_game()
{

}

static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    lv_obj_t * memory_game = (lv_obj_t *)lv_event_get_user_data(e);

    lv_100ask_memory_game_set_map(memory_game, lv_slider_get_value(slider), lv_slider_get_value(slider));
    lv_label_set_text_fmt(level_label, "%d*%d", lv_100ask_memory_game_get_row(memory_game), lv_100ask_memory_game_get_row(memory_game));
}

bool Memory_game::run()
{
	lv_obj_t * memory_game = lv_100ask_memory_game_create(lv_scr_act());
    lv_obj_set_size(memory_game, MEMORY_GAME_DEFAULT_WIDTH, MEMORY_GAME_DEFAULT_HEIGHT);
    lv_100ask_memory_game_set_map(memory_game, 2, 2);
    lv_obj_set_style_text_font(memory_game, &lv_font_montserrat_30, 0);
    lv_obj_center(memory_game);

    lv_obj_t * slider = lv_slider_create(lv_scr_act());
    lv_obj_align_to(slider, memory_game, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_slider_set_range(slider, 2, 10);
    lv_slider_set_value(slider, lv_100ask_memory_game_get_row(memory_game), LV_ANIM_ON);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, memory_game);

    level_label = lv_label_create(lv_scr_act());
    lv_label_set_text_fmt(level_label, "%d*%d", lv_100ask_memory_game_get_row(memory_game), lv_100ask_memory_game_get_row(memory_game));

    lv_obj_align_to(level_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    return true;
}

/**
 * @brief The function will be called when the left button of navigate bar is clicked.
 */
bool Memory_game::back(void)
{
    close();

    return true;
}

/**
 * @brief The function will be called when app should be closed.
 */
bool Memory_game::close(void)
{
    notifyManagerClosed();

    return true;
}

bool Memory_game::init(void)
{
    _status_icon_vector.push_back(&img_app_memory_game);

    return true;
}