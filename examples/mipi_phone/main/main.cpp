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
#include "ESP_UI.h"
#include "apps.h"

/* Print log about SRAM and PSRAM memory */
#define LOG_MEM_INFO    (1)

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

    bsp_display_start();
    bsp_display_lock(0);

    ESP_UI *eui = new ESP_UI();
    assert(eui != NULL);

    MusicPlayer *music_player = new MusicPlayer();

    eui->init();
    eui->data()->data.home.app_table.screen_num = 1;
    eui->data()->data.home.app_table.screen_show_default = 0;
    // eui->data()->data.enable_disp_wallpaper = false;
    eui->data()->data.disp_background_color = lv_color_hex(0x0091ff);
    // eui->enableDebugMode();
    eui->enableGesture(bsp_display_get_input_dev());
    eui->begin();
    eui->installApp(*music_player);
    // eui->printFormatData();

    bsp_display_unlock();

#if LOG_MEM_INFO
    static char buffer[128];    /* Make sure buffer is enough for `sprintf` */
    while (1) {
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

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
#endif
}
