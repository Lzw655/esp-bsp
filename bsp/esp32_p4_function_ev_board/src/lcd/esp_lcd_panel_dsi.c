/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdarg.h>
#include <sys/cdefs.h>
#include <sys/param.h>
#include <string.h>
#include "sdkconfig.h"
#if CONFIG_LCD_DSI_ENABLE_DEBUG_LOG
// The local log level must be defined before including esp_log.h
// Set the maximum log level for this source file
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#endif
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "esp_check.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_interface.h"
#include "esp_intr_alloc.h"
#include "esp_psram.h"
#include "esp_cache.h"
#include "soc/soc_caps.h"
#include "rom/cache.h"

#include "hp_sys_clkrst_reg.h"
#include "mipi_dsi_host_struct.h"
#include "mipi_dsi.h"
#include "bsp_lcd.h"
#include "dw_gdma.h"

#include "esp_lcd_panel_dsi.h"

#define DSI_LCD_PANEL_MAX_FB_NUM         3 // maximum supported frame buffer number

#define DSI_PANEL_SWAP_XY  0
#define DSI_PANEL_MIRROR_Y 1
#define DSI_PANEL_MIRROR_X 2

typedef enum {
    ROTATE_MASK_SWAP_XY = BIT(DSI_PANEL_SWAP_XY),
    ROTATE_MASK_MIRROR_Y = BIT(DSI_PANEL_MIRROR_Y),
    ROTATE_MASK_MIRROR_X = BIT(DSI_PANEL_MIRROR_X),
} panel_rotate_mask_t;

static const char *TAG = "lcd_panel.dsi";

typedef struct esp_dsi_panel_t esp_dsi_panel_t;

static esp_err_t dsi_panel_del(esp_lcd_panel_t *panel);
static esp_err_t dsi_panel_reset(esp_lcd_panel_t *panel);
static esp_err_t dsi_panel_init(esp_lcd_panel_t *panel);
static esp_err_t dsi_panel_draw_bitmap(esp_lcd_panel_t *panel, int x_start, int y_start, int x_end, int y_end, const void *color_data);
static esp_err_t dsi_panel_invert_color(esp_lcd_panel_t *panel, bool invert_color_data);
static esp_err_t dsi_panel_mirror(esp_lcd_panel_t *panel, bool mirror_x, bool mirror_y);
static esp_err_t dsi_panel_swap_xy(esp_lcd_panel_t *panel, bool swap_axes);
static esp_err_t dsi_panel_set_gap(esp_lcd_panel_t *panel, int x_gap, int y_gap);
static esp_err_t dsi_panel_disp_on_off(esp_lcd_panel_t *panel, bool off);

static void lcd_default_isr_handler(void *args);

struct esp_dsi_panel_t {
    esp_lcd_panel_t base;  // Base class of generic lcd panel
    size_t num_fbs;           // Number of frame buffers
    size_t fb_bits_per_pixel; // Frame buffer color depth, in bpp
    size_t sram_trans_align;      /*!< Alignment of buffers (frame buffer or bounce buffer) that allocated in SRAM */
    size_t psram_trans_align;     /*!< Alignment of buffers (frame buffer) that allocated in PSRAM */
    esp_lcd_dsi_timing_t timings;   // RGB timing parameters (e.g. pclk, sync pulse, porch width)
    uint8_t *fbs[DSI_LCD_PANEL_MAX_FB_NUM]; // Frame buffers
    uint8_t cur_fb_index;  // Current frame buffer index
    size_t fb_size;        // Size of frame buffer
    // esp_lcd_dsi_timing_t timings;   // DSI timing parameters (e.g. pclk, sync pulse, porch width)
    esp_lcd_dsi_panel_vsync_cb_t on_vsync; // VSYNC event callback
    void *user_ctx;                 // Reserved user's data of callback functions
    int x_gap;                      // Extra gap in x coordinate, it's used when calculate the flush window
    int y_gap;                      // Extra gap in y coordinate, it's used when calculate the flush window
    int rotate_mask;                // panel rotate_mask mask, Or'ed of `panel_rotate_mask_t`
    struct {
        //     uint32_t disp_en_level: 1;       // The level which can turn on the screen by `disp_gpio_num`
        //     uint32_t stream_mode: 1;         // If set, the LCD transfers data continuously, otherwise, it stops refreshing the LCD when transaction done
        uint32_t fb_in_psram: 1;         // Whether the frame buffer is in PSRAM
        //     uint32_t need_update_pclk: 1;    // Whether to update the PCLK before start a new transaction
        //     uint32_t need_restart: 1;        // Whether to restart the LCD controller and the DMA
        //     uint32_t bb_invalidate_cache: 1; // Whether to do cache invalidation in bounce buffer mode
    } flags;
};

