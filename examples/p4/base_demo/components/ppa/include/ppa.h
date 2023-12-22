#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ppa_struct.h"
#include "dma2d_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DMA2D_COLOR_MODE(w) (w == 32) ? 5 : (w == 24) ? 4 : (w == 16) ? 3 : (w == 12) ? 2 : (w == 8) ? 1 : 0
#define PPA_COLOR_MODE(w)   (w == 32) ? 0 : (w == 24) ? 1 : (w == 16) ? 2 : (w == 8) ? 4 : 5

/* this bitfield is start from the LSB!!! */
// owner:表示当前描述符对应的 buffer 允许的操作者。
// • 0:允许操作者为 CPU;
// • 1:允许操作者为 2D-DMA,2D-DMA 使用完该描述符后会将该 bit 清零;
// 对于发送链表描述符来说,由于默认情况下使用完一个描述符是不回写的,因此,默认
// owner 是不会清零的,需要用户配置 DMA2D_OUT_AUTO_WRBACK 使能回写。
// eof: 结束标志。
// • 0:当前描述符不是最后一个描述符;
// • 1:当前描述符是最后一个描述符;
// mod: 数据块读取模式。
// • 0:2D-DMA 只读/写一次水平宽度为 hb,垂直高度为 vb 的图片块;
// • 1:2D-DMA 读/写多次水平宽度为 hb,垂直高度为 vb 的图片块,直到将水平宽度为
// HA,垂直高度为 VA 的图片数据读/写完成;注意:hb/vb/HA/VA 的单位为像素。
// err_eof:接收结束错误标志。
// 对于接收描述符,硬件在收完 1 个包并检测到接收数据错误会将该 bit 置 1。
// 对于发送描述符,该 bit 固定为 0。
// 2DEN:2D 功能使能
// • 0:2D-DMA 的功能与通用 DMA 功能一样,此时,可用于 H264/JPEG 包头数据搬运
// 或者其他外设连续数据段搬运的需求;
// • 1:2D-DMA 2D 功能使能,按照配置的图片块搬运数据;
// length[13:0]/hb:
// • 2DEN 为 0,表示搬运数据的长度(低 14bit)。对于数据接收,该域由硬件接收完
// 一帧数据后填写;对于数据发送,该域表示即将发送的数据长度。单位为 byte。
// • 2DEN 为 1,表示图片块的水平宽度。单位为像素。
// size[13:0]/vb:
// • 2DEN 为 0,表示搬运数据的内存空间大小(低 14bit)。单位为 byte。
// • 2DEN 为 1,表示图片块的垂直高度。单位为像素。
// length[27:14]/HA:
// • 2DEN 为 0,表示搬运数据的长度(高 14bit)。对于数据接收,该域由硬件接收完
// 一帧数据后填写;对于数据发送,该域表示即将发送的数据长度。单位为 byte。
// • 2DEN 为 1,表示描述符对应图片的水平宽度,以像素为单位。
// size[27:14]/VA:
// • 2DEN 为 0,表示搬运数据的内存空间大小(高 14bit)。单位为 byte。
// • 2DEN 为 1,表示描述符对应图片的垂直高度,以像素为单位。
// pbyte:当前通道的搬运图片单个像素占用的字节数。
// • 0:0.5 byte/pixel;
// • 1:1 byte/pixel;
// • 2:1.5 byte/pixel;
// • 3:2 byte/pixel;
// • 4:3 byte/pixel;
// • 5:4 byte/pixel;
// Buffer address pointer:
// 2DEN 为 0 时,buffer 地址指针;
// 2DEN 为 1 时,HA*VA 图片块原点坐标在 buffer 中的地址指针;
// X:
// • 2DEN 为 0 时,该域没有使用;
// • 2DEN 为 1 时,hb*vb 图片块的 X 坐标,单位为像素;
// • 2DEN 为 0 时,该域没有使用;
// • 2DEN 为 1 时,hb*vb 图片块的 Y 坐标,单位为像素;
// Y:
// Next descriptor address:下一个描述符地址指针。
typedef struct lldesc_dma_2d_s {
    volatile uint32_t   vb:14,
                        hb:14,
                   err_eof:1,
                 dma_2d_en:1,
                       eof:1,
                     owner:1;
 
    volatile uint32_t   va:14,
                        ha:14,
                     pbyte:4; //2d mode
 
    volatile uint32_t   y:14,
                        x:14,
                     mode:1,
               reserved29:3;
 
    volatile uint8_t *   buf;
    volatile uint32_t empty;
} lldesc_dma2d_t;

typedef struct {
    uint32_t bilin_x00;
    uint32_t bilin_y00;
    uint32_t bilin_x01;
    uint32_t bilin_y01;
    uint32_t bilin_x10;
    uint32_t bilin_y10;
    uint32_t bilin_x11;
    uint32_t bilin_y11;
    float    bilin_u;
    float    bilin_v;
    float    bilin_1_u;
    float    bilin_1_v;
} rvs_coordi_result_t;

uint32_t blender_A(uint32_t *argb_fg, uint32_t *argb_bg);

uint32_t blender_R(uint32_t *argb_fg, uint32_t *argb_bg);

uint32_t blender_G(uint32_t *argb_fg, uint32_t *argb_bg);

uint32_t blender_B(uint32_t *argb_fg, uint32_t *argb_bg);

rvs_coordi_result_t rvs_coordi_cal(uint32_t x, uint32_t y, float src_w,    float src_h,    float scal_x, float scal_y,
                                        float x_middle, float y_middle, float x_target, float y_target, uint32_t rotate_angle);


uint32_t bilin_inter_cal(uint32_t f00, uint32_t f01, uint32_t f10, uint32_t f11, float u, float v);

void dma2d_link_dscr_init(uint32_t *head, uint32_t *next, int *buf_ptr, uint32_t ha, uint32_t va, uint32_t hb, uint32_t vb, uint32_t eof, uint32_t en_2d, uint32_t pbyte, uint32_t mod, uint32_t bias_x, uint32_t bias_y);

uint32_t dma2d_access_addr_map(uint32_t addr);

#ifdef __cplusplus
}
#endif