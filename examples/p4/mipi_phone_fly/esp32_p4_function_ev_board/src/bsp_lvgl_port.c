/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_timer.h"
#include "esp_lcd_touch.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "lvgl.h"

#include "esp_intr_alloc.h"
#include "gdma_struct.h"
#include "gpio_sig_map.h"
#include "gpio_struct.h"
#include "lp_iomux_struct.h"
#include "iomux_struct.h"
#include "rom/cache.h"
#include "interrupt_core0_reg.h"
#include "ppa.h"

#include "src/draw/sw/lv_draw_sw.h"
#include "bsp_lvgl_port.h"
#include "bsp_err_check.h"
#include "bsp_lcd.h"
#include "mipi_dsi.h"
#include "bsp/display.h"
#include "bsp/touch.h"
#include "bsp/esp32_p4_function_ev_board.h"

#define TEST_INTERRUPT    (1)
#define TEST_PPA          (1)
#define LV_USE_GPU        (1)

#define LVGL_DISP_HSIZE   (MIPI_DSI_DISP_HSIZE)
#define LVGL_DISP_VSIZE   (MIPI_DSI_DISP_VSIZE)

#define DMA2D_IN_CH0_INTR_SOURCE     (((INTERRUPT_CORE0_DMA2D_IN_CH0_INT_MAP_REG - DR_REG_INTERRUPT_CORE0_BASE) / 4))
#define DMA2D_IN_CH1_INTR_SOURCE     (((INTERRUPT_CORE0_DMA2D_IN_CH1_INT_MAP_REG - DR_REG_INTERRUPT_CORE0_BASE) / 4))

typedef struct {
    struct {
        uint8_t *cache;
        uint8_t *dma;
        uint32_t size;
    } buffer[3];
} ppa_test_t;

ppa_test_t ppa_test = {0};

static const char *TAG = "bsp_lvgl_port";
static SemaphoreHandle_t lvgl_mux;                  // LVGL mutex
static TaskHandle_t lvgl_task_handle = NULL;

static lldesc_dma2d_t *rx_link[4], * tx_link[4];

static SemaphoreHandle_t dma2d_in_ch0_sem;
static SemaphoreHandle_t dma2d_in_ch1_sem;

typedef lv_draw_sw_ctx_t lv_draw_ppa_ctx_t;

#if TEST_INTERRUPT
static void dma2d_ch0_isr(void *arg)
{
    (void)arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    typeof(DMA2D.in_ch0.int_st) in_ch0_status = DMA2D.in_ch0.int_st;
    if (in_ch0_status.val != 0) {
        DMA2D.in_ch0.int_clr.val = in_ch0_status.val;
        // ets_printf("dma in intr: 0x%x\n", (int)in_status.val);
        // handle RX interrupt */
        if (in_ch0_status.in_suc_eof) {
            xSemaphoreGiveFromISR( dma2d_in_ch0_sem, &xHigherPriorityTaskWoken );
        }
    }

    portYIELD_FROM_ISR();
}

static void dma2d_ch1_isr(void *arg)
{
    (void)arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    typeof(DMA2D.in_ch1.int_st) in_ch1_status = DMA2D.in_ch1.int_st;
    if (in_ch1_status.val != 0) {
        DMA2D.in_ch1.int_clr.val = in_ch1_status.val;
        // ets_printf("dma in intr: 0x%x\n", (int)in_status.val);
        // handle RX interrupt */
        if (in_ch1_status.in_suc_eof) {
            xSemaphoreGiveFromISR( dma2d_in_ch1_sem, &xHigherPriorityTaskWoken );
        }
    }
    portYIELD_FROM_ISR();
}
#endif

