/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ESP BSP: S3-LCD-EV board
 */

#pragma once

#include <sys/cdefs.h>
#include "esp_codec_dev.h"
#include "esp_err.h"
#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "driver/sdmmc_host.h"
#include "mipi_dsi.h"
#include "lvgl.h"

#include "sdkconfig.h"

/**************************************************************************************************
 *  ESP32-P4-Function-ev-board Pinout
 **************************************************************************************************/
/* I2C */
#if CONFIG_BSP_BOARD_TYPE_FLY_LINE
#define BSP_I2C_SCL             (GPIO_NUM_22)
#define BSP_I2C_SDA             (GPIO_NUM_23)
#elif CONFIG_BSP_BOARD_TYPE_FIB
#define BSP_I2C_SCL             (GPIO_NUM_8)
#define BSP_I2C_SDA             (GPIO_NUM_7)
#elif CONFIG_BSP_BOARD_TYPE_SAMPLE
#define BSP_I2C_SCL             (GPIO_NUM_34)
#define BSP_I2C_SDA             (GPIO_NUM_31)
#endif

/* I3C */
#define BSP_I3C_MST_SCL         (GPIO_NUM_32)
#define BSP_I3C_MST_SDA         (GPIO_NUM_33)
#define BSP_I3C_SLV_SCL         (GPIO_NUM_20)
#define BSP_I3C_SLV_SDA         (GPIO_NUM_21)

/* Audio */
#if CONFIG_BSP_BOARD_TYPE_FLY_LINE || CONFIG_BSP_BOARD_TYPE_SAMPLE
#define BSP_I2S_SCLK            (GPIO_NUM_29)
#define BSP_I2S_MCLK            (GPIO_NUM_30)
#define BSP_I2S_LCLK            (GPIO_NUM_27)
#define BSP_I2S_DOUT            (GPIO_NUM_26)    // To Codec ES8311
#define BSP_I2S_DSIN            (GPIO_NUM_28)   // From Codec ES8311
#elif CONFIG_BSP_BOARD_TYPE_FIB
#define BSP_I2S_SCLK            (GPIO_NUM_12)
#define BSP_I2S_MCLK            (GPIO_NUM_13)
#define BSP_I2S_LCLK            (GPIO_NUM_10)
#define BSP_I2S_DOUT            (GPIO_NUM_9)    // To Codec ES8311
#define BSP_I2S_DSIN            (GPIO_NUM_11)   // From Codec ES8311
#endif
#define BSP_POWER_AMP_IO        (GPIO_NUM_53)

/* Wireless */
#define BSP_WIRELESS_EN         (GPIO_NUM_54)
#if CONFIG_BSP_BOARD_TYPE_FLY_LINE || CONFIG_BSP_BOARD_TYPE_SAMPLE
#define BSP_WIRELESS_WKUP       (GPIO_NUM_35)
#define BSP_WIRELESS_D0         (GPIO_NUM_49)
#define BSP_WIRELESS_D1         (GPIO_NUM_50)
#elif CONFIG_BSP_BOARD_TYPE_FIB
#define BSP_WIRELESS_WKUP       (GPIO_NUM_6)
#define BSP_WIRELESS_D0         (GPIO_NUM_14)
#define BSP_WIRELESS_D1         (GPIO_NUM_15)
#endif
#define BSP_WIRELESS_D2         (GPIO_NUM_16)
#define BSP_WIRELESS_D3         (GPIO_NUM_17)
#define BSP_WIRELESS_CLK        (GPIO_NUM_18)
#define BSP_WIRELESS_CMD        (GPIO_NUM_19)

/* SD Card */
#define BSP_SD_D0               (GPIO_NUM_39)
#define BSP_SD_D1               (GPIO_NUM_40)
#define BSP_SD_D2               (GPIO_NUM_41)
#define BSP_SD_D3               (GPIO_NUM_42)
#define BSP_SD_CLK              (GPIO_NUM_43)
#define BSP_SD_CMD              (GPIO_NUM_44)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief BSP display configuration structure
 *
 */
typedef struct {
    void *dummy;    /*!< Prepared for future use. */
} bsp_display_cfg_t;