static esp_err_t lcd_dsi_panel_alloc_frame_buffers(const esp_lcd_dsi_panel_config_t *dsi_panel_config, esp_dsi_panel_t *dsi_panel)
{
    bool fb_in_psram = false;
    size_t psram_trans_align = dsi_panel_config->psram_trans_align ? dsi_panel_config->psram_trans_align : 64;
    size_t sram_trans_align = dsi_panel_config->sram_trans_align ? dsi_panel_config->sram_trans_align : 4;
    dsi_panel->psram_trans_align = psram_trans_align;
    dsi_panel->sram_trans_align = sram_trans_align;

    // alloc frame buffer
    if (dsi_panel->num_fbs > 0) {
        // fb_in_psram is only an option, if there's no PSRAM on board, we fallback to alloc from SRAM
        if (dsi_panel_config->flags.fb_in_psram) {
#if CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
            if (esp_psram_is_initialized()) {
                fb_in_psram = true;
            }
#endif
        }
        for (int i = 0; i < dsi_panel->num_fbs; i++) {
            if (fb_in_psram) {
                // the low level malloc function will help check the validation of alignment
                dsi_panel->fbs[i] = heap_caps_aligned_calloc(psram_trans_align, 1, dsi_panel->fb_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
            } else {
                dsi_panel->fbs[i] = heap_caps_aligned_calloc(sram_trans_align, 1, dsi_panel->fb_size, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
            }
            ESP_RETURN_ON_FALSE(dsi_panel->fbs[i], ESP_ERR_NO_MEM, TAG, "no mem for frame buffer");
        }
    }

    dsi_panel->cur_fb_index = 0;
    dsi_panel->flags.fb_in_psram = fb_in_psram;

    return ESP_OK;
}

static esp_err_t lcd_dsi_panel_destory(esp_dsi_panel_t *dsi_panel)
{
    for (size_t i = 0; i < dsi_panel->num_fbs; i++) {
        if (dsi_panel->fbs[i]) {
            free(dsi_panel->fbs[i]);
        }
    }
    free(dsi_panel);
    return ESP_OK;
}

esp_err_t esp_lcd_new_dsi_panel(const esp_lcd_dsi_panel_config_t *dsi_panel_config, esp_lcd_panel_handle_t *ret_panel)
{
#if CONFIG_LCD_DSI_ENABLE_DEBUG_LOG
    esp_log_level_set(TAG, ESP_LOG_DEBUG);
#endif
    esp_err_t ret = ESP_OK;
    esp_dsi_panel_t *dsi_panel = NULL;
    ESP_RETURN_ON_FALSE(dsi_panel_config && ret_panel, ESP_ERR_INVALID_ARG, TAG, "invalid parameter");
    ESP_RETURN_ON_FALSE(dsi_panel_config->num_fbs <= DSI_LCD_PANEL_MAX_FB_NUM, ESP_ERR_INVALID_ARG, TAG, "too many frame buffers");

    size_t fb_bits_per_pixel = dsi_panel_config->bits_per_pixel;
    // calculate buffer size
    size_t fb_size = dsi_panel_config->timings.h_res * dsi_panel_config->timings.v_res * fb_bits_per_pixel / 8;

    // DMA descriptors must be placed in internal SRAM (requested by DMA)
    dsi_panel = heap_caps_calloc(1, sizeof(esp_dsi_panel_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    ESP_GOTO_ON_FALSE(dsi_panel, ESP_ERR_NO_MEM, err, TAG, "no mem for dsi panel");
    dsi_panel->num_fbs = dsi_panel_config->num_fbs;
    dsi_panel->fb_size = fb_size;

    // allocate frame buffers + bounce buffers
    ESP_GOTO_ON_ERROR(lcd_dsi_panel_alloc_frame_buffers(dsi_panel_config, dsi_panel), err, TAG, "alloc frame buffers failed");

    mipi_dsi_timing_t dsi_timing = {
        .dpi_hsa = dsi_panel_config->timings.hsync_sync_active,
        .dpi_hbp = dsi_panel_config->timings.hsync_back_porch,
        .dpi_hfp = dsi_panel_config->timings.hsync_front_porch,
        .dpi_hact = dsi_panel_config->timings.h_res,
        .dpi_vsa = dsi_panel_config->timings.vsync_sync_active,
        .dpi_vbp = dsi_panel_config->timings.vsync_back_porch,
        .dpi_vfp = dsi_panel_config->timings.vsync_front_porch,
        .dpi_vact = dsi_panel_config->timings.v_res,
        .dpi_clock_rate = dsi_panel_config->timings.dpi_clk_hz,
        .dsi_line_rate = dsi_panel_config->timings.dsi_line_hz,
        .dsi_line_num = dsi_panel_config->timings.dsi_line_num,
    };
    mipi_dsi_set_timing(&dsi_timing);
    mipi_dsi_clock_init();
    mipi_dsi_host_phy_init();

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_DMA2D_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_DMA2D_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_DMA2D);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_DMA2D);

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_PPA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_PPA_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_PPA);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_PPA);

    ESP_RETURN_ON_ERROR(dw_gdma_mipi_dsi_init(dsi_panel->fbs[0], dsi_panel->fb_size, dsi_panel->psram_trans_align), TAG,
                        "Initialize GDMA for MIPI DSI failed");

    mipi_dsi_bridge_init();

    mipi_dsih_hal_gen_cmd_mode_en(1);

#if !TEST_DSI_LOOPBACK
    MIPI_DSI_HOST.lpclk_ctrl.val = 0x00000000 |
                                   ((0 & 0x00000001) << 1) | // bit[    1] AUTO_CLKLANE_CTRL     = 0 (Automatic mechanism to stop providing clock in the clock lane when time allows.
                                   ((0 & 0x00000001) << 0);  // bit[    0] TXREQUESTCLKHS        = 1 (Requesting High Speed Clock Transmission.
    // MIPI_DSI_HOST.phy_ulps_ctrl.phy_txrequlpsclk = 1;
#endif

    dsi_panel->timings = dsi_panel_config->timings;
    dsi_panel->fb_bits_per_pixel = fb_bits_per_pixel;
    // fill function table
    dsi_panel->base.del = dsi_panel_del;
    dsi_panel->base.reset = dsi_panel_reset;
    dsi_panel->base.init = dsi_panel_init;
    dsi_panel->base.draw_bitmap = dsi_panel_draw_bitmap;
    dsi_panel->base.disp_on_off = dsi_panel_disp_on_off;
    dsi_panel->base.invert_color = dsi_panel_invert_color;
    dsi_panel->base.mirror = dsi_panel_mirror;
    dsi_panel->base.swap_xy = dsi_panel_swap_xy;
    dsi_panel->base.set_gap = dsi_panel_set_gap;
    // return base class
    *ret_panel = &(dsi_panel->base);

    ESP_LOGI(TAG, "new dsi panel @%p, num_fbs=%zu, fb_size=%zu", dsi_panel, dsi_panel->num_fbs, dsi_panel->fb_size);
    for (size_t i = 0; i < dsi_panel->num_fbs; i++) {
        ESP_LOGI(TAG, "fb[%zu] @%p", i, dsi_panel->fbs[i]);
    }
    return ESP_OK;

err:
    if (dsi_panel) {
        lcd_dsi_panel_destory(dsi_panel);
    }
    return ret;
}

