#include "Game_2048.h"

using namespace std;

LV_IMG_DECLARE(img_app_2048);

#define GAME2048_DEFAULT_HEIGHT         700
#define GAME2048_DEFAULT_WIDTH          700

static void game_2048_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj_2048 = static_cast<lv_obj_t*>(lv_event_get_target(e));
    lv_obj_t * label = static_cast<lv_obj_t*>(lv_event_get_user_data(e));

    if(code == LV_EVENT_VALUE_CHANGED) {
        if (lv_100ask_2048_get_best_tile(obj_2048) >= 2048) {
            lv_label_set_text(label, "#00b329 YOU WIN! #");
        } else if(lv_100ask_2048_get_status(obj_2048)) {
            lv_label_set_text(label, "#00b329 # #ff0000 GAME OVER! #");
        } else {
            lv_label_set_text_fmt(label, "SCORE: #ff00ff %d #", lv_100ask_2048_get_score(obj_2048));
        }
    }
}

static void new_game_btn_event_handler(lv_event_t * e)
{
    lv_obj_t * obj_2048 = static_cast<lv_obj_t*>(lv_event_get_user_data(e));

    lv_100ask_2048_set_new_game(obj_2048);
}

Game2048::Game2048():
    ESP_UiApp(
        "2048 Game",           // name
        &img_app_2048,         // icon
        0,                     // app_table_index
        true,                  // use_scr_act
        true,                  // enable_resource_recycle
        false,                 // use_statusbar
        true,                  // use_navigation
        false)                 // auto_resize_visual_area

{
}


Game2048::~Game2048()
{

}

bool Game2048::run(void)
{
    /*Create 2048 game*/
    lv_obj_t * obj_2048 = lv_100ask_2048_create(lv_scr_act());
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
#if LV_FONT_MONTSERRAT_40
    lv_obj_set_style_text_font(obj_2048, &lv_font_montserrat_40, 0);
#endif
    lv_obj_set_size(obj_2048, GAME2048_DEFAULT_WIDTH, GAME2048_DEFAULT_HEIGHT);
    lv_obj_center(obj_2048);

    /*Information*/
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label, true);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_38, 0);
    lv_label_set_text_fmt(label, "SCORE: #ff00ff %d #", lv_100ask_2048_get_score(obj_2048));
    lv_obj_align_to(label, obj_2048, LV_ALIGN_OUT_TOP_RIGHT, 0, -10);

    lv_obj_add_event_cb(obj_2048, game_2048_event_cb, LV_EVENT_ALL, label);

    /*New Game*/
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x3f72af), LV_PART_MAIN);
    lv_obj_set_size(btn, 130, 100);
    lv_obj_align_to(btn, obj_2048, LV_ALIGN_OUT_TOP_LEFT, 0, -25);
    lv_obj_add_event_cb(btn, new_game_btn_event_handler, LV_EVENT_CLICKED, obj_2048);

    label = lv_label_create(btn);
    lv_label_set_text(label, "New Game");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_22, 0);
    lv_obj_center(label);

    return true;
}

/**
 * @brief The function will be called when the left button of navigate bar is clicked.
 */
bool Game2048::back(void)
{
    close();

    return true;
}

/**
 * @brief The function will be called when app should be closed.
 */
bool Game2048::close(void)
{
    notifyManagerClosed();

    return true;
}

bool Game2048::init(void)
{
    _status_icon_vector.push_back(&img_app_2048);

    return true;
}