/**************************************************************************************************
 *
 * I2C Interface
 *
 * There are multiple devices connected to I2C peripheral:
 *  - Codec ES8311 (configuration only)
 *  - ADC ES7210 (configuration only)
 *  - LCD Touch controller
 *  - IO expander chip TCA9554
 *
 * After initialization of I2C, use `BSP_I2C_NUM` macro when creating I2C devices drivers ie.:
 * \code{.c}
 * es8311_handle_t es8311_dev = es8311_create(BSP_I2C_NUM, ES8311_ADDRRES_0);
 * \endcode
 *
 **************************************************************************************************/
#define BSP_I2C_NUM             (CONFIG_BSP_I2C_NUM)

/**
 * @brief Init I2C driver
 *
 * @return
 *      - ESP_OK:               On success
 *      - ESP_ERR_INVALID_ARG:  I2C parameter error
 *      - ESP_FAIL:             I2C driver installation error
 *
 */
esp_err_t bsp_i2c_init(void);

/**
 * @brief Deinit I2C driver and free its resources
 *
 * @return
 *      - ESP_OK:               On success
 *      - ESP_ERR_INVALID_ARG:  I2C parameter error
 *
 */
esp_err_t bsp_i2c_deinit(void);

/**************************************************************************************************
 *
 * SPIFFS
 *
 * After mounting the SPIFFS, it can be accessed with stdio functions ie.:
 * \code{.c}
 * FILE* f = fopen(BSP_SPIFFS_MOUNT_POINT"/hello.txt", "w");
 * fprintf(f, "Hello World!\n");
 * fclose(f);
 * \endcode
 **************************************************************************************************/
#define BSP_SPIFFS_MOUNT_POINT      CONFIG_BSP_SPIFFS_MOUNT_POINT

/**
 * @brief Mount SPIFFS to virtual file system
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if esp_vfs_spiffs_register was already called
 *      - ESP_ERR_NO_MEM if memory can not be allocated
 *      - ESP_FAIL if partition can not be mounted
 *      - other error codes
 */
esp_err_t bsp_spiffs_mount(void);

/**
 * @brief Unmount SPIFFS from virtual file system
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_NOT_FOUND if the partition table does not contain SPIFFS partition with given label
 *      - ESP_ERR_INVALID_STATE if esp_vfs_spiffs_unregister was already called
 *      - ESP_ERR_NO_MEM if memory can not be allocated
 *      - ESP_FAIL if partition can not be mounted
 *      - other error codes
 */
esp_err_t bsp_spiffs_unmount(void);

/**************************************************************************************************
 *
 * SD card
 *
 * After mounting the SD card, it can be accessed with stdio functions ie.:
 * \code{.c}
 * FILE* f = fopen(BSP_SD_MOUNT_POINT"/hello.txt", "w");
 * fprintf(f, "Hello %s!\n", bsp_sdcard->cid.name);
 * fclose(f);
 * \endcode
 *
 * @attention IO2 is also routed to RGB LED and push button
 **************************************************************************************************/
#define BSP_SD_MOUNT_POINT      CONFIG_BSP_SD_MOUNT_POINT

/**
 * @brief Mount microSD card to virtual file system
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if esp_vfs_fat_sdmmc_mount was already called
 *      - ESP_ERR_NO_MEM if memory can not be allocated
 *      - ESP_FAIL if partition can not be mounted
 *      - other error codes from SDMMC or SPI drivers, SDMMC protocol, or FATFS drivers
 */
sdmmc_card_t *bsp_sdcard_mount(void);

/**
 * @brief Unmount micorSD card from virtual file system
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_NOT_FOUND if the partition table does not contain FATFS partition with given label
 *      - ESP_ERR_INVALID_STATE if esp_vfs_fat_spiflash_mount was already called
 *      - ESP_ERR_NO_MEM if memory can not be allocated
 *      - ESP_FAIL if partition can not be mounted
 *      - other error codes from wear levelling library, SPI flash driver, or FATFS drivers
 */
esp_err_t bsp_sdcard_unmount(void);

