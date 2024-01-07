/*
 * SPDX-FileCopyrightText: 2021-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "esp_lcd_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t dpi_clk_hz;          /*!< Frequency of pixel clock */
    uint32_t dsi_line_hz;           /*!< Frequency of pixel clock */
    uint8_t dsi_line_num;
    uint32_t h_res;             /*!< Horizontal resolution, i.e. the number of pixels in a line */
    uint32_t v_res;             /*!< Vertical resolution, i.e. the number of lines in the frame  */
    uint32_t hsync_sync_active;      /*!< Horizontal sync width, unit: PCLK period */
    uint32_t hsync_back_porch;  /*!< Horizontal back porch, number of PCLK between hsync and start of line active data */
    uint32_t hsync_front_porch; /*!< Horizontal front porch, number of PCLK between the end of active data and the next hsync */
    uint32_t vsync_sync_active;      /*!< Vertical sync width, unit: number of lines */
    uint32_t vsync_back_porch;  /*!< Vertical back porch, number of invalid lines between vsync and start of frame */
    uint32_t vsync_front_porch; /*!< Vertical front porch, number of invalid lines between the end of frame and the next vsync */
    // struct {
    //     uint32_t hsync_idle_low: 1;  /*!< The hsync signal is low in IDLE state */
    //     uint32_t vsync_idle_low: 1;  /*!< The vsync signal is low in IDLE state */
    //     uint32_t de_idle_high: 1;    /*!< The de signal is high in IDLE state */
    //     uint32_t pclk_active_neg: 1; /*!< Whether the display data is clocked out on the falling edge of PCLK */
    //     uint32_t pclk_idle_high: 1;  /*!< The PCLK stays at high level in IDLE phase */
    // } flags;                         /*!< LCD RGB timing flags */
} esp_lcd_dsi_timing_t;

/**
 * @brief Type of RGB LCD panel event data
 */
typedef struct {
} esp_lcd_dsi_panel_event_data_t;

/**
 * @brief RGB LCD VSYNC event callback prototype
 *
 * @param[in] panel LCD panel handle, returned from `esp_lcd_new_rgb_panel`
 * @param[in] edata Panel event data, fed by driver
 * @param[in] user_ctx User data, passed from `esp_lcd_dsi_panel_register_event_callbacks()`
 * @return Whether a high priority task has been waken up by this function
 */
typedef bool (*esp_lcd_dsi_panel_vsync_cb_t)(esp_lcd_panel_handle_t panel, const esp_lcd_dsi_panel_event_data_t *edata, void *user_ctx);

/**
 * @brief Prototype for function to re-fill a bounce buffer, rather than copying from the frame buffer
 *
 * @param[in] panel LCD panel handle, returned from `esp_lcd_new_rgb_panel`
 * @param[in] bounce_buf Bounce buffer to write data into
 * @param[in] pos_px How many pixels already were sent to the display in this frame, in other words,
 *                   at what pixel the routine should start putting data into bounce_buf
 * @param[in] len_bytes Length, in bytes, of the bounce buffer. Routine should fill this length fully.
 * @param[in] user_ctx Opaque pointer that was passed from `esp_lcd_dsi_panel_register_event_callbacks()`
 * @return Whether a high priority task has been waken up by this function
 */
typedef bool (*esp_lcd_dsi_panel_bounce_buf_fill_cb_t)(esp_lcd_panel_handle_t panel, void *bounce_buf, int pos_px, int len_bytes, void *user_ctx);

/**
 * @brief Prototype for the function to be called when the bounce buffer finish copying the entire frame.
 *
 * @param[in] panel LCD panel handle, returned from `esp_lcd_new_rgb_panel`
 * @param[in] edata Panel event data, fed by driver
 * @param[in] user_ctx User data, passed from `esp_lcd_dsi_panel_register_event_callbacks()`
 * @return Whether a high priority task has been waken up by this function
 */
typedef bool (*esp_lcd_dsi_panel_bounce_buf_finish_cb_t)(esp_lcd_panel_handle_t panel, const esp_lcd_dsi_panel_event_data_t *edata, void *user_ctx);

