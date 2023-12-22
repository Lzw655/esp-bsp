#include "ppa.h"

uint32_t dma2d_access_addr_map(uint32_t addr)
{
    uint32_t map_addr = addr;

    return map_addr;
}

void dma2d_link_dscr_init(uint32_t *head, uint32_t *next, int *buf_ptr, uint32_t ha, uint32_t va, uint32_t hb, uint32_t vb, uint32_t eof, uint32_t en_2d, uint32_t pbyte, uint32_t mod, uint32_t bias_x, uint32_t bias_y)
{
    lldesc_dma2d_t *dma2d = head;
    memset(dma2d, 0, sizeof(lldesc_dma2d_t));
    dma2d->owner = 0x1;
    dma2d->eof = eof;
    dma2d->dma_2d_en = en_2d;
    dma2d->err_eof = 0x0;
    dma2d->hb = hb;
    dma2d->vb = vb;
    dma2d->pbyte = pbyte;
    dma2d->ha = ha;
    dma2d->va = va;
    dma2d->mode = mod;
    dma2d->y = bias_y;
    dma2d->x = bias_x;
    dma2d->buf = dma2d_access_addr_map(buf_ptr);
    dma2d->empty = next;
}

uint32_t blender_A(uint32_t *argb_fg, uint32_t *argb_bg)
{

    uint32_t A, R, G, B;
    uint32_t alpha_mult;

    uint32_t a_fg, r_fg, g_fg, b_fg;
    uint32_t a_bg, r_bg, g_bg, b_bg;

    a_fg = * argb_fg >> 24;
    r_fg = (* argb_fg >> 16) & 0x000000FF;
    g_fg = (* argb_fg >> 8) & 0x000000FF;
    b_fg = * argb_fg & 0x000000FF;

    a_bg = * argb_bg >> 24;
    r_bg = (* argb_bg >> 16) & 0x000000FF;
    g_bg = (* argb_bg >> 8) & 0x000000FF;
    b_bg = * argb_bg & 0x000000FF;


    alpha_mult = a_fg * a_bg / 256;
    A = a_fg + a_bg - alpha_mult;
    //R = (r_fg*a_fg + r_bg*a_bg - r_bg*alpha_mult)/A;
    //G = (g_fg*a_fg + g_bg*a_bg - g_bg*alpha_mult)/A;
    //B = (b_fg*a_fg + b_bg*a_bg - b_bg*alpha_mult)/A;

    //printf("A is: %x!\n", A);

    return (A);
}

uint32_t blender_R(uint32_t *argb_fg, uint32_t *argb_bg)
{

    uint32_t A, R, G, B;
    uint32_t alpha_mult;

    uint32_t a_fg, r_fg, g_fg, b_fg;
    uint32_t a_bg, r_bg, g_bg, b_bg;

    a_fg = * argb_fg >> 24;
    r_fg = (* argb_fg >> 16) & 0x000000FF;
    g_fg = (* argb_fg >> 8) & 0x000000FF;
    b_fg = * argb_fg & 0x000000FF;

    a_bg = * argb_bg >> 24;
    r_bg = (* argb_bg >> 16) & 0x000000FF;
    g_bg = (* argb_bg >> 8) & 0x000000FF;
    b_bg = * argb_bg & 0x000000FF;


    alpha_mult = a_fg * a_bg / 256;
    A = a_fg + a_bg - alpha_mult;
    R = (r_fg * a_fg + r_bg * a_bg - r_bg * alpha_mult) / A;
    //G = (g_fg*a_fg + g_bg*a_bg - g_bg*alpha_mult)/A;
    //B = (b_fg*a_fg + b_bg*a_bg - b_bg*alpha_mult)/A;

    //printf("R is: %x!\n", R);

    return (R);
}

uint32_t blender_G(uint32_t *argb_fg, uint32_t *argb_bg)
{

    uint32_t A, R, G, B;
    uint32_t alpha_mult;

    uint32_t a_fg, r_fg, g_fg, b_fg;
    uint32_t a_bg, r_bg, g_bg, b_bg;

    a_fg = * argb_fg >> 24;
    r_fg = (* argb_fg >> 16) & 0x000000FF;
    g_fg = (* argb_fg >> 8) & 0x000000FF;
    b_fg = * argb_fg & 0x000000FF;

    a_bg = * argb_bg >> 24;
    r_bg = (* argb_bg >> 16) & 0x000000FF;
    g_bg = (* argb_bg >> 8) & 0x000000FF;
    b_bg = * argb_bg & 0x000000FF;


    alpha_mult = a_fg * a_bg / 256;
    A = a_fg + a_bg - alpha_mult;
    //R = (r_fg*a_fg + r_bg*a_bg - r_bg*alpha_mult)/A;
    G = (g_fg * a_fg + g_bg * a_bg - g_bg * alpha_mult) / A;
    //B = (b_fg*a_fg + b_bg*a_bg - b_bg*alpha_mult)/A;

    //printf("G is: %x!\n", G);

    return (G);
}