/**************************************************************************************************
 *
 * I2S audio interface
 *
 * There are two devices connected to the I2S peripheral:
 *  - Codec ES8311 for output (playback) path
 *  - ADC ES7210 for input (recording) path
 *
 * For speaker initialization use `bsp_audio_codec_speaker_init()` which is inside initialize I2S with `bsp_audio_init()`.
 * For microphone initialization use `bsp_audio_codec_microphone_init()` which is inside initialize I2S with `bsp_audio_init()`.
 * After speaker or microphone initialization, use functions from esp_codec_dev for play/record audio.
 * Example audio play:
 * \code{.c}
 * esp_codec_dev_set_out_vol(spk_codec_dev, DEFAULT_VOLUME);
 * esp_codec_dev_open(spk_codec_dev, &fs);
 * esp_codec_dev_write(spk_codec_dev, wav_bytes, bytes_read_from_spiffs);
 * esp_codec_dev_close(spk_codec_dev);
 * \endcode
 **************************************************************************************************/
/**
 * @brief Init audio
 *
 * @note  There is no deinit audio function. Users can free audio resources by calling `i2s_del_channel()`.
 * @note  This function wiil call `bsp_io_expander_init()` to setup and enable the control pin of audio power amplifier.
 * @note  This function will be called in `bsp_audio_codec_speaker_init()` and `bsp_audio_codec_microphone_init()`.
 *
 * @param[in] i2s_config I2S configuration. Pass NULL to use default values (Mono, duplex, 16bit, 22050 Hz)
 * @return
 *      - ESP_OK                On success
 *      - ESP_ERR_NOT_SUPPORTED The communication mode is not supported on the current chip
 *      - ESP_ERR_INVALID_ARG   NULL pointer or invalid configuration
 *      - ESP_ERR_NOT_FOUND     No available I2S channel found
 *      - ESP_ERR_NO_MEM        No memory for storing the channel information
 *      - ESP_ERR_INVALID_STATE This channel has not initialized or already started
 *      - other error codes
 */
esp_err_t bsp_audio_init(const i2s_std_config_t *i2s_config);

/**
 * @brief Initialize speaker codec device
 *
 * @note  This function will call `bsp_audio_init()` if it has not been called already.
 *
 * @return Pointer to codec device handle or NULL when error occured
 */
esp_codec_dev_handle_t bsp_audio_codec_speaker_init(void);

/**
 * @brief Initialize microphone codec device
 *
 * @note  This function will call `bsp_audio_init()` if it has not been called already.
 *
 * @return Pointer to codec device handle or NULL when error occured
 */
esp_codec_dev_handle_t bsp_audio_codec_microphone_init(void);

/**
 * Display
 *
 */
/* LVGL related parameters */
#define LVGL_TICK_PERIOD_MS         (CONFIG_BSP_DISPLAY_LVGL_TICK)
#define LVGL_BUFFER_HEIGHT          (CONFIG_BSP_DISPLAY_LVGL_BUF_HEIGHT)
#if CONFIG_BSP_DISPLAY_LVGL_PSRAM
#define LVGL_BUFFER_MALLOC          (MALLOC_CAP_SPIRAM)
#else
#define LVGL_BUFFER_MALLOC          (MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT)
#endif
#define LVGL_TASK_CORE_ID           (CONFIG_BSP_DISPLAY_LVGL_TASK_CORE_ID)
#define LVGL_TASK_STACK_SIZE_KB     (CONFIG_BSP_DISPLAY_LVGL_TASK_STACK_SIZE_KB)
#define LVGL_TASK_PRIORITY          (CONFIG_BSP_DISPLAY_LVGL_TASK_PRIORITY)

#define TEST_DSI_NT35516      (0)
#define TEST_DSI_ILI9806E     (0)
#define TEST_DSI_ILI9881C     (1)
#define TEST_DSI_ST7796       (0)
#define TEST_DSI_NT35532      (0)
#define TEST_DSI_ICN6211_800  (0)
#define TEST_DSI_ICN6211_1280 (0)

#if TEST_DSI_NT35516
#define  MIPI_DSI_LINE_RATE  (800000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (60000000)

#define  MIPI_DSI_IMAGE_HSIZE           540
#define  MIPI_DSI_IMAGE_VSIZE           960