esp_err_t esp_lcd_dsi_panel_register_event_callbacks(esp_lcd_panel_handle_t panel, const esp_lcd_dsi_panel_event_callbacks_t *callbacks, void *user_ctx)
{
    ESP_RETURN_ON_FALSE(panel && callbacks, ESP_ERR_INVALID_ARG, TAG, "invalid argument");
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);
#if CONFIG_LCD_DSI_ISR_IRAM_SAFE
    if (callbacks->on_vsync) {
        ESP_RETURN_ON_FALSE(esp_ptr_in_iram(callbacks->on_vsync), ESP_ERR_INVALID_ARG, TAG, "on_vsync callback not in IRAM");
    }
    if (user_ctx) {
        ESP_RETURN_ON_FALSE(esp_ptr_internal(user_ctx), ESP_ERR_INVALID_ARG, TAG, "user context not in internal RAM");
    }
#endif // CONFIG_LCD_DSI_ISR_IRAM_SAFE
    dsi_panel->on_vsync = callbacks->on_vsync;
    dsi_panel->user_ctx = user_ctx;
    return ESP_OK;
}

esp_err_t esp_lcd_dsi_panel_get_frame_buffer(esp_lcd_panel_handle_t panel, uint32_t fb_num, void **fb0, ...)
{
    ESP_RETURN_ON_FALSE(panel, ESP_ERR_INVALID_ARG, TAG, "invalid argument");
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);
    ESP_RETURN_ON_FALSE(fb_num && fb_num <= dsi_panel->num_fbs, ESP_ERR_INVALID_ARG, TAG, "invalid frame buffer number");
    void **fb_itor = fb0;
    va_list args;
    va_start(args, fb0);
    for (int i = 0; i < fb_num; i++) {
        if (fb_itor) {
            *fb_itor = dsi_panel->fbs[i];
            fb_itor = va_arg(args, void **);
        }
    }
    va_end(args);
    return ESP_OK;
}