static void flush_callback(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t len = (sizeof(lv_color_t) * ((area->y2 - area->y1 + 1) * (area->x2 - area->x1 + 1)));
    void *dsi_frame_buf = NULL;

    ESP_ERROR_CHECK(bsp_lcd_get_frame_buffer(1, &dsi_frame_buf));

#if !TEST_PPA
    uint8_t *frame_buffer = dsi_frame_buf;
    int cnt = 0;
    int index = 0;

    cnt = 0;

    for (int y = area->y1; y <= area->y2; y++) {
        for (int x = area->x1; x <= area->x2; x++) {
            index = (y * LVGL_DISP_HSIZE + x) * 2 ;
            frame_buffer[index + 0] = ((color_p[cnt].ch.red & 0xF8)) | ((color_p[cnt].ch.green & 0xE0) >> 5);
            frame_buffer[index + 1] = ((color_p[cnt].ch.green & 0x1C) << 3) | ((color_p[cnt].ch.blue & 0x1F));
            cnt++;
        }
    }
    esp_spiram_writeback_cache();
#else
    Cache_WriteBack_Addr(CACHE_MAP_L1_DCACHE, color_p, len);
    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE, color_p, len);

    int hb = area->x2 - area->x1 + 1;
    int vb = area->y2 - area->y1 + 1;

    dma2d_link_dscr_init(rx_link[0], NULL, color_p, hb, vb, hb, vb, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, 0, 0);
    dma2d_link_dscr_init(tx_link[0], NULL, dsi_frame_buf, LVGL_DISP_HSIZE, LVGL_DISP_VSIZE, hb, vb, 1, 1, DMA2D_COLOR_MODE(TEST_DSI_COLOR_WIDTH), 0, area->x1, area->y1);

    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE | CACHE_MAP_L1_DCACHE, ppa_test.buffer[0].cache, 1024);

    PPA.sr_scal_rotate.scal_rotate_rst = 0x1;
    PPA.sr_scal_rotate.scal_rotate_rst = 0x0;

    DMA2D.out_ch[0].conf0.out_rst = 1;
    DMA2D.out_ch[0].conf0.out_rst = 0;
    DMA2D.in_ch0.conf0.in_rst = 1;
    DMA2D.in_ch0.conf0.in_rst = 0;
    //start
    DMA2D.out_ch[0].int_clr.val = ~0;
    DMA2D.in_ch0.int_clr.val = ~0;
    DMA2D.in_ch0.int_ena.val = ~0;
    DMA2D.out_ch[0].link_conf.outlink_start = 0x1;
    DMA2D.in_ch0.link_conf.inlink_start = 0x1;

    //--configure display, scaler/rorate
    PPA.sr_byte_order.sr_rx_byte_swap_en = 0;
    PPA.sr_byte_order.sr_rx_rgb_swap_en = 0;
    PPA.sr_byte_order.sr_macro_bk_ro_bypass = 0;
    PPA.sr_color_mode.sr_rx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);
    PPA.sr_color_mode.sr_tx_cm = PPA_COLOR_MODE(TEST_DSI_COLOR_WIDTH);
    PPA.sr_scal_rotate.sr_rotate_angle = 0;
    PPA.sr_scal_rotate.sr_scal_x_int = 1;
    PPA.sr_scal_rotate.sr_scal_x_frag = 0;
    PPA.sr_scal_rotate.sr_scal_y_int = 1;
    PPA.sr_scal_rotate.sr_scal_y_frag = 0;

    //--start scaler rotate
    PPA.sr_scal_rotate.scal_rotate_start = 0x1;
#if TEST_INTERRUPT
    xSemaphoreTake(dma2d_in_ch0_sem, portMAX_DELAY);
#else
    while (!DMA2D.in_int_raw_ch0.suc_eof);
#endif
    lv_disp_flush_ready(disp_drv);
#endif
}


/*OPTIONAL: GPU INTERFACE*/
#if LV_USE_GPU

/* If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void ppa_blend(lv_color_t *dest_buf, const lv_area_t *dest_area, lv_coord_t dest_stride,
                      const lv_color_t *src_buf, lv_coord_t src_stride, lv_opa_t opa)
{
    /*It's an example code which should be done by your GPU*/
    uint32_t i;
#if !TEST_PPA
    for (i = 0; i < length; i++) {
        dest[i] = lv_color_mix(dest[i], src[i], opa);
    }