/**
 * @brief Group of supported RGB LCD panel callbacks
 * @note The callbacks are all running under ISR environment
 * @note When CONFIG_LCD_DSI_ISR_IRAM_SAFE is enabled, the callback itself and functions called by it should be placed in IRAM.
 */
typedef struct {
    esp_lcd_dsi_panel_vsync_cb_t on_vsync;                      /*!< VSYNC event callback */
} esp_lcd_dsi_panel_event_callbacks_t;

/**
 * @brief LCD RGB panel configuration structure
 */
typedef struct {
    // lcd_clock_source_t clk_src;   /*!< Clock source for the RGB LCD peripheral */
    esp_lcd_dsi_timing_t timings; /*!< RGB timing parameters, including the screen resolution */
    // size_t data_width;            /*!< Number of data lines */
    size_t bits_per_pixel;        /*!< Frame buffer color depth, in bpp, specially, if set to zero, it will default to `data_width`.
    //                                    When using a Serial RGB interface, this value could be different from `data_width` */
    size_t num_fbs;               /*!< Number of screen-sized frame buffers that allocated by the driver. By default (set to either 0 or 1) only one frame buffer will be used. Maximum number of buffers are 3 */
    size_t sram_trans_align;      /*!< Alignment of buffers (frame buffer or bounce buffer) that allocated in SRAM */
    size_t psram_trans_align;     /*!< Alignment of buffers (frame buffer) that allocated in PSRAM */
    struct {
        uint32_t fb_in_psram: 1;         /*!< If this flag is enabled, the frame buffer will be allocated from PSRAM, preferentially */
    } flags;                             /*!< LCD RGB panel configuration flags */
} esp_lcd_dsi_panel_config_t;

/**
 * @brief Create RGB LCD panel
 *
 * @param[in] rgb_panel_config RGB panel configuration
 * @param[out] ret_panel Returned LCD panel handle
 * @return
 *      - ESP_ERR_INVALID_ARG: Create RGB LCD panel failed because of invalid argument
 *      - ESP_ERR_NO_MEM: Create RGB LCD panel failed because of out of memory
 *      - ESP_ERR_NOT_FOUND: Create RGB LCD panel failed because some mandatory hardware resources are not found
 *      - ESP_OK: Create RGB LCD panel successfully
 */
esp_err_t esp_lcd_new_dsi_panel(const esp_lcd_dsi_panel_config_t *dsi_panel_config, esp_lcd_panel_handle_t *ret_panel);

/**
 * @brief Register LCD RGB panel event callbacks
 *
 * @param[in] panel LCD panel handle, returned from `esp_lcd_new_rgb_panel`
 * @param[in] callbacks Group of callback functions
 * @param[in] user_ctx User data, which will be passed to the callback functions directly
 * @return
 *      - ESP_OK: Set event callbacks successfully
 *      - ESP_ERR_INVALID_ARG: Set event callbacks failed because of invalid argument
 *      - ESP_FAIL: Set event callbacks failed because of other error
 */
esp_err_t esp_lcd_dsi_panel_register_event_callbacks(esp_lcd_panel_handle_t panel, const esp_lcd_dsi_panel_event_callbacks_t *callbacks, void *user_ctx);

/**
 * @brief Get the address of the frame buffer(s) that allocated by the driver
 *
 * @param[in] panel LCD panel handle, returned from `esp_lcd_new_rgb_panel`
 * @param[in] fb_num Number of frame buffer(s) to get. This value must be the same as the number of the following parameters.
 * @param[out] fb0 Returned address of the frame buffer 0
 * @param[out] ... List of other frame buffer addresses
 * @return
 *      - ESP_ERR_INVALID_ARG: Get frame buffer address failed because of invalid argument
 *      - ESP_OK: Get frame buffer address successfully
 */
esp_err_t esp_lcd_dsi_panel_get_frame_buffer(esp_lcd_panel_handle_t panel, uint32_t fb_num, void **fb0, ...);

#ifdef __cplusplus
}
#endif