static esp_err_t dsi_panel_del(esp_lcd_panel_t *panel)
{
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);
    ESP_LOGI(TAG, "del dsi panel @%p", panel);
    ESP_RETURN_ON_ERROR(lcd_dsi_panel_destory(dsi_panel), TAG, "destroy dsi panel @%p failed", panel);
    return ESP_OK;
}

static esp_err_t dsi_panel_reset(esp_lcd_panel_t *panel)
{
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);
    return ESP_OK;
}

static esp_err_t dsi_panel_init(esp_lcd_panel_t *panel)
{
    esp_err_t ret = ESP_OK;
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);

#if !TEST_DSI_LOOPBACK && TEST_DSI_ESCAPE_MODE
    // 可以断开 clk lane, 观察data lane0 是否进入了escape mode
    uint16_t data[128];

    ESP_LOGI(TAG, "esacpe mode test");

    for (int x = 0; x < 128; x++) {
        data[x] = 0x001F;
    }

    for (int x = 0; x < 100; x++) {
        // Memory write
        mipi_dcs_write_data((uint8_t *)data, 128 * 2);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    mipi_dcs_write_cmd(0x29, 0); //Display ON
#endif

    // MIPI_DSI_HOST.phy_ulps_ctrl.phy_txexitulpsclk = 1;
    mipi_dsih_hal_gen_cmd_mode_en(0);

    MIPI_DSI_HOST.lpclk_ctrl.val = 0x00000000 |
                                   ((1 & 0x00000001) << 1) | // bit[    1] AUTO_CLKLANE_CTRL     = 0 (Automatic mechanism to stop providing clock in the clock lane when time allows.
                                   ((1 & 0x00000001) << 0);  // bit[    0] TXREQUESTCLKHS        = 1 (Requesting High Speed Clock Transmission.
#if TEST_DSI_PATTERN
    MIPI_DSI_HOST.vid_mode_cfg.val = 0x00000000 |
                                     ((1 & 0x00000001) << 24) | // bit[   24] VPG_ORIENTATION       = 1 (Video Pattern Generator Orientation - 0=Vertical / 1=Horizontal
                                     ((0 & 0x00000001) << 20) | // bit[   20] VPG_MODE              = 0 (Video Pattern Generator Mode        - 0=ColorBAr / 1=BER Testing
                                     ((1 & 0x00000001) << 16) | // bit[   16] VPG_EN                = 0 (Video Pattern Generator Activation
                                     ((0 & 0x00000001) << 15) | // bit[   15] LPCMDEN               = 0 (Enables Low Power command transmission.
                                     ((1 & 0x00000001) << 13) | // bit[   13] LP_HFP_EN             = 1 (Enables Low Power HFP.
                                     ((1 & 0x00000001) << 12) | // bit[   12] LP_HBP_EN             = 1 (Enables Low Power HBP.
                                     ((1 & 0x00000001) << 11) | // bit[   11] LP_VACT_EN            = 1 (Enables Low Power VACT.
                                     ((1 & 0x00000001) << 10) | // bit[   10] LP_VFP_EN             = 1 (Enables Low Power VFP.
                                     ((1 & 0x00000001) << 9) |  // bit[    9] LP_VBP_EN             = 1 (Enables Low Power VBP.
                                     ((1 & 0x00000001) << 8) |  // bit[    8] LP_VSA_EN             = 1 (Enables Low Power VSA.
                                     ((2 & 0x00000003) << 0);   // bit[ 1: 0] VIDEO_MODE            = 2 (Video transmission mode. - 2=Burst Mode
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
#endif

    dw_gdma_mipi_dsi_register_callback(lcd_default_isr_handler, dsi_panel);

    // Enable DW_GDMA CH-2 transfer.
    dw_gdma_mipi_dsi_start();

    esp_rom_delay_us(10);

    mipi_dsi_bridge_start();

    ESP_LOGI(TAG, "dsi panel @%p start, dpipclk=%"PRIu32"Hz, dsirate=%"PRIu32"Hz", dsi_panel,
             dsi_panel->timings.dpi_clk_hz, dsi_panel->timings.dsi_line_hz);

    return ret;
}