#else
    int hb = dest_area->x2 - dest_area->x1 + 1;
    int vb = dest_area->y2 - dest_area->y1 + 1;
    // dest_buf += dest_stride * dest_area->y1; /*Go to the first line*/

    Cache_WriteBack_Addr(CACHE_MAP_L1_DCACHE, dest_buf, dest_stride * vb * sizeof(lv_color_t));
    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE, dest_buf, dest_stride * vb * sizeof(lv_color_t));
    Cache_WriteBack_Addr(CACHE_MAP_L1_DCACHE, src_buf, src_stride * vb * sizeof(lv_color_t));
    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE, src_buf, src_stride * vb * sizeof(lv_color_t));

    // printf("len: %d, opa: %d\n", length, opa);
    dma2d_link_dscr_init(rx_link[1], NULL, src_buf, src_stride, vb, hb, vb, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, 0, 0);
    dma2d_link_dscr_init(rx_link[2], NULL, dest_buf, dest_stride, vb, hb, vb, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, 0, 0);
    dma2d_link_dscr_init(tx_link[1], NULL, dest_buf, dest_stride, vb, hb, vb, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, 0, 0);

    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE | CACHE_MAP_L1_DCACHE, ppa_test.buffer[0].cache, 1024);

    PPA.blend_trans_mode.blend_rst = 0x1;
    PPA.blend_trans_mode.blend_rst = 0x0;

    //start
    DMA2D.out_ch[1].int_clr.val = ~0;
    DMA2D.out_ch[2].int_clr.val = ~0;
    DMA2D.in_ch1.int_clr.val = ~0;
    DMA2D.in_ch1.int_ena.val = ~0;
    DMA2D.out_ch[1].link_conf.outlink_start = 0x1;
    DMA2D.out_ch[2].link_conf.outlink_start = 0x1;
    DMA2D.in_ch1.link_conf.inlink_start = 0x1;

    //--configure display
    PPA.blend_byte_order.blend0_rx_byte_swap_en = 0x0;
    PPA.blend_byte_order.blend1_rx_byte_swap_en = 0x0;
    PPA.blend_byte_order.blend0_rx_rgb_swap_en = 0x0;
    PPA.blend_byte_order.blend1_rx_rgb_swap_en = 0x0;
    PPA.blend_fix_alpha.blend0_rx_fix_alpha = opa;
    PPA.blend_fix_alpha.blend0_rx_alpha_mod = 0x1;
    PPA.blend_fix_alpha.blend0_rx_alpha_inv = 0x0;
    PPA.blend_fix_alpha.blend1_rx_fix_alpha = 255 - opa;
    PPA.blend_fix_alpha.blend1_rx_alpha_mod = 0x1;
    PPA.blend_fix_alpha.blend1_rx_alpha_inv = 0x0;
    PPA.blend_color_mode.blend0_rx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);
    PPA.blend_color_mode.blend1_rx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);
    PPA.blend_color_mode.blend_tx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);

    PPA.blend_trans_mode.blend_bypass = 0x0;
    PPA.blend_trans_mode.blend_fix_pixel_fill_en = 0x0;

    PPA.blend_trans_mode.blend_en = 0x1;
    PPA.blend_trans_mode.blend_trans_mode_update = 0x1;

#if TEST_INTERRUPT
    xSemaphoreTake(dma2d_in_ch1_sem, portMAX_DELAY);
#else
    while (!DMA2D.in_int_raw_ch1.suc_eof);
#endif

    Cache_Invalidate_Addr(CACHE_MAP_L2_CACHE, dest_buf, dest_stride * vb * sizeof(lv_color_t));
    Cache_Invalidate_Addr(CACHE_MAP_L1_DCACHE, dest_buf, dest_stride * vb * sizeof(lv_color_t));
#endif
}

static void ppa_fill(lv_color_t *dest_buf, lv_coord_t dest_width, const lv_area_t *fill_area, lv_color_t color)
{
    int hb = fill_area->x2 - fill_area->x1 + 1;
    int vb = fill_area->y2 - fill_area->y1 + 1;
    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/

    Cache_WriteBack_Addr(CACHE_MAP_L1_DCACHE, dest_buf, dest_width * vb * sizeof(lv_color_t));
    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE, dest_buf, dest_width * vb * sizeof(lv_color_t));

