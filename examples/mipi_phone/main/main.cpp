#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "lvgl.h"
#include "bsp/esp-bsp.h"
#include "bsp/bsp_board_extra.h"
#include "bsp/camera.h"
#include "bsp/display.h"
#include "bsp_lcd.h"
#include "dw_gdma.h"
#include "ESP_UI.h"
#include "apps.h"

#define LOG_MEM_INFO            (0)
#define LOG_MIPI_FRAME          (0)
#define LOG_TIME_INTERVAL_MS    (30)

static const char *TAG = "main";

extern "C" void app_main(void)
{
#if CONFIG_EXAMPLE_USE_SD_CARD
    if (bsp_sdcard_mount() == NULL) {
        ESP_LOGE(TAG, "SD card mount failed");
        assert(0);
    }
    ESP_LOGI(TAG, "SD card mount successfully");
#else
    ESP_ERROR_CHECK(bsp_spiffs_mount());
    ESP_LOGI(TAG, "SPIFFS mount successfully");
#endif

    ESP_ERROR_CHECK(bsp_extra_codec_init());
#if CONFIG_EXAMPLE_USE_SD_CARD
    ESP_ERROR_CHECK(bsp_extra_player_init(BSP_SD_MOUNT_POINT"/music"));
#else
    ESP_ERROR_CHECK(bsp_extra_player_init(BSP_SPIFFS_MOUNT_POINT"/music"));
#endif

    size_t frame_buffer_size = MIPI_DSI_DISP_HSIZE * 960 * sizeof(lv_color_t);
    uint8_t *frame_buffer = (uint8_t *)heap_caps_aligned_alloc(64, frame_buffer_size, MALLOC_CAP_SPIRAM);
    assert(frame_buffer != NULL);

    bsp_camera_config_t camera_cfg = {
        .hor_res = MIPI_DSI_DISP_HSIZE,
        .ver_res = 960,
        .buffer_ptr = (void **)&frame_buffer,
        .buffer_size_ptr = &frame_buffer_size,
    };
    ESP_ERROR_CHECK(bsp_camera_new(&camera_cfg));

    bsp_display_start();
    bsp_display_lock(0);

    static lv_img_dsc_t img_dsc = {
        .header = {
            .cf = LV_IMG_CF_TRUE_COLOR,
            .always_zero = 0,
            .reserved = 0,
            .w = camera_cfg.hor_res,
            .h = camera_cfg.ver_res,
        },
        .data_size = frame_buffer_size,
        .data = (const uint8_t *)frame_buffer,
    };
    lv_obj_t *img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, &img_dsc);

    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, 0);

    // ESP_UI *eui = new ESP_UI();
    // assert(eui != NULL);

    // MusicPlayer *music_player = new MusicPlayer();

    // eui->init();
    // eui->data()->data.home.app_table.screen_num = 1;
    // eui->data()->data.home.app_table.screen_show_default = 0;
    // // eui->data()->data.enable_disp_wallpaper = false;
    // eui->data()->data.disp_background_color = lv_color_hex(0x0091ff);
    // // eui->enableDebugMode();
    // eui->enableGesture(bsp_display_get_input_dev());
    // eui->begin();
    // eui->installApp(*music_player);
    // eui->printFormatData();

    bsp_display_unlock();

    // void *frame_buffer = NULL;
    // size_t frame_buffer_size = MIPI_DSI_DISP_BUF_SIZE / 2;
    // // size_t frame_buffer_size = 0;
    // ESP_ERROR_CHECK(bsp_display_new(NULL, NULL, NULL));
    // ESP_ERROR_CHECK(bsp_lcd_get_frame_buffer(1, &frame_buffer));

    // bsp_camera_config_t camera_cfg = {
    //     .hor_res = MIPI_DSI_DISP_HSIZE,
    //     .ver_res = MIPI_DSI_DISP_VSIZE / 2,
    //     .buffer_ptr = (void **)&frame_buffer,
    //     .buffer_size_ptr = &frame_buffer_size,
    // };
    // ESP_ERROR_CHECK(bsp_camera_new(&camera_cfg));

    while (1) {
#if LOG_MEM_INFO
        static char buffer[128];    /* Make sure buffer is enough for `sprintf` */
        /**
         * It's not recommended to frequently use functions like `heap_caps_get_free_size()` to obtain memory information
         * in practical applications, especially when the application extensively uses `malloc()` to dynamically allocate
         * a significant number of memory blocks. The frequent interrupt disabling may potentially lead to issues with other functionalities.
         */
        sprintf(buffer, "   Biggest /     Free /    Total\n"
                "\t  SRAM : [%8d / %8d / %8d]\n"
                "\t PSRAM : [%8d / %8d / %8d]",
                heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
                heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                heap_caps_get_total_size(MALLOC_CAP_INTERNAL),
                heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM),
                heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
                heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
        ESP_LOGI("MEM", "%s", buffer);
#endif

    bsp_display_lock(0);
    lv_obj_invalidate(img);
    bsp_display_unlock();

#if LOG_MIPI_FRAME
        ESP_LOGI(TAG, "DSI: %d, CSI: %d", dw_gdma_mipi_dsi_get_frame_count(), dw_gdma_mipi_csi_get_frame_count());
#endif

        vTaskDelay(pdMS_TO_TICKS(LOG_TIME_INTERVAL_MS));
    }
}