uint32_t blender_B(uint32_t *argb_fg, uint32_t *argb_bg)
{

    uint32_t A, R, G, B;
    uint32_t alpha_mult;

    uint32_t a_fg, r_fg, g_fg, b_fg;
    uint32_t a_bg, r_bg, g_bg, b_bg;

    a_fg = * argb_fg >> 24;
    r_fg = (* argb_fg >> 16) & 0x000000FF;
    g_fg = (* argb_fg >> 8) & 0x000000FF;
    b_fg = * argb_fg & 0x000000FF;

    a_bg = * argb_bg >> 24;
    r_bg = (* argb_bg >> 16) & 0x000000FF;
    g_bg = (* argb_bg >> 8) & 0x000000FF;
    b_bg = * argb_bg & 0x000000FF;


    alpha_mult = a_fg * a_bg / 256;
    A = a_fg + a_bg - alpha_mult;
    //R = (r_fg*a_fg + r_bg*a_bg - r_bg*alpha_mult)/A;
    //G = (g_fg*a_fg + g_bg*a_bg - g_bg*alpha_mult)/A;
    B = (b_fg * a_fg + b_bg * a_bg - b_bg * alpha_mult) / A;

    //printf("B is: %x!\n", B);

    return (B);
}

rvs_coordi_result_t rvs_coordi_cal(uint32_t x, uint32_t y, float src_w,    float src_h,    float scal_x, float scal_y,
                                        float x_middle, float y_middle, float x_target, float y_target, uint32_t rotate_angle)
{

    rvs_coordi_result_t result;
    float sin_a = 0, cos_a = 0;
    float x_src = 0, y_src = 0;

    if (rotate_angle == 0) { // 0 degree
        //sin_a = 0;
        //cos_a = 1;
        x_src = ((x + 0.5) - x_target  + x_middle) / scal_x - 0.5;
        y_src = ((y + 0.5) - y_target  + y_middle) / scal_y - 0.5;
    } else if (rotate_angle == 1) { // 90 degree
        //sin_a = 1;
        //cos_a = 0;
        x_src = (x_middle - (y + 0.5) + y_target) / scal_x - 0.5;
        y_src = (y_middle + (x + 0.5) - x_target) / scal_y - 0.5;
    } else if (rotate_angle == 2) { // 180 degree
        //sin_a = 0;
        //cos_a = -1;
        x_src = (x_middle - (x + 0.5)  + x_target) / scal_x - 0.5;
        y_src = (y_middle - (y + 0.5)  + y_target) / scal_y - 0.5;
    } else if (rotate_angle == 3) { // 270 degree
        //sin_a = -1;
        //cos_a = 0;
        x_src = (x_middle + (y + 0.5) - y_target) / scal_x - 0.5;
        y_src = (y_middle - (x + 0.5) + x_target) / scal_y - 0.5;
    }

    if (x_src < 0) {
        result.bilin_u   = 0;
        result.bilin_1_u = 1;
        result.bilin_x00 = 0;
        result.bilin_x01 = 0;
        result.bilin_x10 = 1;
        result.bilin_x11 = 1;
    } else if (x_src > (src_w - 0.5)) {
        result.bilin_u   = 1;
        result.bilin_1_u = 0;
        result.bilin_x00 = src_w - 1;
        result.bilin_x01 = src_w - 1;
        result.bilin_x10 = src_w;
        result.bilin_x11 = src_w;
    } else {
        result.bilin_u   = x_src - (uint32_t)(x_src);
        result.bilin_1_u = 1 - result.bilin_u;
        result.bilin_x00 = (uint32_t)(x_src);
        result.bilin_x01 = (uint32_t)(x_src);
        result.bilin_x10 = (uint32_t)(x_src) + 1;
        result.bilin_x11 = (uint32_t)(x_src) + 1;
    }

    if (y_src < 0) {
        result.bilin_v   = 0;
        result.bilin_1_v = 1;
        result.bilin_y00 = 0;
        result.bilin_y01 = 1;
        result.bilin_y10 = 0;
        result.bilin_y11 = 1;
    } else if (y_src > (src_h - 0.5)) {
        result.bilin_v   = 1;
        result.bilin_1_v = 0;
        result.bilin_y00 = src_h - 1;
        result.bilin_y01 = src_h;
        result.bilin_y10 = src_h - 1;
        result.bilin_y11 = src_h;
    } else {
        result.bilin_v   = y_src - (uint32_t)(y_src);
        result.bilin_1_v = 1 - result.bilin_v;
        result.bilin_y00 = (uint32_t)(y_src);
        result.bilin_y01 = (uint32_t)(y_src) + 1;
        result.bilin_y10 = (uint32_t)(y_src);
        result.bilin_y11 = (uint32_t)(y_src) + 1;
    }

    return (result);
}


uint32_t bilin_inter_cal(uint32_t f00, uint32_t f01, uint32_t f10, uint32_t f11, float u, float v)
{
    float result_tmp;
    uint32_t result;


    result_tmp = (f00 * (1 - u) * (1 - v)) + (f01 * (1 - u) * v) + (f10 * u * (1 - v)) + (f11 * u * v);
    result = (uint32_t)result_tmp;

    return (result);
}