#if (TEST_DSI_COLOR_MODE==MIPI_DSI_RGB888_MODE)
#define  MIPI_DSI_IMAGE_HSYNC           200
#define  MIPI_DSI_IMAGE_HBP             1000
#define  MIPI_DSI_IMAGE_HFP             1000
#define  MIPI_DSI_IMAGE_VSYNC           16
#define  MIPI_DSI_IMAGE_VBP             32
#define  MIPI_DSI_IMAGE_VFP             32 + (960 - MIPI_DSI_IMAGE_VSIZE)
#else
#if 1
#define  MIPI_DSI_IMAGE_HSYNC           80
#define  MIPI_DSI_IMAGE_HBP             200
#define  MIPI_DSI_IMAGE_HFP             200
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             8
#define  MIPI_DSI_IMAGE_VFP             8 + (960 - MIPI_DSI_IMAGE_VSIZE)
#else
#define  MIPI_DSI_IMAGE_HSYNC           80
#define  MIPI_DSI_IMAGE_HBP             1000
#define  MIPI_DSI_IMAGE_HFP             1000
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             8
#define  MIPI_DSI_IMAGE_VFP             8 + (960 - MIPI_DSI_IMAGE_VSIZE)
#endif
#endif

#endif

#if TEST_DSI_ILI9806E
#define  MIPI_DSI_LINE_RATE  (1000000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (120000000)

#define  MIPI_DSI_IMAGE_HSIZE           480
#define  MIPI_DSI_IMAGE_VSIZE           864

#define  MIPI_DSI_IMAGE_HSYNC           1
#define  MIPI_DSI_IMAGE_HBP             2
#define  MIPI_DSI_IMAGE_HFP             2
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             8
#define  MIPI_DSI_IMAGE_VFP             8
#endif

#if TEST_DSI_ILI9881C
#define  MIPI_DSI_LINE_RATE  (1000000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (40000000)

#define  MIPI_DSI_IMAGE_HSIZE           800
#define  MIPI_DSI_IMAGE_VSIZE           1280

#define  MIPI_DSI_IMAGE_HSYNC           40
#define  MIPI_DSI_IMAGE_HBP             140
#define  MIPI_DSI_IMAGE_HFP             40
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             16
#define  MIPI_DSI_IMAGE_VFP             16
#endif

#if  TEST_DSI_ST7796
#define  MIPI_DSI_LINE_RATE  (400000000)
#define  MIPI_DPI_LINE_NUM   (1)
#define  MIPI_DPI_CLOCK_RATE (20000000)

#define  MIPI_DSI_IMAGE_HSIZE           320
#define  MIPI_DSI_IMAGE_VSIZE           480

#define  MIPI_DSI_IMAGE_HSYNC           100
#define  MIPI_DSI_IMAGE_HBP             120
#define  MIPI_DSI_IMAGE_HFP             120
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             8
#define  MIPI_DSI_IMAGE_VFP             8 + (480 - MIPI_DSI_IMAGE_VSIZE)
#endif

#if TEST_DSI_NT35532
#define  MIPI_DSI_LINE_RATE  (1000000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (64000000)

#define  MIPI_DSI_IMAGE_HSIZE           1080
#define  MIPI_DSI_IMAGE_VSIZE           1920

#define  MIPI_DSI_IMAGE_HSYNC           400
#define  MIPI_DSI_IMAGE_HBP             400
#define  MIPI_DSI_IMAGE_HFP             400
#define  MIPI_DSI_IMAGE_VSYNC           2
#define  MIPI_DSI_IMAGE_VBP             2
#define  MIPI_DSI_IMAGE_VFP             4 + (1920 - MIPI_DSI_IMAGE_VSIZE)
#endif

#if TEST_DSI_ICN6211_800
#define  MIPI_DSI_LINE_RATE  (800000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (32000000)

#define  MIPI_DSI_IMAGE_HSIZE           800
#define  MIPI_DSI_IMAGE_VSIZE           480

// HSYNC 1.407us HT 33.29us, 30.04KHz
#define  MIPI_DSI_IMAGE_HSYNC           80
#define  MIPI_DSI_IMAGE_HBP             100
#define  MIPI_DSI_IMAGE_HFP             100

// VSYNC 100us, VT 18ms, 56.88Hz
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             4
#define  MIPI_DSI_IMAGE_VFP             8
#endif

#if TEST_DSI_ICN6211_1280
#define  MIPI_DSI_LINE_RATE  (800000000)
#define  MIPI_DPI_LINE_NUM   (2)
#define  MIPI_DPI_CLOCK_RATE (80000000)

