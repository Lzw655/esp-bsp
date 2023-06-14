#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lv_demos.h"

#include "bsp/esp-bsp.h"

static char *TAG = "app_main";

void app_main(void)
{
    ESP_LOGI(TAG, "hello world");
    bsp_i2c_init();

    bsp_display_start();

    bsp_display_lock(0);
    // lv_demo_benchmark();
    lv_demo_widgets();
    // lv_obj_t *obj = lv_obj_create(lv_scr_act());
    // lv_obj_set_style_bg_color(obj, lv_color_hex(0x001f), 0);
    // lv_obj_center(obj);
    bsp_display_unlock();
}