#if 1   // ppa_blend fill 填充

    dma2d_link_dscr_init(tx_link[1], NULL, dest_buf, dest_width, vb, hb, vb, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, fill_area->x1, 0);

    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE | CACHE_MAP_L1_DCACHE, ppa_test.buffer[0].cache, 1024);

    PPA.blend_trans_mode.blend_rst = 0x1;
    PPA.blend_trans_mode.blend_rst = 0x0;

    DMA2D.out_ch[1].conf0.out_rst = 1;
    DMA2D.out_ch[1].conf0.out_rst = 0;
    DMA2D.out_ch[2].conf0.out_rst = 1;
    DMA2D.out_ch[2].conf0.out_rst = 0;
    DMA2D.in_ch1.conf0.in_rst = 1;
    DMA2D.in_ch1.conf0.in_rst = 0;
    //start
    DMA2D.out_ch[1].int_clr.val = ~0;
    DMA2D.out_ch[2].int_clr.val = ~0;
    DMA2D.in_ch1.int_clr.val = ~0;
    DMA2D.in_ch1.int_ena.val = 0;
    DMA2D.out_ch[1].link_conf.outlink_start = 0x0;
    DMA2D.out_ch[2].link_conf.outlink_start = 0x0;
    DMA2D.in_ch1.link_conf.inlink_start = 0x1;

    //--configure display
    PPA.blend_byte_order.blend0_rx_byte_swap_en = 0x0;
    PPA.blend_byte_order.blend1_rx_byte_swap_en = 0x0;
    PPA.blend_byte_order.blend0_rx_rgb_swap_en = 0x0;
    PPA.blend_byte_order.blend1_rx_rgb_swap_en = 0x0;
    PPA.blend_fix_alpha.blend0_rx_fix_alpha = 0x0;
    PPA.blend_fix_alpha.blend0_rx_alpha_mod = 0x0;
    PPA.blend_fix_alpha.blend0_rx_alpha_inv = 0x0;
    PPA.blend_fix_alpha.blend1_rx_fix_alpha = 0x0;
    PPA.blend_fix_alpha.blend1_rx_alpha_mod = 0x0;
    PPA.blend_fix_alpha.blend1_rx_alpha_inv = 0x0;
    PPA.blend_fix_pixel = lv_color_to32(color);
    PPA.blend_tx_size.blend_hb = hb;
    PPA.blend_tx_size.blend_vb = vb;
    PPA.blend_color_mode.blend0_rx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);
    PPA.blend_color_mode.blend1_rx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);
    PPA.blend_color_mode.blend_tx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);
    PPA.blend_trans_mode.blend_bypass = 0x0;
    PPA.blend_trans_mode.blend_fix_pixel_fill_en = 0x1;

    PPA.blend_trans_mode.blend_en = 0x1;
    PPA.blend_trans_mode.blend_trans_mode_update = 0x1;

    while (!DMA2D.in_ch1.int_raw.in_done);
#else

#endif

    Cache_Invalidate_Addr(CACHE_MAP_L2_CACHE, dest_buf, dest_width * vb * sizeof(lv_color_t));
    Cache_Invalidate_Addr(CACHE_MAP_L1_DCACHE, dest_buf, dest_width * vb * sizeof(lv_color_t));
}

