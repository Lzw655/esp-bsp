#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_memory_utils.h"
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

#define LOG_SYSTEM_INFO         (1)
#define LOG_MIPI_FRAME          (0)
#define LOG_TIME_INTERVAL_MS    (2000)

static const char *TAG = "main";

esp_err_t print_real_time_mem_stats(void);

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
    // eui->data()->data.home.app_table.screen_show_default = 0;
    // eui->data()->data.enable_disp_wallpaper = false;
    eui->data()->data.disp_background_color = lv_color_hex(0x0091ff);
    eui->data()->data.home.app_table.main_text_color = lv_color_white();
    // eui->enableDebugMode();
    eui->enableGesture(bsp_display_get_input_dev());
    eui->begin();
    // eui->printFormatData();

    eui->installApp(new LVGLDemos(false, true));
    eui->installApp(new SmartGadget(false, true));
    // eui->installApp(new MusicPlayer(false, true));
    // eui->installApp(new Camera(MIPI_CSI_IMAGE_HSIZE, MIPI_CSI_IMAGE_VSIZE, 0, false, true));
    eui->installApp(new Calculator());
    eui->installApp(new Game2048());
    eui->installApp(new Memory_game());
    eui->installApp(new Setting());
    eui->installApp(new Sketchpad());
    eui->installApp(new WeatherForecast());

    bsp_display_unlock();

    static char buffer[2048];
    while (1) {
#if LOG_SYSTEM_INFO
        sprintf(buffer, "\t  Biggest /     Free /    Total\n"
                " SRAM : [%8d / %8d / %8d]\n"
                "PSRAM : [%8d / %8d / %8d]\n",
                heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
                heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                heap_caps_get_total_size(MALLOC_CAP_INTERNAL),
                heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM),
                heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
                heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
        printf("------------ Memory ------------\n");
        printf("%s\n", buffer);

        ESP_ERROR_CHECK(print_real_time_mem_stats());
        printf("\n");
#endif

#if LOG_MIPI_FRAME
        ESP_LOGI(TAG, "DSI: %d, CSI: %d", dw_gdma_mipi_dsi_get_frame_count(), dw_gdma_mipi_csi_get_frame_count());
#endif

        vTaskDelay(pdMS_TO_TICKS(LOG_TIME_INTERVAL_MS));
    }
}

#if LOG_SYSTEM_INFO
#define ARRAY_SIZE_OFFSET                   8   // Increase this if audio_sys_get_real_time_stats returns ESP_ERR_INVALID_SIZE
#define AUDIO_SYS_TASKS_ELAPSED_TIME_MS  1000   // Period of stats measurement

#define audio_malloc    malloc
#define audio_calloc    calloc
#define audio_free      free
#define AUDIO_MEM_CHECK(tag, x, action) if (x == NULL) { \
        ESP_LOGE(tag, "Memory exhausted (%s:%d)", __FILE__, __LINE__); \
        action; \
    }

const char *task_state[] = {
    "Running",
    "Ready",
    "Blocked",
    "Suspended",
    "Deleted"
};

/** @brief
 * "Extr": Allocated task stack from psram, "Intr": Allocated task stack from internel
 */
const char *task_stack[] = {"Extr", "Intr"};

esp_err_t print_real_time_mem_stats(void)
{
#if (CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID && CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS)
    TaskStatus_t *start_array = NULL, *end_array = NULL;
    UBaseType_t start_array_size, end_array_size;
    uint32_t start_run_time, end_run_time;
    uint32_t total_elapsed_time;
    uint32_t task_elapsed_time, percentage_time;
    esp_err_t ret;

    // Allocate array to store current task states
    start_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    start_array = (TaskStatus_t *)audio_malloc(sizeof(TaskStatus_t) * start_array_size);
    AUDIO_MEM_CHECK(TAG, start_array, {
        ret = ESP_FAIL;
        goto exit;
    });
    // Get current task states
    start_array_size = uxTaskGetSystemState(start_array, start_array_size, &start_run_time);
    if (start_array_size == 0) {
        ESP_LOGE(TAG, "Insufficient array size for uxTaskGetSystemState. Trying increasing ARRAY_SIZE_OFFSET");
        ret = ESP_FAIL;
        goto exit;
    }

    vTaskDelay(pdMS_TO_TICKS(AUDIO_SYS_TASKS_ELAPSED_TIME_MS));

    // Allocate array to store tasks states post delay
    end_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    end_array = (TaskStatus_t *)audio_malloc(sizeof(TaskStatus_t) * end_array_size);
    AUDIO_MEM_CHECK(TAG, start_array, {
        ret = ESP_FAIL;
        goto exit;
    });

    // Get post delay task states
    end_array_size = uxTaskGetSystemState(end_array, end_array_size, &end_run_time);
    if (end_array_size == 0) {
        ESP_LOGE(TAG, "Insufficient array size for uxTaskGetSystemState. Trying increasing ARRAY_SIZE_OFFSET");
        ret = ESP_FAIL;
        goto exit;
    }

    // Calculate total_elapsed_time in units of run time stats clock period.
    total_elapsed_time = (end_run_time - start_run_time);
    if (total_elapsed_time == 0) {
        ESP_LOGE(TAG, "Delay duration too short. Trying increasing AUDIO_SYS_TASKS_ELAPSED_TIME_MS");
        ret = ESP_FAIL;
        goto exit;
    }

    ESP_LOGI(TAG, "| Task              | Run Time    | Per | Prio | HWM       | State   | CoreId   | Stack ");

    // Match each task in start_array to those in the end_array
    for (int i = 0; i < start_array_size; i++) {
        for (int j = 0; j < end_array_size; j++) {
            if (start_array[i].xHandle == end_array[j].xHandle) {

                task_elapsed_time = end_array[j].ulRunTimeCounter - start_array[i].ulRunTimeCounter;
                percentage_time = (task_elapsed_time * 100UL) / (total_elapsed_time * portNUM_PROCESSORS);
                ESP_LOGI(TAG, "| %-17s | %-11d |%2d%%  | %-4u | %-9u | %-7s | %-8x | %s",
                                start_array[i].pcTaskName, (int)task_elapsed_time, (int)percentage_time, start_array[i].uxCurrentPriority,
                                (int)start_array[i].usStackHighWaterMark, task_state[(start_array[i].eCurrentState)],
                                start_array[i].xCoreID, task_stack[esp_ptr_internal(pxTaskGetStackStart(start_array[i].xHandle))]);

                // Mark that task have been matched by overwriting their handles
                start_array[i].xHandle = NULL;
                end_array[j].xHandle = NULL;
                break;
            }
        }
    }

    // Print unmatched tasks
    for (int i = 0; i < start_array_size; i++) {
        if (start_array[i].xHandle != NULL) {
            ESP_LOGI(TAG, "| %s | Deleted", start_array[i].pcTaskName);
        }
    }
    for (int i = 0; i < end_array_size; i++) {
        if (end_array[i].xHandle != NULL) {
            ESP_LOGI(TAG, "| %s | Created", end_array[i].pcTaskName);
        }
    }
    printf("\n");
    ret = ESP_OK;

exit:    // Common return path
    if (start_array) {
        audio_free(start_array);
        start_array = NULL;
    }
    if (end_array) {
        audio_free(end_array);
        end_array = NULL;
    }
    return ret;
#else
    ESP_LOGW(TAG, "Please enbale `CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID` and `CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS` in menuconfig");
    return ESP_FAIL;
#endif
}
#endif