__attribute__((always_inline))
static inline void copy_pixel_8bpp(uint8_t *to, const uint8_t *from)
{
    *to++ = *from++;
}

__attribute__((always_inline))
static inline void copy_pixel_16bpp(uint8_t *to, const uint8_t *from)
{
    *to++ = *from++;
    *to++ = *from++;
}

__attribute__((always_inline))
static inline void copy_pixel_24bpp(uint8_t *to, const uint8_t *from)
{
    *to++ = *from++;
    *to++ = *from++;
    *to++ = *from++;
}

#define COPY_PIXEL_CODE_BLOCK(_bpp)                                                               \
    switch (dsi_panel->rotate_mask)                                                               \
    {                                                                                             \
    case 0:                                                                                       \
    {                                                                                             \
        uint8_t *to = fb + (y_start * h_res + x_start) * bytes_per_pixel;                         \
        for (int y = y_start; y < y_end; y++)                                                     \
        {                                                                                         \
            memcpy(to, from, copy_bytes_per_line);                                                \
            to += bytes_per_line;                                                                 \
            from += copy_bytes_per_line;                                                          \
        }                                                                                         \
        bytes_to_flush = (y_end - y_start) * bytes_per_line;                                      \
        flush_ptr = fb + y_start * bytes_per_line;                                                \
    }                                                                                             \
    break;                                                                                        \
    case ROTATE_MASK_MIRROR_X:                                                                    \
        for (int y = y_start; y < y_end; y++)                                                     \
        {                                                                                         \
            uint32_t index = (y * h_res + (h_res - 1 - x_start)) * bytes_per_pixel;               \
            for (size_t x = x_start; x < x_end; x++)                                              \
            {                                                                                     \
                copy_pixel_##_bpp##bpp(to + index, from);                                         \
                index -= bytes_per_pixel;                                                         \
                from += bytes_per_pixel;                                                          \
            }                                                                                     \
        }                                                                                         \
        bytes_to_flush = (y_end - y_start) * bytes_per_line;                                      \
        flush_ptr = fb + y_start * bytes_per_line;                                                \
        break;                                                                                    \
    case ROTATE_MASK_MIRROR_Y:                                                                    \
    {                                                                                             \
        uint8_t *to = fb + ((v_res - 1 - y_start) * h_res + x_start) * bytes_per_pixel;           \
        for (int y = y_start; y < y_end; y++)                                                     \
        {                                                                                         \
            memcpy(to, from, copy_bytes_per_line);                                                \
            to -= bytes_per_line;                                                                 \
            from += copy_bytes_per_line;                                                          \
        }                                                                                         \
        bytes_to_flush = (y_end - y_start) * bytes_per_line;                                      \
        flush_ptr = fb + (v_res - y_end) * bytes_per_line;                                        \
    }                                                                                             \
    break;                                                                                        \
    case ROTATE_MASK_MIRROR_X | ROTATE_MASK_MIRROR_Y:                                             \
        for (int y = y_start; y < y_end; y++)                                                     \
        {                                                                                         \
            uint32_t index = ((v_res - 1 - y) * h_res + (h_res - 1 - x_start)) * bytes_per_pixel; \
            for (size_t x = x_start; x < x_end; x++)                                              \
            {                                                                                     \
                copy_pixel_##_bpp##bpp(to + index, from);                                         \
                index -= bytes_per_pixel;                                                         \
                from += bytes_per_pixel;                                                          \
            }                                                                                     \
        }                                                                                         \
        bytes_to_flush = (y_end - y_start) * bytes_per_line;                                      \
        flush_ptr = fb + (v_res - y_end) * bytes_per_line;                                        \
        break;                                                                                    \
    case ROTATE_MASK_SWAP_XY:                                                                     \
        for (int y = y_start; y < y_end; y++)                                                     \
        {                                                                                         \
            for (int x = x_start; x < x_end; x++)                                                 \
            {                                                                                     \
                uint32_t j = y * copy_bytes_per_line + x * bytes_per_pixel - offset;              \
                uint32_t i = (x * h_res + y) * bytes_per_pixel;                                   \
                copy_pixel_##_bpp##bpp(to + i, from + j);                                         \
            }                                                                                     \
        }                                                                                         \
        bytes_to_flush = (x_end - x_start) * bytes_per_line;                                      \
        flush_ptr = fb + x_start * bytes_per_line;                                                \
        break;                                                                                    \
    case ROTATE_MASK_SWAP_XY | ROTATE_MASK_MIRROR_X:                                              \
        for (int y = y_start; y < y_end; y++)                                                     \
        {                                                                                         \
            for (int x = x_start; x < x_end; x++)                                                 \
            {                                                                                     \
                uint32_t j = y * copy_bytes_per_line + x * bytes_per_pixel - offset;              \
                uint32_t i = (x * h_res + h_res - 1 - y) * bytes_per_pixel;                       \
                copy_pixel_##_bpp##bpp(to + i, from + j);                                         \
            }                                                                                     \
        }                                                                                         \
        bytes_to_flush = (x_end - x_start) * bytes_per_line;                                      \
        flush_ptr = fb + x_start * bytes_per_line;                                                \
        break;                                                                                    \
    case ROTATE_MASK_SWAP_XY | ROTATE_MASK_MIRROR_Y:                                              \
        for (int y = y_start; y < y_end; y++)                                                     \
        {                                                                                         \
            for (int x = x_start; x < x_end; x++)                                                 \
            {                                                                                     \
                uint32_t j = y * copy_bytes_per_line + x * bytes_per_pixel - offset;              \
                uint32_t i = ((v_res - 1 - x) * h_res + y) * bytes_per_pixel;                     \
                copy_pixel_##_bpp##bpp(to + i, from + j);                                         \
            }                                                                                     \
        }                                                                                         \
        bytes_to_flush = (x_end - x_start) * bytes_per_line;                                      \
        flush_ptr = fb + (v_res - x_end) * bytes_per_line;                                        \
        break;                                                                                    \
    case ROTATE_MASK_SWAP_XY | ROTATE_MASK_MIRROR_X | ROTATE_MASK_MIRROR_Y:                       \
        for (int y = y_start; y < y_end; y++)                                                     \
        {                                                                                         \
            for (int x = x_start; x < x_end; x++)                                                 \
            {                                                                                     \
                uint32_t j = y * copy_bytes_per_line + x * bytes_per_pixel - offset;              \
                uint32_t i = ((v_res - 1 - x) * h_res + h_res - 1 - y) * bytes_per_pixel;         \
                copy_pixel_##_bpp##bpp(to + i, from + j);                                         \
            }                                                                                     \
        }                                                                                         \
        bytes_to_flush = (x_end - x_start) * bytes_per_line;                                      \
        flush_ptr = fb + (v_res - x_end) * bytes_per_line;                                        \
        break;                                                                                    \
    default:                                                                                      \
        break;                                                                                    \
    }