#define  MIPI_DSI_IMAGE_HSIZE           1280
#define  MIPI_DSI_IMAGE_VSIZE           800

// HSYNC 1.407us HT 33.29us, 30.04KHz
#define  MIPI_DSI_IMAGE_HSYNC           114
#define  MIPI_DSI_IMAGE_HBP             120
#define  MIPI_DSI_IMAGE_HFP             120

// VSYNC 100us, VT 18ms, 56.88Hz
#define  MIPI_DSI_IMAGE_VSYNC           4
#define  MIPI_DSI_IMAGE_VBP             4
#define  MIPI_DSI_IMAGE_VFP             8
#endif

#define DPI_HSA                MIPI_DSI_IMAGE_HSYNC
#define DPI_HBP                MIPI_DSI_IMAGE_HBP
#define DPI_HFP                MIPI_DSI_IMAGE_HFP
#define DPI_HACT               MIPI_DSI_IMAGE_HSIZE

#define DPI_VSA                MIPI_DSI_IMAGE_VSYNC
#define DPI_VBP                MIPI_DSI_IMAGE_VBP
#define DPI_VFP                MIPI_DSI_IMAGE_VFP
#define DPI_VACT               MIPI_DSI_IMAGE_VSIZE

#define MIPI_DSI_DISP_HSIZE     (MIPI_DSI_IMAGE_HSIZE)
#define MIPI_DSI_DISP_VSIZE     (MIPI_DSI_IMAGE_VSIZE)
#define MIPI_DSI_DISP_BUF_SIZE  (MIPI_DSI_DISP_HSIZE * MIPI_DSI_DISP_VSIZE * TEST_DSI_COLOR_WIDTH / 8)

#define BSP_LCD_H_RES           (MIPI_DSI_DISP_HSIZE)
#define BSP_LCD_V_RES           (MIPI_DSI_IMAGE_VSIZE)

/**
 * @brief Initialize display
 *
 * @note This function initializes display controller and starts LVGL handling task.
 * @note Users can get LCD panel handle from `user_data` in returned display.
 *
 * @return Pointer to LVGL display or NULL when error occured
 */
lv_disp_t *bsp_display_start(void);

/**
 * @brief Initialize display
 *
 * This function initializes SPI, display controller and starts LVGL handling task.
 * LCD backlight must be enabled separately by calling `bsp_display_brightness_set()`
 *
 * @param cfg display configuration
 *
 * @return Pointer to LVGL display or NULL when error occured
 */
lv_disp_t *bsp_display_start_with_config(const bsp_display_cfg_t *cfg);

/**
 * @brief Get pointer to input device (touch, buttons, ...)
 *
 * @note  The LVGL input device is initialized in `bsp_display_start()` function.
 * @note  This function should be called after calling `bsp_display_start()`.
 *
 * @return Pointer to LVGL input device or NULL when not initialized
 */
lv_indev_t *bsp_display_get_input_dev(void);

/**
 * @brief Set display's brightness (Useless, just for compatibility)
 *
 * @param[in] brightness_percent: Brightness in [%]
 * @return
 *      - ESP_ERR_NOT_SUPPORTED: Always
 */
esp_err_t bsp_display_brightness_set(int brightness_percent);

/**
 * @brief Turn on display backlight (Useless, just for compatibility)
 *
 * @return
 *      - ESP_ERR_NOT_SUPPORTED: Always
 */
esp_err_t bsp_display_backlight_on(void);

/**
 * @brief Turn off display backlight (Useless, just for compatibility)
 *
 * @return
 *      - ESP_ERR_NOT_SUPPORTED: Always
 */
esp_err_t bsp_display_backlight_off(void);

/**
 * @brief Take LVGL mutex
 *
 * @note  Display must be already initialized by calling `bsp_display_start()`
 *
 * @param[in] timeout_ms: Timeout in [ms]. 0 will block indefinitely.
 *
 * @return
 *      - true:  Mutex was taken
 *      - false: Mutex was NOT taken
 */
bool bsp_display_lock(uint32_t timeout_ms);

/**
 * @brief Give LVGL mutex
 *
 * @note  Display must be already initialized by calling `bsp_display_start()`
 *
 */
void bsp_display_unlock(void);

#ifdef __cplusplus
}
#endif
