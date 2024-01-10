/**
 * @file lv_100ask_sketchpad.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_100ask_sketchpad.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lv_100ask_sketchpad_class
/**********************
 *      TYPEDEFS
 **********************/

LV_IMG_DECLARE(img_color);
LV_IMG_DECLARE(img_pen);
LV_IMG_DECLARE(img_erase);

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_100ask_sketchpad_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_100ask_sketchpad_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_100ask_sketchpad_toolbar_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_100ask_sketchpad_toolbar_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_100ask_sketchpad_event(const lv_obj_class_t * class_p, lv_event_t * e);
static void lv_100ask_sketchpad_toolbar_event(const lv_obj_class_t * class_p, lv_event_t * e);
static void sketchpad_toolbar_event_cb(lv_event_t * e);
static void toolbar_set_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_100ask_sketchpad_class = {
    .constructor_cb = lv_100ask_sketchpad_constructor,
    .destructor_cb = lv_100ask_sketchpad_destructor,
    .event_cb = lv_100ask_sketchpad_event,
    .instance_size = sizeof(lv_100ask_sketchpad_t),
    .base_class = &lv_canvas_class
};

const lv_obj_class_t lv_100ask_sketchpad_toolbar_class = {
    .constructor_cb = lv_100ask_sketchpad_toolbar_constructor,
    .destructor_cb = lv_100ask_sketchpad_toolbar_destructor,
    .event_cb = lv_100ask_sketchpad_toolbar_event,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .base_class = &lv_obj_class
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ClearbtnEventCallback( lv_event_t * e)
{
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_user_data(e);

    lv_canvas_fill_bg(obj, lv_color_white(), LV_OPA_COVER);
}

lv_obj_t * lv_100ask_sketchpad_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * canvas_obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(canvas_obj);

    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 100, 50);
    lv_obj_set_align(btn, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x364f6b), 0);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Clear");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);

    lv_obj_add_event_cb(btn, ClearbtnEventCallback, LV_EVENT_CLICKED, (void*)canvas_obj);

    return canvas_obj;
}

/*=====================
 * Other functions
 *====================*/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_100ask_sketchpad_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_100ask_sketchpad_t * sketchpad = (lv_100ask_sketchpad_t *)obj;

    sketchpad->dsc.header.always_zero = 0;
    sketchpad->dsc.header.cf          = LV_IMG_CF_TRUE_COLOR;
    sketchpad->dsc.header.h           = 0;
    sketchpad->dsc.header.w           = 0;
    sketchpad->dsc.data_size          = 0;
    sketchpad->dsc.data               = NULL;

    lv_draw_line_dsc_init(&sketchpad->line_rect_dsc);
    sketchpad->line_rect_dsc.width = 2;
    sketchpad->line_rect_dsc.round_start = true;
    sketchpad->line_rect_dsc.round_end = true;
    sketchpad->line_rect_dsc.color = lv_color_black();
    sketchpad->line_rect_dsc.opa = LV_OPA_COVER;

    lv_img_set_src(obj, &sketchpad->dsc);

    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);

    /*toolbar*/
    lv_obj_t * toolbar = lv_obj_class_create_obj(&lv_100ask_sketchpad_toolbar_class, obj);
    lv_obj_class_init_obj(toolbar);

    LV_TRACE_OBJ_CREATE("finished");
}

static void lv_100ask_sketchpad_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_canvas_t * canvas = (lv_canvas_t *)obj;
    lv_img_cache_invalidate_src(&canvas->dsc);
}


static void lv_100ask_sketchpad_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_res_t res;

    /*Call the ancestor's event handler*/
    res = lv_obj_event_base(MY_CLASS, e);
    if(res != LV_RES_OK) return;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_100ask_sketchpad_t * sketchpad = (lv_100ask_sketchpad_t *)obj;

    static lv_coord_t last_x, last_y = -32768;

    if (code == LV_EVENT_PRESSING || code == LV_EVENT_PRESSED || code == LV_EVENT_LONG_PRESSED || code == LV_EVENT_LONG_PRESSED_REPEAT || code == LV_EVENT_PRESS_LOST)
    {
        lv_indev_t * indev = lv_indev_get_act();
        if(indev == NULL)  return;

        lv_point_t point;
        lv_indev_get_point(indev, &point);

        lv_color_t c0;
        c0.full = 10;

        lv_point_t points[2];

        /*Release or first use*/
        if ((last_x == -32768) || (last_y == -32768))
        {
            last_x = point.x;
            last_y = point.y;
        }
        else
        {
            points[0].x = last_x;
            points[0].y = last_y;
            points[1].x = point.x;
            points[1].y = point.y;
            last_x = point.x;
            last_y = point.y;

            lv_canvas_draw_line(obj, points, 2, &sketchpad->line_rect_dsc);
        }
    }

    /*Loosen the brush*/
    else if(code == LV_EVENT_RELEASED)
    {
        last_x = -32768;
        last_y = -32768;
    }
}