static esp_err_t dsi_panel_draw_bitmap(esp_lcd_panel_t *panel, int x_start, int y_start, int x_end, int y_end, const void *color_data)
{
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);
    ESP_RETURN_ON_FALSE(dsi_panel->num_fbs > 0, ESP_ERR_NOT_SUPPORTED, TAG, "no frame buffer installed");
    assert((x_start < x_end) && (y_start < y_end) && "start position must be smaller than end position");

    // check if we need to copy the draw buffer (pointed by the color_data) to the driver's frame buffer
    bool do_copy = false;
    if (color_data == dsi_panel->fbs[0]) {
        dsi_panel->cur_fb_index = 0;
    } else if (color_data == dsi_panel->fbs[1]) {
        dsi_panel->cur_fb_index = 1;
    } else if (color_data == dsi_panel->fbs[2]) {
        dsi_panel->cur_fb_index = 2;
    } else {
        // we do the copy only if the color_data is different from either frame buffer
        do_copy = true;
    }

    // adjust the flush window by adding extra gap
    x_start += dsi_panel->x_gap;
    y_start += dsi_panel->y_gap;
    x_end += dsi_panel->x_gap;
    y_end += dsi_panel->y_gap;
    // round the boundary
    int h_res = dsi_panel->timings.h_res;
    int v_res = dsi_panel->timings.v_res;
    if (dsi_panel->rotate_mask & ROTATE_MASK_SWAP_XY) {
        x_start = MIN(x_start, v_res);
        x_end = MIN(x_end, v_res);
        y_start = MIN(y_start, h_res);
        y_end = MIN(y_end, h_res);
    } else {
        x_start = MIN(x_start, h_res);
        x_end = MIN(x_end, h_res);
        y_start = MIN(y_start, v_res);
        y_end = MIN(y_end, v_res);
    }

    int bytes_per_pixel = dsi_panel->fb_bits_per_pixel / 8;
    int pixels_per_line = dsi_panel->timings.h_res;
    uint32_t bytes_per_line = bytes_per_pixel * pixels_per_line;
    uint8_t *fb = dsi_panel->fbs[dsi_panel->cur_fb_index];
    size_t bytes_to_flush = v_res * h_res * bytes_per_pixel;
    uint8_t *flush_ptr = fb;

    if (do_copy) {
        // copy the UI draw buffer into internal frame buffer
        const uint8_t *from = (const uint8_t *)color_data;
        uint32_t copy_bytes_per_line = (x_end - x_start) * bytes_per_pixel;
        size_t offset = y_start * copy_bytes_per_line + x_start * bytes_per_pixel;
        uint8_t *to = fb;
        if (1 == bytes_per_pixel) {
            COPY_PIXEL_CODE_BLOCK(8)
        } else if (2 == bytes_per_pixel) {
            COPY_PIXEL_CODE_BLOCK(16)
        } else if (3 == bytes_per_pixel) {
            COPY_PIXEL_CODE_BLOCK(24)
        }
    } else {
        dw_gdma_mipi_dsi_set_buffer(color_data);
    }

    // Note that if we use a bounce buffer, the data gets read by the CPU as well so no need to write back
    if (dsi_panel->flags.fb_in_psram) {
        // CPU writes data to PSRAM through DCache, data in PSRAM might not get updated, so write back
        ESP_RETURN_ON_ERROR(esp_cache_msync(flush_ptr, bytes_to_flush, 0), TAG, "flush cache buffer failed");
    }

    return ESP_OK;
}