void lv_draw_ppa_buffer_copy(lv_draw_ctx_t *draw_ctx,
                             void *dest_buf, lv_coord_t dest_stride, const lv_area_t *dest_area,
                             void *src_buf, lv_coord_t src_stride, const lv_area_t *src_area)
{
    LV_UNUSED(draw_ctx);
#if !TEST_PPA
    lv_draw_sw_buffer_copy(draw_ctx,
                           dest_buf, dest_stride, dest_area,
                           src_buf, src_stride, src_area);
#else
    printf("lv_draw_ppa_buffer_copy\n");
    lv_color_t *dest_bufc =  dest_buf;
    lv_color_t *src_bufc =  src_buf;

    /*Got the first pixel of each buffer*/
    dest_bufc += dest_stride * dest_area->y1;
    dest_bufc += dest_area->x1;

    src_bufc += src_stride * src_area->y1;
    src_bufc += src_area->x1;

    int hb = dest_area->x2 - dest_area->x1 + 1;
    int vb = dest_area->y2 - dest_area->y1 + 1;
    int len = hb * vb * sizeof(lv_color_t);
    float scal_x = 1;
    float scal_y = 1;

    Cache_WriteBack_Addr(CACHE_MAP_L1_DCACHE, src_bufc, len);
    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE, src_bufc, len);
    Cache_WriteBack_Addr(CACHE_MAP_L1_DCACHE, dest_bufc, len);
    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE, dest_bufc, len);

    dma2d_link_dscr_init(rx_link[0], NULL, src_bufc, hb, vb, hb, vb, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, 0, 0);
    dma2d_link_dscr_init(tx_link[0], NULL, dest_bufc, hb, vb, hb, vb, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, 0, 0);

    Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE | CACHE_MAP_L1_DCACHE, ppa_test.buffer[0].cache, 1024);

    PPA.sr_scal_rotate.scal_rotate_rst = 0x1;
    PPA.sr_scal_rotate.scal_rotate_rst = 0x0;

    DMA2D.out_ch[0].conf0.out_rst = 1;
    DMA2D.out_ch[0].conf0.out_rst = 0;
    DMA2D.in_ch0.conf0.in_rst = 1;
    DMA2D.in_ch0.conf0.in_rst = 0;
    //start
    DMA2D.out_ch[0].int_clr.val = ~0;
    DMA2D.in_ch0.int_clr.val = ~0;
    DMA2D.in_ch0.int_ena.val = ~0;
    DMA2D.out_ch[0].link_conf.outlink_start = 0x1;
    DMA2D.in_ch0.link_conf.inlink_start = 0x1;

    //--configure display, scaler/rorate
    PPA.sr_byte_order.sr_rx_byte_swap_en = 0;
    PPA.sr_byte_order.sr_rx_rgb_swap_en = 0;
    PPA.sr_byte_order.sr_macro_bk_ro_bypass = 0;
    PPA.sr_color_mode.sr_rx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);
    PPA.sr_color_mode.sr_tx_cm = PPA_COLOR_MODE(TEST_DSI_COLOR_WIDTH);
    PPA.sr_scal_rotate.sr_rotate_angle = 0;
    PPA.sr_scal_rotate.sr_scal_x_int = (uint8_t)scal_x;
    PPA.sr_scal_rotate.sr_scal_x_frag = (uint8_t)(scal_x * 16) % 16;
    PPA.sr_scal_rotate.sr_scal_y_int = (uint8_t)scal_y;
    PPA.sr_scal_rotate.sr_scal_y_frag = (uint8_t)(scal_y * 16) % 16;

    //--start scaler rotate
    PPA.sr_scal_rotate.scal_rotate_start = 0x1;
#if TEST_INTERRUPT
    xSemaphoreTake(dma2d_in_ch0_sem, portMAX_DELAY);
#else
    while (!DMA2D.in_int_raw_ch0.suc_eof);
#endif
    Cache_Invalidate_Addr(CACHE_MAP_L2_CACHE, dest_bufc, len);
    Cache_Invalidate_Addr(CACHE_MAP_L1_DCACHE, dest_bufc, len);
#endif
}

void lv_draw_ppa_blend(lv_draw_ctx_t *draw_ctx, const lv_draw_sw_blend_dsc_t *dsc)
{
    lv_area_t blend_area;
    if (!_lv_area_intersect(&blend_area, dsc->blend_area, draw_ctx->clip_area)) {
        return;
    }

    bool done = false;

    if (dsc->mask_buf == NULL && lv_area_get_size(&blend_area) > 100) {
        lv_coord_t dest_stride = lv_area_get_width(draw_ctx->buf_area);

        lv_color_t *dest_buf = draw_ctx->buf;
        dest_buf += dest_stride * (blend_area.y1 - draw_ctx->buf_area->y1) + (blend_area.x1 - draw_ctx->buf_area->x1);

        const lv_color_t *src_buf = dsc->src_buf;
        if (src_buf) {
            lv_coord_t src_stride;
            src_stride = lv_area_get_width(dsc->blend_area);
            src_buf += src_stride * (blend_area.y1 - dsc->blend_area->y1) + (blend_area.x1 -  dsc->blend_area->x1);
            lv_area_move(&blend_area, -draw_ctx->buf_area->x1, -draw_ctx->buf_area->y1);
            ppa_blend(dest_buf, &blend_area, dest_stride, src_buf, src_stride, dsc->opa);
            done = true;
        } else if (dsc->opa >= LV_OPA_MAX) {
            lv_area_move(&blend_area, -draw_ctx->buf_area->x1, -draw_ctx->buf_area->y1);
            ppa_fill(draw_ctx->buf, dest_stride, &blend_area, dsc->color);
            done = true;
        }
    }

    if (!done) {
        lv_draw_sw_blend_basic(draw_ctx, dsc);
    }
}

