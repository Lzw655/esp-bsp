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
#include "bsp/display.h"
#include "bsp_lcd.h"

#include "ESP_UI.h"
#include "ESP_UI_APP.h"
#include "mipi_csi.h"
#include "dw_gdma.h"
#include "apps.h"

#define LOG_MEM_INFO            (0)
#define LOG_MIPI_FRAME          (0)
#define LOG_TIME_INTERVAL_MS    (10000)

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

    bsp_display_start();

    bsp_display_lock(0);

    ESP_UI *eui = new ESP_UI();
    assert(eui != NULL);

    eui->init();
    eui->data()->data.home.app_table.screen_num = 1;
    eui->data()->data.home.app_table.screen_show_default = 0;
    // eui->data()->data.enable_disp_wallpaper = false;
    // eui->data()->data.disp_background_color = lv_color_hex(0x0091ff);
    eui->data()->data.home.app_table.main_text_color = lv_color_white();
    // eui->enableDebugMode();
    eui->enableGesture(bsp_display_get_input_dev());
    eui->begin();
    // eui->printFormatData();

    eui->installApp(new LVGLDemos());
    eui->installApp(new SmartGadget());
    eui->installApp(new MusicPlayer());
    eui->installApp(new Camera(MIPI_CSI_IMAGE_HSIZE, MIPI_CSI_IMAGE_VSIZE, 0));

    bsp_display_unlock();

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

#if LOG_MIPI_FRAME
        ESP_LOGI(TAG, "DSI: %d, CSI: %d", dw_gdma_mipi_dsi_get_frame_count(), dw_gdma_mipi_csi_get_frame_count());
#endif

        vTaskDelay(pdMS_TO_TICKS(LOG_TIME_INTERVAL_MS));
    }
}
