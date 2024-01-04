#include "Calculator.h"

using namespace std;

LV_IMG_DECLARE(img_app_calculator);

#define CALCULATOR_DEFAULT_HEIGHT         1100
#define CALCULATOR_DEFAULT_WIDTH          800

Calculator::Calculator():
    ESP_UiApp(
        "Calculator",           // name
        0,                      // app_table_index
        true,                   // enable_resource_recycle
        true,                   // use_scr_act
        &img_app_calculator,    // icon
        false,                  // use_statusbar
        true)                   // use_navigation

{
}


Calculator::~Calculator()
{

}

void Calculator::run(void)
{
	lv_obj_t * calc = lv_100ask_calc_create(lv_scr_act());
    lv_obj_set_size(calc, CALCULATOR_DEFAULT_WIDTH, CALCULATOR_DEFAULT_HEIGHT);
    lv_obj_center(calc);

    lv_obj_t * calc_ta_hist = lv_100ask_calc_get_ta_hist(calc);
    lv_obj_set_style_text_font(calc_ta_hist, &lv_font_montserrat_38, 0);

    lv_obj_t * calc_ta_input = lv_100ask_calc_get_ta_input(calc);
    lv_obj_set_style_text_font(calc_ta_input, &lv_font_montserrat_40, 0);

    lv_obj_t * calc_ta_btnm = lv_100ask_calc_get_btnm(calc);
    lv_obj_set_style_text_font(calc_ta_btnm, &lv_font_montserrat_40, 0);
}

/**
 * @brief The function will be called when the left button of navigate bar is clicked.
 */
void Calculator::back(void)
{
    close();
}

/**
 * @brief The function will be called when app should be closed.
 */
void Calculator::close(void)
{
    notifyManagerClosed();
}

void Calculator::init(void)
{
    _status_icon_vector.push_back(&img_app_calculator);
}