void lv_draw_ppa_ctx_init(lv_disp_drv_t *drv, lv_draw_ctx_t *draw_ctx)
{

    lv_draw_sw_init_ctx(drv, draw_ctx);

    lv_draw_ppa_ctx_t *ppa_draw_ctx = (lv_draw_sw_ctx_t *)draw_ctx;

    ppa_draw_ctx->blend = lv_draw_ppa_blend;
    ppa_draw_ctx->base_draw.buffer_copy = lv_draw_ppa_buffer_copy;
    printf("ppa_ctx_init\n");

}

void lv_draw_ppa_ctx_deinit(lv_disp_drv_t *drv, lv_draw_ctx_t *draw_ctx)
{
    LV_UNUSED(drv);
    LV_UNUSED(draw_ctx);
}
#endif  /*LV_USE_GPU*/

static lv_disp_t *display_init(esp_lcd_panel_handle_t lcd)
{
    // BSP_NULL_CHECK(lcd, NULL);

    static int hsize = LVGL_DISP_HSIZE;
    static int vsize = LVGL_DISP_VSIZE;
    static lv_disp_draw_buf_t  draw_buf;
    static lv_color_t *draw_buf_1 = NULL;
    static lv_color_t *draw_buf_2 = NULL;

    // draw_buf_1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * (LVGL_DISP_HSIZE * LVGL_DISP_VSIZE / 4), MALLOC_CAP_INTERNAL); /*A screen sized buffer*/
    // draw_buf_1 = (lv_color_t *)malloc(sizeof(lv_color_t) * ((hsize * vsize) / 16));
    // draw_buf_1 = (lv_color_t *)(PSRAM_CACHE_BASE + 0x800000);
    //draw_buf_2 = (lv_color_t *)(dsi_frame_buf + LVGL_DISP_HSIZE * LVGL_DISP_VSIZE * 6);

    draw_buf_1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * LVGL_DISP_HSIZE * LVGL_DISP_VSIZE, MALLOC_CAP_SPIRAM); /*A screen sized buffer*/
    assert(draw_buf_1);
    lv_disp_draw_buf_init(&draw_buf, draw_buf_1, NULL, LVGL_DISP_HSIZE * LVGL_DISP_VSIZE);   /*Initialize the display buffer*/

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = flush_callback;     /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
    disp_drv.hor_res = hsize;
    disp_drv.ver_res = vsize;
    disp_drv.physical_hor_res = -1;
    disp_drv.physical_ver_res = -1;
    disp_drv.offset_x = 0;
    disp_drv.offset_y = 0;
    /*Set a display buffer*/
    // disp_drv.direct_mode = 1;
    //disp_drv.full_refresh = 1;
#if LV_USE_GPU
    disp_drv.draw_ctx_init = lv_draw_ppa_ctx_init;
    disp_drv.draw_ctx_deinit = lv_draw_ppa_ctx_deinit;
    disp_drv.draw_ctx_size = sizeof(lv_draw_ppa_ctx_t);
#endif

    return lv_disp_drv_register(&disp_drv);
}

