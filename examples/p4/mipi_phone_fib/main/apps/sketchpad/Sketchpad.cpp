#include "Sketchpad.h"

using namespace std;

LV_IMG_DECLARE(img_app_sketchpad);

#define SKETCHPAD_DEFAULT_HEIGHT         1280
#define SKETCHPAD_DEFAULT_WIDTH          800

static lv_color_t* cbuf;

Sketchpad::Sketchpad():
    ESP_UiApp(
        "Sketchpad",         // name
        0,                      // app_table_index
        true,                   // enable_resource_recycle
        true,                  // use_scr_act
        false,                  // use_statusbar
        true,                  // use_navigation
        &img_app_sketchpad)   // icon
{
}


Sketchpad::~Sketchpad()
{

}

void Sketchpad::run(void)
{
    int buffer_size = LV_CANVAS_BUF_SIZE_TRUE_COLOR(SKETCHPAD_DEFAULT_WIDTH, SKETCHPAD_DEFAULT_HEIGHT);
    cbuf = (lv_color_t*)malloc(buffer_size * sizeof(lv_color_t));

	lv_obj_t * sketchpad = lv_100ask_sketchpad_create(lv_scr_act());

	lv_canvas_set_buffer(sketchpad, cbuf, SKETCHPAD_DEFAULT_WIDTH, SKETCHPAD_DEFAULT_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_center(sketchpad);
    lv_canvas_fill_bg(sketchpad, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_OPA_COVER);
}

/**
 * @brief The function will be called when the left button of navigate bar is clicked.
 */
void Sketchpad::back(void)
{
    close();
}

/**
 * @brief The function will be called when app should be closed.
 */
void Sketchpad::close(void)
{
    notifyManagerClosed();
    free(cbuf);
}

void Sketchpad::init(void)
{
    _status_icon_vector.push_back(&img_app_sketchpad);
}