/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/portmacro.h"
#include "esp_intr_alloc.h"
#include "gdma_struct.h"
#include "gpio_sig_map.h"
#include "gpio_struct.h"
#include "lp_iomux_struct.h"
#include "iomux_struct.h"
#include "esp_heap_caps.h"
#include "esp_system.h"
#include "esp_log.h"
#include "rom/cache.h"
#include "interrupt_core0_reg.h"
#include "gui.h"
#include "mipi.h"
#include "ov5640.h"
#include "gt911.h"
#include "ppa.h"

static const char *TAG = "main";

#define TEST_INTERRUPT    (1)
#define TEST_PPA          (1)
#define LV_USE_GPU        (1)

#define LVGL_DISP_VPOS    (MIPI_CSI_DISP_VSIZE)
#define LVGL_DISP_HSIZE   (MIPI_DSI_DISP_HSIZE)
#define LVGL_DISP_VSIZE   (MIPI_DSI_DISP_VSIZE - LVGL_DISP_VPOS)
#define PSRAM_DMA_BASE    (0x48001000) // 16M psram, Non-cacheable address range is 0x8B000000 ~ 0x8C000000
#define PSRAM_CACHE_BASE  (0x48001000) // 16M psram, The cacheable address range is 0x4B000000 ~ 0x4C000000.

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

static uint8_t * dsi_frame_buffer = PSRAM_CACHE_BASE + LVGL_DISP_HSIZE * LVGL_DISP_VPOS * (TEST_DSI_COLOR_WIDTH / 8);

static lldesc_dma2d_t *rx_link[4], * tx_link[4];

static SemaphoreHandle_t dma2d_in_ch0_sem;
static SemaphoreHandle_t dma2d_in_ch1_sem;

typedef lv_draw_sw_ctx_t lv_draw_ppa_ctx_t;
static lv_disp_t *disp[1];
static lv_disp_t *indev[1];

static void delay_us(uint32_t t)
{
    ets_delay_us(t);
}

/* Initialize your display and the required peripherals. */
static void lv_disp_init(void)
{
    int stat;

    
}

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

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_disp_flush_ready()' has to be called when finished. */
static void lv_disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    uint32_t len = (sizeof(lv_color_t) * ((area->y2 - area->y1 + 1)*(area->x2 - area->x1 + 1)));

#if !TEST_PPA
    uint8_t * frame_buffer = dsi_frame_buffer;
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
    dma2d_link_dscr_init(tx_link[0], NULL, dsi_frame_buffer, LVGL_DISP_HSIZE, LVGL_DISP_VSIZE, hb, vb, 1, 1, DMA2D_COLOR_MODE(TEST_DSI_COLOR_WIDTH), 0, area->x1, area->y1);

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
static void ppa_blend(lv_color_t * dest_buf, const lv_area_t * dest_area, lv_coord_t dest_stride,
                                          const lv_color_t * src_buf, lv_coord_t src_stride, lv_opa_t opa)
{
    /*It's an example code which should be done by your GPU*/
    uint32_t i;
#if !TEST_PPA
    for(i = 0; i < length; i++) {
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

static void ppa_fill(lv_color_t * dest_buf, lv_coord_t dest_width, const lv_area_t * fill_area, lv_color_t color)
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
    uint32_t scal_x = 1;
    uint32_t scal_y = 1;
    int max_fill_line = 255;

    if (hb <= 255 && vb <= 255) {
            scal_x = hb;
            scal_y = vb;
            Cache_WriteBack_Addr(CACHE_MAP_L1_DCACHE, &color.full, 1 * sizeof(lv_color_t));
            Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE, &color.full, 1 * sizeof(lv_color_t));

            dma2d_link_dscr_init(rx_link[0], NULL, &color.full, 1, 1, 1, 1, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, 0, 0);
            dma2d_link_dscr_init(tx_link[0], NULL, dest_buf, dest_width, vb, scal_x, scal_y, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, fill_area->x1, 0);

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
            PPA.sr_color_mode.sr_tx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);
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

    } else {
        lv_color_t buf[dest_width];
        for (int x = 0; x < hb; x++) {
            buf[x].full = color.full;
        }

        Cache_WriteBack_Addr(CACHE_MAP_L1_DCACHE, buf, dest_width * sizeof(lv_color_t));
        Cache_WriteBack_Addr(CACHE_MAP_L2_CACHE, buf, dest_width * sizeof(lv_color_t));

        scal_y = max_fill_line;
        int y = 0;
        for (y = 0; y < vb / scal_y; y++) {
            dma2d_link_dscr_init(rx_link[0], NULL, dma2d_access_addr_map(buf), dest_width, 1, hb, 1, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, 0, 0);
            dma2d_link_dscr_init(tx_link[0], NULL, dma2d_access_addr_map(dest_buf), dest_width, vb, hb, scal_y, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, fill_area->x1, y * max_fill_line);

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
            PPA.sr_color_mode.sr_tx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);
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
        }

        scal_y = (vb % max_fill_line);
        if (scal_y) {
            dma2d_link_dscr_init(rx_link[0], NULL, dma2d_access_addr_map(buf), dest_width, 1, hb, 1, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, 0, 0);
            dma2d_link_dscr_init(tx_link[0], NULL, dma2d_access_addr_map(dest_buf), dest_width, vb, hb, scal_y, 1, 1, DMA2D_COLOR_MODE(LV_COLOR_DEPTH), 0, fill_area->x1, y * max_fill_line);

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
            PPA.sr_color_mode.sr_tx_cm = PPA_COLOR_MODE(LV_COLOR_DEPTH);
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
        }
    }