static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    esp_lcd_touch_handle_t tp = (esp_lcd_touch_handle_t)indev_drv->user_data;
    assert(tp);

    uint16_t touchpad_x;
    uint16_t touchpad_y;
    uint8_t touchpad_cnt = 0;
    /* Read data from touch controller into memory */
    esp_lcd_touch_read_data(tp);

    /* Read data from touch controller */
    bool touchpad_pressed = esp_lcd_touch_get_coordinates(tp, &touchpad_x, &touchpad_y, NULL, &touchpad_cnt, 1);
    if (touchpad_pressed && touchpad_cnt > 0) {
        data->point.x = touchpad_x;
        data->point.y = touchpad_y;
        data->state = LV_INDEV_STATE_PRESSED;
        ESP_LOGD(TAG, "Touch position: %d,%d", touchpad_x, touchpad_y);
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static lv_indev_t *indev_init(esp_lcd_touch_handle_t tp)
{
    BSP_NULL_CHECK(tp, NULL);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.user_data = tp;
    indev_drv.read_cb = touchpad_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/

    return lv_indev_drv_register(&indev_drv);
}

static void tick_increment(void *arg)
{
    /* Tell LVGL how many milliseconds have elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

static esp_err_t tick_init(void)
{
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &tick_increment,
        .name = "LVGL tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    BSP_ERROR_CHECK_RETURN_ERR(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    return esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000);
}

static void lvgl_port_task(void *arg)
{
    ESP_LOGI(TAG, "Starting LVGL task");
    while (1) {
        bsp_lvgl_port_lock(0);
        uint32_t task_delay_ms = lv_timer_handler();
        bsp_lvgl_port_unlock();
        if (task_delay_ms > 500) {
            task_delay_ms = 500;
        } else if (task_delay_ms < CONFIG_BSP_DISPLAY_LVGL_TASK_DELAY) {
            task_delay_ms = CONFIG_BSP_DISPLAY_LVGL_TASK_DELAY;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

esp_err_t bsp_lvgl_port_init(esp_lcd_panel_handle_t lcd, esp_lcd_touch_handle_t tp, lv_disp_t **disp, lv_indev_t **indev)
{
    uint8_t *link_buffer = (uint32_t *)malloc(1024 + 7);

    if ((uint32_t)link_buffer % 8) {
        link_buffer = (uint32_t *)((uint32_t)link_buffer + (8 - ((uint32_t)link_buffer % 8)));
    }
    uint8_t *link_buffer_dma = dma2d_access_addr_map(link_buffer);
    ppa_test.buffer[0].cache = link_buffer;
    ppa_test.buffer[0].dma = link_buffer_dma;
    ppa_test.buffer[0].size = 1024;

    rx_link[0] = (uint32_t *)((uint32_t)ppa_test.buffer[0].dma + 0x0000);
    tx_link[0] = (uint32_t *)((uint32_t)ppa_test.buffer[0].dma + 0x0020);
    rx_link[1] = (uint32_t *)((uint32_t)ppa_test.buffer[0].dma + 0x0040);
    tx_link[1] = (uint32_t *)((uint32_t)ppa_test.buffer[0].dma + 0x0060);
    rx_link[2] = (uint32_t *)((uint32_t)ppa_test.buffer[0].dma + 0x0080);
    tx_link[2] = (uint32_t *)((uint32_t)ppa_test.buffer[0].dma + 0x00A0);
    rx_link[3] = (uint32_t *)((uint32_t)ppa_test.buffer[0].dma + 0x00C0);
    tx_link[3] = (uint32_t *)((uint32_t)ppa_test.buffer[0].dma + 0x00E0);

    // reset DMA2D
    DMA2D.out_ch[0].conf0.out_rst = 1;
    DMA2D.out_ch[0].conf0.out_rst = 0;
    DMA2D.in_ch0.conf0.in_rst = 1;
    DMA2D.in_ch0.conf0.in_rst = 0;

    // inter-mem & extr-mem start/end addr
    DMA2D.intr_mem_start_addr = 0x00000000;
    DMA2D.intr_mem_end_addr = 0xFFFFFFFF;
    DMA2D.extr_mem_start_addr = 0x60000000;
    DMA2D.extr_mem_end_addr = 0x8FFFFFFF;

    // peri_sel
    DMA2D.out_ch[0].peri_sel.out_peri_sel = 0x1;
    DMA2D.in_ch0.peri_sel.in_peri_sel = 0x1;

    // commonn
    DMA2D.out_ch[0].conf0.out_dscr_port_en = 0x1;
    DMA2D.out_ch[0].conf0.out_auto_wrback = 0x0;
    DMA2D.out_ch[0].conf0.out_eof_mode = 0x0;
    DMA2D.out_ch[0].conf0.out_check_owner = 0x0;
    DMA2D.out_ch[0].conf0.out_mem_burst_length = 0x3;
    DMA2D.out_ch[0].conf0.outdscr_burst_en = 0x1;

    DMA2D.in_ch0.conf0.in_mem_trans_en = 0x0;
    DMA2D.in_ch0.conf0.in_dscr_port_en = 0x1;
    DMA2D.in_ch0.conf0.in_check_owner = 0x0;
    DMA2D.in_ch0.conf0.in_mem_burst_length = 0x3;
    DMA2D.in_ch0.conf0.indscr_burst_en = 0x1;

    // dscr addr
    DMA2D.out_ch[0].link_addr = rx_link[0];
    DMA2D.in_ch0.link_addr = tx_link[0];

    // reset DMA2D
    DMA2D.out_ch[1].conf0.out_rst = 1;
    DMA2D.out_ch[1].conf0.out_rst = 0;
    DMA2D.out_ch[2].conf0.out_rst = 1;
    DMA2D.out_ch[2].conf0.out_rst = 0;
    DMA2D.in_ch1.conf0.in_rst = 1;
    DMA2D.in_ch1.conf0.in_rst = 0;

    // peri_sel
    DMA2D.out_ch[1].peri_sel.out_peri_sel = 0x2;
    DMA2D.out_ch[2].peri_sel.out_peri_sel = 0x3;
    DMA2D.in_ch1.peri_sel.in_peri_sel = 0x2;

    // commonn
    DMA2D.out_ch[1].conf0.out_dscr_port_en = 0x0;
    DMA2D.out_ch[1].conf0.out_auto_wrback = 0x0;
    DMA2D.out_ch[1].conf0.out_eof_mode = 0x0;
    DMA2D.out_ch[1].conf0.out_check_owner = 0x0;
    DMA2D.out_ch[1].conf0.out_mem_burst_length = 0x3;
    DMA2D.out_ch[1].conf0.outdscr_burst_en = 0x1;

    DMA2D.out_ch[2].conf0.out_dscr_port_en = 0x0;
    DMA2D.out_ch[2].conf0.out_auto_wrback = 0x0;
    DMA2D.out_ch[2].conf0.out_eof_mode = 0x0;
    DMA2D.out_ch[2].conf0.out_check_owner = 0x0;
    DMA2D.out_ch[2].conf0.out_mem_burst_length = 0x3;
    DMA2D.out_ch[2].conf0.outdscr_burst_en = 0x1;

    DMA2D.in_ch1.conf0.in_dscr_port_en = 0x0;
    DMA2D.in_ch1.conf0.in_check_owner = 0x0;
    DMA2D.in_ch1.conf0.in_mem_burst_length = 0x3;
    DMA2D.in_ch1.conf0.indscr_burst_en = 0x1;

    // dscr addr
    DMA2D.out_ch[1].link_addr = rx_link[1];
    DMA2D.out_ch[2].link_addr = rx_link[2];
    DMA2D.in_ch1.link_addr = tx_link[1];

#if TEST_INTERRUPT
    dma2d_in_ch0_sem = xSemaphoreCreateBinary( );
    dma2d_in_ch1_sem = xSemaphoreCreateBinary( );

    esp_intr_alloc(DMA2D_IN_CH0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, dma2d_ch0_isr, NULL, NULL);
    esp_intr_alloc(DMA2D_IN_CH1_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, dma2d_ch1_isr, NULL, NULL);
#endif

    lv_init();
    BSP_ERROR_CHECK_RETURN_ERR(tick_init());
    BSP_NULL_CHECK(*disp = display_init(lcd), ESP_FAIL);
    BSP_NULL_CHECK(*indev = indev_init(tp), ESP_FAIL);

    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    BSP_NULL_CHECK(lvgl_mux, ESP_FAIL);
    ESP_LOGI(TAG, "Create LVGL task");
    BaseType_t core_id = (LVGL_TASK_CORE_ID < 0) ? tskNO_AFFINITY : LVGL_TASK_CORE_ID;
    BaseType_t ret = xTaskCreatePinnedToCore(
                         lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE_KB * 1024, NULL,
                         LVGL_TASK_PRIORITY, &lvgl_task_handle, core_id
                     );
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create LVGL task");
        return ESP_FAIL;
    }

    return ESP_OK;
}

bool bsp_lvgl_port_lock(uint32_t timeout_ms)
{
    assert(lvgl_mux && "bsp_lvgl_port_init must be called first");

    const TickType_t timeout_ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}

void bsp_lvgl_port_unlock(void)
{
    assert(lvgl_mux && "bsp_lvgl_port_init must be called first");
    xSemaphoreGiveRecursive(lvgl_mux);
}