/*toolbar*/
static void lv_100ask_sketchpad_toolbar_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(obj, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_flex_grow(obj, 1);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);

    static lv_coord_t sketchpad_toolbar_cw = LV_100ASK_SKETCHPAD_TOOLBAR_OPT_CW;
    lv_obj_t * color = lv_img_create(obj);
    lv_obj_set_align(color, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(color, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_align(color, LV_ALIGN_LEFT_MID);
    lv_img_set_src(color, &img_color);
    lv_obj_add_event_cb(color, sketchpad_toolbar_event_cb, LV_EVENT_ALL, &sketchpad_toolbar_cw);

    static lv_coord_t sketchpad_toolbar_width = LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH;
    lv_obj_t * size = lv_img_create(obj);
    lv_img_set_src(size, &img_pen);
    lv_obj_add_flag(size, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_align(size, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(size, sketchpad_toolbar_event_cb, LV_EVENT_ALL, &sketchpad_toolbar_width);

    static lv_coord_t sketchpad_toolbar_earse = LV_100ASK_SKETCHPAD_TOOLBAR_OPT_ERASE;
    lv_obj_t * erase = lv_img_create(obj);
    lv_img_set_src(erase, &img_erase);
    lv_obj_add_flag(erase, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_align(erase, LV_ALIGN_RIGHT_MID);
    lv_obj_add_event_cb(erase, sketchpad_toolbar_event_cb, LV_EVENT_ALL, &sketchpad_toolbar_earse);

    LV_TRACE_OBJ_CREATE("finished");
}


static void lv_100ask_sketchpad_toolbar_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{

}


static void lv_100ask_sketchpad_toolbar_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_res_t res;

    /*Call the ancestor's event handler*/
    res = lv_obj_event_base(MY_CLASS, e);
    if(res != LV_RES_OK) return;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSING)
    {
        lv_indev_t * indev = lv_indev_get_act();
        if(indev == NULL)  return;

        lv_point_t vect;
        lv_indev_get_vect(indev, &vect);

        lv_coord_t x = lv_obj_get_x(obj) + vect.x;
        lv_coord_t y = lv_obj_get_y(obj) + vect.y;
        lv_obj_set_pos(obj, x, y);
    }
}

static void sketchpad_toolbar_event_cb(lv_event_t * e)
{
    lv_coord_t *toolbar_opt = lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * toolbar= lv_obj_get_parent(obj);
    lv_obj_t * sketchpad= lv_obj_get_parent(toolbar);
    lv_100ask_sketchpad_t * sketchpad_t = (lv_100ask_sketchpad_t *)sketchpad;


    if (code == LV_EVENT_CLICKED)
    {
        if ((*toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_CW)
        {
            static lv_coord_t sketchpad_toolbar_cw = LV_100ASK_SKETCHPAD_TOOLBAR_OPT_CW;
            lv_obj_t * cw = lv_colorwheel_create(sketchpad, true);
            lv_obj_align_to(cw, obj, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
            lv_obj_add_event_cb(cw, toolbar_set_event_cb, LV_EVENT_RELEASED, &sketchpad_toolbar_cw);
        }
        else if((*toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH || (*toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_ERASE)
        {
            static lv_coord_t sketchpad_toolbar_width = LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH;
            lv_obj_t * slider = lv_slider_create(sketchpad);
            lv_slider_set_value(slider, (int32_t)(sketchpad_t->line_rect_dsc.width), LV_ANIM_OFF);
            lv_obj_align_to(slider, obj, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
            lv_obj_add_event_cb(slider, toolbar_set_event_cb, LV_EVENT_ALL, &sketchpad_toolbar_width);

            if((*toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_ERASE) {
                sketchpad_t->line_rect_dsc.color = lv_color_white();
            }
        }
    }
}

static void toolbar_set_event_cb(lv_event_t * e)
{
	lv_coord_t *toolbar_opt = lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_100ask_sketchpad_t * sketchpad = (lv_100ask_sketchpad_t *)lv_obj_get_parent(obj);

    if (code == LV_EVENT_RELEASED)
    {
        if ((*toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_CW)
        {
            sketchpad->line_rect_dsc.color = lv_colorwheel_get_rgb(obj);
            lv_obj_del(obj);
        }
        else if (*(toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH)
        {
            lv_obj_del(obj);
        }
        else if (*(toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_ERASE)
        {
            lv_obj_del(obj);
        }
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        if((*toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH || (*toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_ERASE)
        {
            sketchpad->line_rect_dsc.width = (lv_coord_t)lv_slider_get_value(obj);
        }
    }
}