static esp_err_t dsi_panel_invert_color(esp_lcd_panel_t *panel, bool invert_color_data)
{
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);
    return ESP_OK;
}

static esp_err_t dsi_panel_mirror(esp_lcd_panel_t *panel, bool mirror_x, bool mirror_y)
{
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);
    dsi_panel->rotate_mask &= ~(ROTATE_MASK_MIRROR_X | ROTATE_MASK_MIRROR_Y);
    dsi_panel->rotate_mask |= (mirror_x << DSI_PANEL_MIRROR_X | mirror_y << DSI_PANEL_MIRROR_Y);
    return ESP_OK;
}

static esp_err_t dsi_panel_swap_xy(esp_lcd_panel_t *panel, bool swap_axes)
{
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);
    dsi_panel->rotate_mask &= ~(ROTATE_MASK_SWAP_XY);
    dsi_panel->rotate_mask |= swap_axes << DSI_PANEL_SWAP_XY;
    return ESP_OK;
}

static esp_err_t dsi_panel_set_gap(esp_lcd_panel_t *panel, int x_gap, int y_gap)
{
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);
    dsi_panel->x_gap = x_gap;
    dsi_panel->y_gap = y_gap;
    return ESP_OK;
}

static esp_err_t dsi_panel_disp_on_off(esp_lcd_panel_t *panel, bool on_off)
{
    esp_dsi_panel_t *dsi_panel = __containerof(panel, esp_dsi_panel_t, base);
    return ESP_OK;
}

IRAM_ATTR static void lcd_default_isr_handler(void *args)
{
    esp_dsi_panel_t *dsi_panel = (esp_dsi_panel_t *)args;
    bool need_yield = false;

    // call user registered callback
    if (dsi_panel->on_vsync) {
        if (dsi_panel->on_vsync(&dsi_panel->base, NULL, dsi_panel->user_ctx)) {
            need_yield = true;
        }
    }

    if (need_yield) {
        portYIELD_FROM_ISR();
    }
}
