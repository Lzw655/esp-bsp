#include "Sketchpad.h"

using namespace std;

LV_IMG_DECLARE(img_app_sketchpad);

#define SKETCHPAD_DEFAULT_HEIGHT         1280
#define SKETCHPAD_DEFAULT_WIDTH          800

static lv_color_t* cbuf;

Sketchpad::Sketchpad():
    ESP_UiApp(
        "Sketchpad",            // name
        &img_app_sketchpad,     // icon
        0,                      // app_table_index
        true,                   // use_scr_act
        true,                   // enable_resource_recycle
        false,                  // use_statusbar
        true,                   // use_navigation
        false)                  // auto_resize_visual_area

{
}


Sketchpad::~Sketchpad()
{

}

bool Sketchpad::run(void)
{
    int buffer_size = LV_CANVAS_BUF_SIZE_TRUE_COLOR(SKETCHPAD_DEFAULT_WIDTH, SKETCHPAD_DEFAULT_HEIGHT);

    cbuf = (lv_color_t*)malloc(buffer_size * sizeof(lv_color_t));
    if(cbuf == NULL) {
        printf("Failed to allocate memory for canvas buffer\n");
        return false;
        }

	lv_obj_t * sketchpad = lv_100ask_sketchpad_create(lv_scr_act());
    printf("Failed to allocate memory for canvas buffer\n");

	lv_canvas_set_buffer(sketchpad, cbuf, SKETCHPAD_DEFAULT_WIDTH, SKETCHPAD_DEFAULT_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_center(sketchpad);
    lv_canvas_fill_bg(sketchpad, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_OPA_COVER);

    return true;
}

/**
 * @brief The function will be called when the left button of navigate bar is clicked.
 */
bool Sketchpad::back(void)
{
    close();

    return true;
}

/**
 * @brief The function will be called when app should be closed.
 */
bool Sketchpad::close(void)
{
    free(cbuf);
    cbuf = NULL;

    notifyManagerClosed();

    return true;
}

bool Sketchpad::init(void)
{
    _status_icon_vector.push_back(&img_app_sketchpad);

    return true;
}