#endif

    Cache_Invalidate_Addr(CACHE_MAP_L2_CACHE, dest_buf, dest_width * vb * sizeof(lv_color_t)); 
    Cache_Invalidate_Addr(CACHE_MAP_L1_DCACHE, dest_buf, dest_width * vb * sizeof(lv_color_t)); 
}

void lv_draw_ppa_buffer_copy(lv_draw_ctx_t * draw_ctx,
                            void * dest_buf, lv_coord_t dest_stride, const lv_area_t * dest_area,
                            void * src_buf, lv_coord_t src_stride, const lv_area_t * src_area)
{
    LV_UNUSED(draw_ctx);
#if !TEST_PPA
    lv_draw_sw_buffer_copy(draw_ctx,
                            dest_buf, dest_stride, dest_area,
                            src_buf, src_stride, src_area);
#else
    printf("lv_draw_ppa_buffer_copy\n");
    lv_color_t * dest_bufc =  dest_buf;
    lv_color_t * src_bufc =  src_buf;

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

void lv_draw_ppa_blend(lv_draw_ctx_t * draw_ctx, const lv_draw_sw_blend_dsc_t * dsc)
{
    lv_area_t blend_area;
    if(!_lv_area_intersect(&blend_area, dsc->blend_area, draw_ctx->clip_area)) return;

    bool done = false;

    if(dsc->mask_buf == NULL && lv_area_get_size(&blend_area) > 100) {
        lv_coord_t dest_stride = lv_area_get_width(draw_ctx->buf_area);

        lv_color_t * dest_buf = draw_ctx->buf;
        dest_buf += dest_stride * (blend_area.y1 - draw_ctx->buf_area->y1) + (blend_area.x1 - draw_ctx->buf_area->x1);

        const lv_color_t * src_buf = dsc->src_buf;
        if(src_buf) {
            lv_coord_t src_stride;
            src_stride = lv_area_get_width(dsc->blend_area);
            src_buf += src_stride * (blend_area.y1 - dsc->blend_area->y1) + (blend_area.x1 -  dsc->blend_area->x1);
            lv_area_move(&blend_area, -draw_ctx->buf_area->x1, -draw_ctx->buf_area->y1);
            ppa_blend(dest_buf, &blend_area, dest_stride, src_buf, src_stride, dsc->opa);
            done = true;
        }
        else if(dsc->opa >= LV_OPA_MAX) {
            lv_area_move(&blend_area, -draw_ctx->buf_area->x1, -draw_ctx->buf_area->y1);
            ppa_fill(draw_ctx->buf, dest_stride, &blend_area, dsc->color);
            done = true;
        }
    }

    if(!done) lv_draw_sw_blend_basic(draw_ctx, dsc);
}

void lv_draw_ppa_ctx_init(lv_disp_drv_t * drv, lv_draw_ctx_t * draw_ctx)
{

    lv_draw_sw_init_ctx(drv, draw_ctx);

    lv_draw_ppa_ctx_t * ppa_draw_ctx = (lv_draw_sw_ctx_t *)draw_ctx;

    ppa_draw_ctx->blend = lv_draw_ppa_blend;
    ppa_draw_ctx->base_draw.buffer_copy = lv_draw_ppa_buffer_copy;
    printf("ppa_ctx_init\n");

}

void lv_draw_ppa_ctx_deinit(lv_disp_drv_t * drv, lv_draw_ctx_t * draw_ctx)
{
    LV_UNUSED(drv);
    LV_UNUSED(draw_ctx);
}

#endif  /*LV_USE_GPU*/

static bool lv_disp_input(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    int ret = 0;
    static uint16_t x = 0, y = 0;

    ret = gt911_pos_read(&x, &y);
    if (ret && y >= LVGL_DISP_VPOS) {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = x;   
        data->point.y = y - LVGL_DISP_VPOS;
    } else {
        data->state =  LV_INDEV_STATE_REL;
    }
    // printf("x: %d, y: %d\n", x, y);
    return false; /*No buffering so no more data read*/
}

static void lv_tick_task(void * arg)
{
    while(1) {
        lv_tick_inc(10);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void gui_task(void *arg)
{
    lv_disp_init();

    xTaskCreate(lv_tick_task, "lv_tick_task", 1024, NULL, 5, NULL);

    lv_init();

    static int hsize = LVGL_DISP_HSIZE;
    static int vsize = LVGL_DISP_VSIZE;
    static lv_disp_draw_buf_t  draw_buf;
    static lv_color_t *draw_buf_1 = NULL;
    static lv_color_t *draw_buf_2 = NULL;

    // draw_buf_1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * (LVGL_DISP_HSIZE * LVGL_DISP_VSIZE / 4), MALLOC_CAP_INTERNAL); /*A screen sized buffer*/
    // draw_buf_1 = (lv_color_t *)malloc(sizeof(lv_color_t) * ((hsize * vsize) / 16));
    draw_buf_1 = (lv_color_t *)(PSRAM_CACHE_BASE + 0x800000);
    //draw_buf_2 = (lv_color_t *)(dsi_frame_buffer + LVGL_DISP_HSIZE * LVGL_DISP_VSIZE * 6);
    lv_disp_draw_buf_init(&draw_buf, draw_buf_1, NULL, (hsize * vsize) / 1);   /*Initialize the display buffer*/

    /*Create a display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = lv_disp_flush;    /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
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
    disp[0] = lv_disp_drv_register(&disp_drv);

    gt911_touch_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = lv_disp_input;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev[0] = lv_indev_drv_register(&indev_drv);
    
    lv_disp_set_default(disp[0]);

    gui_init(disp, indev, NULL);
    while(1) {
        lv_task_handler();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

static void mipi_task(void *arg)
{
    mipi_init(PSRAM_DMA_BASE, PSRAM_CACHE_BASE);

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

    //reset
    DMA2D.out_ch[0].conf0.out_rst = 1;
    DMA2D.out_ch[0].conf0.out_rst = 0;
    DMA2D.in_ch0.conf0.in_rst = 1;
    DMA2D.in_ch0.conf0.in_rst = 0;

    //inter-mem & extr-mem start/end addr
    DMA2D.intr_mem_start_addr = 0x00000000;
    DMA2D.intr_mem_end_addr = 0xFFFFFFFF;
    DMA2D.extr_mem_start_addr = 0x60000000;
    DMA2D.extr_mem_end_addr = 0x8FFFFFFF;

    //peri_sel
    DMA2D.out_ch[0].peri_sel.out_peri_sel = 0x1;
    DMA2D.in_ch0.peri_sel.in_peri_sel = 0x1;

    //commonn
    DMA2D.out_ch[0].conf0.out_dscr_port_en = 0x1;
    DMA2D.out_ch[0].conf0.out_auto_wrback = 0x0;
    DMA2D.out_ch[0].conf0.out_eof_mode = 0x0;
    DMA2D.out_ch[0].conf0.out_check_owner = 0x0;
    DMA2D.out_ch[0].conf0.out_mem_burst_length = 0x4;
    DMA2D.out_ch[0].conf0.outdscr_burst_en = 0x1;

    DMA2D.in_ch0.conf0.in_mem_trans_en = 0x0;
    DMA2D.in_ch0.conf0.in_dscr_port_en = 0x1;
    DMA2D.in_ch0.conf0.in_check_owner = 0x0;
    DMA2D.in_ch0.conf0.in_mem_burst_length = 0x4;
    DMA2D.in_ch0.conf0.indscr_burst_en = 0x1;

    //dscr addr
    DMA2D.out_ch[0].link_addr = rx_link[0];
    DMA2D.in_ch0.link_addr = tx_link[0];

    //reset
    DMA2D.out_ch[1].conf0.out_rst = 1;
    DMA2D.out_ch[1].conf0.out_rst = 0;
    DMA2D.out_ch[2].conf0.out_rst = 1;
    DMA2D.out_ch[2].conf0.out_rst = 0;
    DMA2D.in_ch1.conf0.in_rst = 1;
    DMA2D.in_ch1.conf0.in_rst = 0;

    //peri_sel
    DMA2D.out_ch[1].peri_sel.out_peri_sel = 0x2;
    DMA2D.out_ch[2].peri_sel.out_peri_sel = 0x3;
    DMA2D.in_ch1.peri_sel.in_peri_sel = 0x2;

    //commonn
    DMA2D.out_ch[1].conf0.out_dscr_port_en = 0x0;
    DMA2D.out_ch[1].conf0.out_auto_wrback = 0x0;
    DMA2D.out_ch[1].conf0.out_eof_mode = 0x0;
    DMA2D.out_ch[1].conf0.out_check_owner = 0x0;
    DMA2D.out_ch[1].conf0.out_mem_burst_length = 0x4;
    DMA2D.out_ch[1].conf0.outdscr_burst_en = 0x1;

    DMA2D.out_ch[2].conf0.out_dscr_port_en = 0x0;
    DMA2D.out_ch[2].conf0.out_auto_wrback = 0x0;
    DMA2D.out_ch[2].conf0.out_eof_mode = 0x0;
    DMA2D.out_ch[2].conf0.out_check_owner = 0x0;
    DMA2D.out_ch[2].conf0.out_mem_burst_length = 0x4;
    DMA2D.out_ch[2].conf0.outdscr_burst_en = 0x1;

    DMA2D.in_ch1.conf0.in_dscr_port_en = 0x0;
    DMA2D.in_ch1.conf0.in_check_owner = 0x0;
    DMA2D.in_ch1.conf0.in_mem_burst_length = 0x4;
    DMA2D.in_ch1.conf0.indscr_burst_en = 0x1;
    //dscr addr
    DMA2D.out_ch[1].link_addr = rx_link[1];
    DMA2D.out_ch[2].link_addr = rx_link[2];
    DMA2D.in_ch1.link_addr = tx_link[1];

#if TEST_INTERRUPT
    dma2d_in_ch0_sem = xSemaphoreCreateBinary( );
    dma2d_in_ch1_sem = xSemaphoreCreateBinary( );
    
    esp_intr_alloc(DMA2D_IN_CH0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, dma2d_ch0_isr, NULL, NULL);
    esp_intr_alloc(DMA2D_IN_CH1_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, dma2d_ch1_isr, NULL, NULL);
#endif

    xTaskCreate(gui_task, "gui_task", 8192 * 2, NULL, 5, NULL);

     while(1) {
        // printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        // printf("PPA.int_raw: %x\n", PPA.int_raw.val);
        // printf("in_int_raw_ch0: %x, in_int_raw_ch1: %x\n", DMA2D.in_int_raw_ch0.val, DMA2D.in_int_raw_ch1.val);
        // printf("in_state_ch0: %x, in_state_ch1: %x\n", DMA2D.in_state_ch0.val, DMA2D.in_state_ch1.val);
        // printf("infifo_status_ch1: 0x%x, infifo_full: %d, infifo_empty: %d, infifo_cnt: %d\n", DMA2D.infifo_status_ch1.val, DMA2D.infifo_status_ch1.infifo_full, DMA2D.infifo_status_ch1.infifo_empty, DMA2D.infifo_status_ch1.infifo_cnt);
        // printf("axi_err: %x\n", DMA2D.axi_err.val);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }   
}

void app_main(void)
{
    xTaskCreate(mipi_task, "mipi_task", 8192, NULL, 5, NULL);
}
