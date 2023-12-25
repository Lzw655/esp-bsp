#include "isp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/portmacro.h"
#include "pid.h"

#if TEST_CSI_AWB
static uint32_t ori_ccm_coef[3][3] = {0};

static int isp_awb_config()
{
    ISP.awb_mode.awb_mode = 0x3;

    //窗口配置

    ISP.awb_hscale.awb_lpoint = (MIPI_CSI_DISP_HSIZE / 2) - 50; // 窗口左坐标
    ISP.awb_hscale.awb_rpoint = (MIPI_CSI_DISP_HSIZE / 2) + 49; // 窗口右坐标
    ISP.awb_vscale.awb_tpoint = (MIPI_CSI_DISP_VSIZE / 2) - 50; // 窗口上坐标
    ISP.awb_vscale.awb_bpoint = (MIPI_CSI_DISP_VSIZE / 2) + 49; // 窗口下坐标

    //白点约束配置
    ISP.awb_th_lum.awb_max_lum = 735; // r+g+b亮度上限 245×3
    ISP.awb_th_lum.awb_min_lum = 360; // r+g+b亮度下限 120×3
    ISP.awb_th_rg.awb_max_rg = 384; // r/g亮度上限 1.5*2^8
    ISP.awb_th_rg.awb_min_rg = 128; // r/g亮度下限 0.5*2^8
    ISP.awb_th_bg.awb_max_bg = 384; // b/g亮度上限
    ISP.awb_th_bg.awb_min_bg = 128; // b/g亮度下限

    ori_ccm_coef[0][0] = 0x0500;
    ori_ccm_coef[0][1] = 0x1080;
    ori_ccm_coef[0][2] = 0x1080;
    ori_ccm_coef[1][0] = 0x1080;
    ori_ccm_coef[1][1] = 0x0500;
    ori_ccm_coef[1][2] = 0x1080;
    ori_ccm_coef[2][0] = 0x1080;
    ori_ccm_coef[2][1] = 0x1080;
    ori_ccm_coef[2][2] = 0x0500;

    printf("ori_ccm_coef: \n");

    for (int y = 0; y < 3; y++) {
        printf("[");

        for (int x = 0; x < 3; x++) {
            printf("%d ", ori_ccm_coef[y][x]);
        }

        printf("]\n");
    }

    ISP.cntl.awb_en = 1;
    return 0;
}

static int isp_awb_update()
{
    while (!ISP.int_raw.awb_fdone);

    ISP.int_clr.awb_fdone = 0x1;
    ISP.cntl.awb_en = 0;

    //最初没有白平衡的ccm值
    //每次调白平衡时，该值不变化

    uint32_t new_ccm_coef[3][3] = {0};

    uint32_t awb_white_cnt = ISP.awb0_white_cnt.awb0_white_cnt; // 白点数量
    uint32_t awb_acc_r = ISP.awb0_acc_r; // R通道亮度和
    uint32_t awb_acc_g = ISP.awb0_acc_g; // G通道亮度和
    uint32_t awb_acc_b = ISP.awb0_acc_b; // B通道亮度和
    float gain_r = 1.0;
    float gain_b = 1.0;

    uint32_t th = MIPI_CSI_DISP_HSIZE * MIPI_CSI_DISP_VSIZE;
    uint32_t tl = 0;

    if ((awb_white_cnt < th) & (awb_white_cnt > tl)) {
        //白点数量在一定范围内认为窗口统计有效
        gain_r = (float)awb_acc_g / awb_acc_r; //R通道增益
        gain_b = (float)awb_acc_g / awb_acc_b; //B通道增益

        //R通道，i表示第i行，RR、GR、BR乘gain_r
        for (int x = 0; x < 3; x++) {
            if (ori_ccm_coef[x][0] >= 4096) { //负数
                new_ccm_coef[x][0] = (uint32_t)((ori_ccm_coef[x][0] - 4096) * gain_r); //去掉符号位相乘

                if (new_ccm_coef[x][0] > 4095) { //防止溢出
                    new_ccm_coef[x][0] = 4095;
                }

                new_ccm_coef[x][0] = new_ccm_coef[x][0] + 4096; //加回符号位
            } else {
                new_ccm_coef[x][0] = (uint32_t)((ori_ccm_coef[x][0]) * gain_r); //正数直接相乘

                if (new_ccm_coef[x][0] > 4095) { //防止溢出
                    new_ccm_coef[x][0] = 4095;
                }
            }
        }

        // G通道，第2列（RG、GG、BG）不变
        for (int x = 0; x < 3; x++) {
            new_ccm_coef[x][1] = ori_ccm_coef[x][1];
        }

        //B通道，i表示第i行，RB、GB、BB乘gain_b
        for (int x = 0; x < 3; x++) {
            if (ori_ccm_coef[x][2] >= 4096) { //负数
                new_ccm_coef[x][2] = (uint32_t)((ori_ccm_coef[x][2] - 4096) * gain_b); //去掉符号位相乘

                if (new_ccm_coef[x][2] > 4095) { //防止溢出
                    new_ccm_coef[x][2] = 4095;
                }

                new_ccm_coef[x][2] = new_ccm_coef[x][2] + 4096; //加回符号位
            } else {
                new_ccm_coef[x][2] = (uint32_t)((ori_ccm_coef[x][2]) * gain_b); //正数直接相乘

                if (new_ccm_coef[x][2] > 4095) { //防止溢出
                    new_ccm_coef[x][2] = 4095;
                }
            }
        }

        ISP.ccm_coef0.ccm_coef_rr = new_ccm_coef[0][0];
        ISP.ccm_coef0.ccm_coef_rg = new_ccm_coef[0][1];
        ISP.ccm_coef1.ccm_coef_rb = new_ccm_coef[0][2];
        ISP.ccm_coef1.ccm_coef_gr = new_ccm_coef[1][0];
        ISP.ccm_coef3.ccm_coef_gg = new_ccm_coef[1][1];
        ISP.ccm_coef3.ccm_coef_gb = new_ccm_coef[1][2];
        ISP.ccm_coef4.ccm_coef_br = new_ccm_coef[2][0];
        ISP.ccm_coef4.ccm_coef_bg = new_ccm_coef[2][1];
        ISP.ccm_coef5.ccm_coef_bb = new_ccm_coef[2][2];

        printf("new_ccm_coef: \n");

        for (int y = 0; y < 3; y++) {
            printf("[");

            for (int x = 0; x < 3; x++) {
                printf("%d ", new_ccm_coef[y][x]);
            }

            printf("]\n");
        }
    }

    ISP.cntl.awb_en = 1;
    printf("awb_white_cnt: %d, awb_acc_r: %d, awb_acc_g: %d, awb_acc_b: %d\n", awb_white_cnt, awb_acc_r, awb_acc_g, awb_acc_b);
    printf("gain_r: %d.%d, gain_b: %d.%d\n", (int)gain_r, (int)(gain_r * 1000) % 1000, (int)gain_b, (int)(gain_b * 1000) % 1000);

    return 0;
}
#endif

#if TEST_CSI_AE

float exposure_line_50hz;
static pid_ctrl_t pid_eg;
uint32_t ae_weight[25];
uint32_t ae_weight_sum = 0;

static int isp_ae_config()
{
    ISP.ae_bx.ae_x_start = (MIPI_CSI_DISP_HSIZE / 2) - (MIPI_CSI_DISP_HSIZE / 5 - 1) * 2.5;
    ISP.ae_bx.ae_x_bsize = MIPI_CSI_DISP_HSIZE / 5 - 1;
    ISP.ae_by.ae_y_start = (MIPI_CSI_DISP_VSIZE / 2) - (MIPI_CSI_DISP_VSIZE / 5 - 1) * 2.5;
    ISP.ae_by.ae_y_bsize = MIPI_CSI_DISP_VSIZE / 5 - 1;

    ISP.cntl.ae_en = 0;
    printf("[ae_config] ae_x_start: %d, ae_x_bsize: %d\n", ISP.ae_bx.ae_x_start, ISP.ae_bx.ae_x_bsize);
    printf("[ae_config] ae_y_start: %d, ae_y_bsize: %d\n", ISP.ae_by.ae_y_start, ISP.ae_by.ae_y_bsize);

    exposure_line_50hz = (1 / (50.0 * 2)) * 21.21 * 984; // (1 / (50Hz * 2)) * FPS * VTS = 0.01 * 21.21 * 984
    printf("[ae_config] exposure_line_50hz: %d.%d\n", (int)exposure_line_50hz, abs((int)(exposure_line_50hz * 1000) % 1000));

    float exposure_line = exposure_line_50hz;
    uint32_t exposure = ((uint16_t)exposure_line << 4) | ((uint16_t)(exposure_line * 16) % 16);
    uint32_t agc = 50;
    uint8_t exposure_reg[3];
    uint8_t agc_reg[2];

    exposure_reg[0] = (exposure >> 16) & 0x7;
    exposure_reg[1] = (exposure >> 8) & 0xFF;
    exposure_reg[2] = (exposure) & 0xFF;
    ov5647_write(0x3500, exposure_reg[0]);
    ov5647_write(0x3501, exposure_reg[1]);
    ov5647_write(0x3502, exposure_reg[2]);
    agc_reg[0] = (agc >> 8) & 0x3;
    agc_reg[1] = (agc) & 0xFF;
    ov5647_write(0x350A, agc_reg[0]);
    ov5647_write(0x350B, agc_reg[1]);

    pid_init(&pid_eg);
    pid_set_gains(&pid_eg, 10000, 10.0, 1.0);

    for (int x = 0; x < 25; x++) {
        ae_weight[x] = 1;
        ae_weight_sum = ae_weight_sum + ae_weight[x];
    }

    printf("[ae_config] ae_weight_sum: %d\n", ae_weight_sum);
    printf("[ae_config] hadr: %d, vadr: %d\n", ISP.frame_cfg.hadr_num, ISP.frame_cfg.vadr_num);

    return 0;
}


static int isp_ae_update()
{
    float lum = 0;
    float ae_target = 120;
    float ae_th = 122;
    float ae_tl = 118;
    uint8_t exposure_reg[3] = {0};
    float exposure_line;
    uint32_t exposure;
    uint8_t agc_reg[2] = {0};
    uint32_t agc;
    float error_eg;
    float pid_eg_val;
    float eg;
    float eg_new;
    float eg_fix;
    float e_ref = exposure_line_50hz;
    float e_max = 10 * exposure_line_50hz;
    float e_min = 10;
    uint32_t g_ref = 50;
    uint32_t g_max = 1000;
    uint32_t g_min = 10;
    float eg_ref = e_ref * g_ref;
    float eg_max = e_max * g_max;
    float eg_min = e_min * g_min;
    float eg_pid_p = 1;
    uint8_t ae_state_cnt = 0;
    uint32_t lum_array[25] = {0};
    uint32_t lum_array_tmp[10][25] = {0};


    ISP.int_clr.ae_frame_done = 0x1;
    ISP.cntl.ae_en = 1;
    ISP.ae_ctrl.ae_update = 1;
    while (ISP.ae_ctrl.ae_update);

    while (!ISP.int_raw.ae_frame_done);

    ISP.int_clr.ae_frame_done = 0x1;
    ISP.cntl.ae_en = 0;

    lum = 0;
    lum_array[0] = ISP.ae_block_mean_0.ae_b00_mean;
    lum_array[1] = ISP.ae_block_mean_0.ae_b01_mean;
    lum_array[2] = ISP.ae_block_mean_0.ae_b02_mean;
    lum_array[3] = ISP.ae_block_mean_0.ae_b03_mean;
    lum_array[4] = ISP.ae_block_mean_1.ae_b04_mean;
    lum_array[5] = ISP.ae_block_mean_1.ae_b10_mean;
    lum_array[6] = ISP.ae_block_mean_1.ae_b11_mean;
    lum_array[7] = ISP.ae_block_mean_1.ae_b12_mean;
    lum_array[8] = ISP.ae_block_mean_2.ae_b13_mean;
    lum_array[9] = ISP.ae_block_mean_2.ae_b14_mean;
    lum_array[10] = ISP.ae_block_mean_2.ae_b20_mean;
    lum_array[11] = ISP.ae_block_mean_2.ae_b21_mean;
    lum_array[12] = ISP.ae_block_mean_3.ae_b22_mean;
    lum_array[13] = ISP.ae_block_mean_3.ae_b23_mean;
    lum_array[14] = ISP.ae_block_mean_3.ae_b24_mean;
    lum_array[15] = ISP.ae_block_mean_3.ae_b30_mean;
    lum_array[16] = ISP.ae_block_mean_4.ae_b31_mean;
    lum_array[17] = ISP.ae_block_mean_4.ae_b32_mean;
    lum_array[18] = ISP.ae_block_mean_4.ae_b33_mean;
    lum_array[19] = ISP.ae_block_mean_4.ae_b34_mean;
    lum_array[20] = ISP.ae_block_mean_5.ae_b40_mean;
    lum_array[21] = ISP.ae_block_mean_5.ae_b41_mean;
    lum_array[22] = ISP.ae_block_mean_5.ae_b42_mean;
    lum_array[23] = ISP.ae_block_mean_5.ae_b43_mean;
    lum_array[24] = ISP.ae_block_mean_6.ae_b44_mean;

    for (int lum_cnt = 0; lum_cnt < 25; lum_cnt++) {
        lum = lum + ae_weight[lum_cnt] * lum_array[lum_cnt];
        printf("lum_array[%d]: %d ", lum_cnt, lum_array[lum_cnt]);

        if (lum_cnt % 5 == 4) {
            printf("\n");
        }
    }

    printf("lum_sum: %d.%d\n", (int)lum, abs((int)(lum * 1000) % 1000));
    lum = lum / ae_weight_sum;
    printf("lum: %d.%d\n", (int)lum, abs((int)(lum * 1000) % 1000));

    // read current exposure_line
    ov5647_read(0x3500, &exposure_reg[0]);
    ov5647_read(0x3501, &exposure_reg[1]);
    ov5647_read(0x3502, &exposure_reg[2]);
    exposure = ((exposure_reg[0] & 0x7) << 16) | ((exposure_reg[1] & 0xFF) << 8) | ((exposure_reg[2] & 0xFF));
    exposure_line = (float)(exposure >> 4) + (float)(exposure & 0x0F) / 16.0;
    // read current agc
    ov5647_read(0x350A, &agc_reg[0]);
    ov5647_read(0x350B, &agc_reg[1]);
    agc = ((agc_reg[0] & 0x03) << 8) | ((agc_reg[1] & 0xFF));

    eg = ((float)exposure_line) * ((float)agc);

    printf("current exposeure: %d, exposure line: %d.%d\n", exposure, (int)exposure_line, abs((int)(exposure_line * 1000) % 1000));
    printf("current agc: %d\n", agc);
    printf("current eg: %d.%d\n", (int)eg, abs((int)(eg * 1000) % 1000));
    printf("current lum: %d.%d\n", (int)lum, abs((int)(lum * 1000) % 1000));

    if (((lum > ae_th) && (~((exposure_line <= e_min) && (agc <= g_min)))) ||
            ((lum < ae_tl) && (~((exposure_line >= e_max) && (agc >= g_max))))) {
        printf("//------------------ ae ------------------//\n");
        ae_state_cnt = 0;

        while (1) {
            //read lum
            ISP.int_clr.ae_frame_done = 0x1;
            ISP.cntl.ae_en = 1;
            ISP.ae_ctrl.ae_update = 1;
            while (ISP.ae_ctrl.ae_update);

            while (!ISP.int_raw.ae_frame_done);

            ISP.int_clr.ae_frame_done = 0x1;
            ISP.cntl.ae_en = 0;



            lum = 0;
            lum_array[0] = ISP.ae_block_mean_0.ae_b00_mean;
            lum_array[1] = ISP.ae_block_mean_0.ae_b01_mean;
            lum_array[2] = ISP.ae_block_mean_0.ae_b02_mean;
            lum_array[3] = ISP.ae_block_mean_0.ae_b03_mean;
            lum_array[4] = ISP.ae_block_mean_1.ae_b04_mean;
            lum_array[5] = ISP.ae_block_mean_1.ae_b10_mean;
            lum_array[6] = ISP.ae_block_mean_1.ae_b11_mean;
            lum_array[7] = ISP.ae_block_mean_1.ae_b12_mean;
            lum_array[8] = ISP.ae_block_mean_2.ae_b13_mean;
            lum_array[9] = ISP.ae_block_mean_2.ae_b14_mean;
            lum_array[10] = ISP.ae_block_mean_2.ae_b20_mean;
            lum_array[11] = ISP.ae_block_mean_2.ae_b21_mean;
            lum_array[12] = ISP.ae_block_mean_3.ae_b22_mean;
            lum_array[13] = ISP.ae_block_mean_3.ae_b23_mean;
            lum_array[14] = ISP.ae_block_mean_3.ae_b24_mean;
            lum_array[15] = ISP.ae_block_mean_3.ae_b30_mean;
            lum_array[16] = ISP.ae_block_mean_4.ae_b31_mean;
            lum_array[17] = ISP.ae_block_mean_4.ae_b32_mean;
            lum_array[18] = ISP.ae_block_mean_4.ae_b33_mean;
            lum_array[19] = ISP.ae_block_mean_4.ae_b34_mean;
            lum_array[20] = ISP.ae_block_mean_5.ae_b40_mean;
            lum_array[21] = ISP.ae_block_mean_5.ae_b41_mean;
            lum_array[22] = ISP.ae_block_mean_5.ae_b42_mean;
            lum_array[23] = ISP.ae_block_mean_5.ae_b43_mean;
            lum_array[24] = ISP.ae_block_mean_6.ae_b44_mean;

            for (int lum_cnt = 0; lum_cnt < 25; lum_cnt++) {
                lum = lum + ae_weight[lum_cnt] * lum_array[lum_cnt];
                printf("[AEC] lum_array[%d]: %d ", lum_cnt, lum_array[lum_cnt]);

                if (lum_cnt % 5 == 4) {
                    printf("\n");
                }
            }

            printf("[AEC] lum_sum: %d.%d\n", (int)lum, abs((int)(lum * 1000) % 1000));
            lum = lum / ae_weight_sum;
            printf("[AEC] lum: %d.%d\n", (int)lum, abs((int)(lum * 1000) % 1000));

            // read current exposure_line
            ov5647_read(0x3500, &exposure_reg[0]);
            ov5647_read(0x3501, &exposure_reg[1]);
            ov5647_read(0x3502, &exposure_reg[2]);
            exposure = ((exposure_reg[0] & 0x7) << 16) | ((exposure_reg[1] & 0xFF) << 8) | ((exposure_reg[2] & 0xFF));
            exposure_line = (float)(exposure >> 4) + (float)(exposure & 0x0F) / 16.0;
            // read current agc
            ov5647_read(0x350A, &agc_reg[0]);
            ov5647_read(0x350B, &agc_reg[1]);
            agc = ((agc_reg[0] & 0x03) << 8) | ((agc_reg[1] & 0xFF));

            eg = ((float)exposure_line) * ((float)agc);
            printf("[AEC] current exposeure: %d, exposure line: %d.%d\n", exposure, (int)exposure_line, abs((int)(exposure_line * 1000) % 1000));
            printf("[AEC] current agc: %d\n", agc);
            printf("[AEC] current eg: %d.%d\n", (int)eg, abs((int)(eg * 1000) % 1000));
            printf("[AEC] current lum: %d.%d\n", (int)lum, abs((int)(lum * 1000) % 1000));

            // calculate new eg
            error_eg = (float)lum - (float)ae_target;
            eg_pid_p = eg / (float)lum; //abs(error_eg) * eg / (float)lum; // err/(lum/eg)
            eg_pid_p = eg_pid_p * 0.5;
            pid_set_gains(&pid_eg, eg_pid_p, 0.0, 0.0);
            pid_eg_val = pid_process(&pid_eg, error_eg);
            eg_new = eg + pid_eg_val;

            if (eg_new <= 0) {
                exposure_line = e_min;
                agc = g_min;
            } else if ((eg_new > 0) && (eg_new <= e_ref * g_ref)) {
                if (eg_new < e_min * g_min) {
                    exposure_line = e_min;
                    agc = g_min;
                } else if (eg_new < e_ref * g_min) {
                    exposure_line = eg_new / g_min;
                    agc = g_min;
                } else {
                    exposure_line = e_ref;
                    agc = (uint32_t)(eg_new / e_ref);
                }
            } else if (eg_new > e_ref * g_ref) {
                if (eg_new > e_max * g_max) {
                    exposure_line = e_max;
                    agc = g_max;
                } else if (eg_new > e_max * g_ref) {
                    exposure_line = e_max;
                    agc = (uint32_t)(eg_new / e_max);
                } else {
                    eg_fix = eg_new / ((float)(e_ref * g_ref));
                    exposure_line = (uint32_t)(eg_fix) * e_ref;
                    agc = eg_fix / ((uint32_t)eg_fix) * g_ref;
                }
            }

            exposure = ((uint32_t)exposure_line << 4) | ((uint32_t)(exposure_line * 16) % 16);

            // write back
            exposure_reg[0] = (exposure >> 16) & 0x7;
            exposure_reg[1] = (exposure >> 8) & 0xFF;
            exposure_reg[2] = (exposure) & 0xFF;
            ov5647_write(0x3500, exposure_reg[0]);
            ov5647_write(0x3501, exposure_reg[1]);
            ov5647_write(0x3502, exposure_reg[2]);

            agc_reg[0] = (agc >> 8) & 0x3;
            agc_reg[1] = (agc) & 0xFF;
            ov5647_write(0x350A, agc_reg[0]);
            ov5647_write(0x350B, agc_reg[1]);


            if ((lum < ae_th) && (lum > ae_tl)) {
                ae_state_cnt += 1;

                if (ae_state_cnt >= 2) {
                    break;
                }
            } else {
                if (ae_state_cnt > 0) {
                    ae_state_cnt -= 1;
                }
            }

            printf("\n");
        }

        printf("//------------------ ae done ------------------//\n");
    }

    printf("\n");
    printf("\n");

    return 0;
}
#endif

#if TEST_CSI_AF

static int isp_af_config()
{
    ISP.af_hscale_a.af_lpoint_a = (MIPI_CSI_DISP_HSIZE / 2) - 100;
    ISP.af_hscale_a.af_rpoint_a = (MIPI_CSI_DISP_HSIZE / 2) + 99;
    ISP.af_vscale_a.af_tpoint_a = (MIPI_CSI_DISP_VSIZE / 2) - 100;
    ISP.af_vscale_a.af_bpoint_a = (MIPI_CSI_DISP_VSIZE / 2) + 99;
    ISP.af_hscale_b.af_lpoint_b = (MIPI_CSI_DISP_HSIZE / 2) - 100;
    ISP.af_hscale_b.af_rpoint_b = (MIPI_CSI_DISP_HSIZE / 2) + 99;
    ISP.af_vscale_b.af_tpoint_b = (MIPI_CSI_DISP_VSIZE / 2) - 100;
    ISP.af_vscale_b.af_bpoint_b = (MIPI_CSI_DISP_VSIZE / 2) + 99;
    ISP.af_hscale_c.af_lpoint_c = (MIPI_CSI_DISP_HSIZE / 2) - 100;
    ISP.af_hscale_c.af_rpoint_c = (MIPI_CSI_DISP_HSIZE / 2) + 99;
    ISP.af_vscale_c.af_tpoint_c = (MIPI_CSI_DISP_VSIZE / 2) - 100;
    ISP.af_vscale_c.af_bpoint_c = (MIPI_CSI_DISP_VSIZE / 2) + 99;

    ISP.af_env_user_th_sum = 0x0;
    ISP.af_env_user_th_lum.af_env_user_threshold_lum = 0x0;
    ISP.af_ctrl0.af_env_threshold = 2; // 阈值为上一次对焦完成后的1/16
    ISP.af_ctrl0.af_env_period = 10; // 隔30帧检测一次
    ISP.af_env_user_th_sum = 1;
    ISP.af_env_user_th_lum.af_env_user_threshold_lum = 1;
    ISP.af_threshold.af_threshold = 128;
    ISP.af_ctrl0.af_auto_update = 0;

    ISP.cntl.af_en = 1;
    ISP.int_clr.af_env = 0x1;
    ISP.int_clr.af_fdone = 0x1;

    return 0;
}

static int isp_af_update()
{
    uint32_t af_sum = 0;
    uint32_t af_lum = 0;
    uint32_t af_sum_max = 0;
    float af_current_best = 0;
    uint32_t sum[3] = {0};
    uint32_t lum[3] = {0};

    uint32_t af_current_base = 0;
    uint32_t af_current = 0;

    uint32_t af_sum_env_th = 0;
    uint32_t af_lum_env_th = 0;
    uint32_t af_sum_tmp[30] = {0};
    uint32_t af_lum_tmp[30] = {0};
    uint32_t af_sum_buf[2] = {0};
    uint32_t ref_x = 29;

    sum[0] = ISP.af_sum_a.af_suma;
    sum[1] = ISP.af_sum_b.af_sumb;
    sum[2] = ISP.af_sum_c.af_sumc;
    lum[0] = ISP.af_lum_a.af_luma;
    lum[1] = ISP.af_lum_b.af_lumb;
    lum[2] = ISP.af_lum_c.af_lumc;
    af_sum = sum[0] + sum[1] + sum[2];
    af_lum = lum[0] + lum[1] + lum[2];
    printf("read, af_sum: %d, af_lum: %d\n", af_sum, af_lum);

    if (ISP.int_raw.af_env) {
        ISP.cntl.af_en = 1;
        ISP.int_clr.af_env = 0x1;
        dw9714_set_current(0);
        printf("//----------- af start ----------//\n");

        // first search
        printf("//----------- first search ----------//\n");
        af_sum_max = 0;
        af_current_base = 0;

        for (int x = 0; x <= 10; x++) {
            af_current = af_current_base + x * 12;
            dw9714_set_current(af_current);
            ISP.af_ctrl0.af_manual_update = 1;

            while (!ISP.int_raw.af_fdone);

            ISP.int_clr.af_fdone = 0x1;
            sum[0] = ISP.af_sum_a.af_suma;
            sum[1] = ISP.af_sum_b.af_sumb;
            sum[2] = ISP.af_sum_c.af_sumc;
            af_sum = sum[0] + sum[1] + sum[2];

            if (af_sum > af_sum_max) {
                af_sum_max = af_sum;
                af_current_best = af_current;
            }

            printf("af_sum: %d, ", af_sum);
            printf("af_current: %d.%d\n", (int)af_current, (int)((int)(af_current * 1000) % 1000));
        }

        // second search
        printf("//----------- second search ----------//\n");
        af_sum_max = 0;
        af_current_base = af_current_best + 10;

        if (af_current_base > 120) {
            af_current_base = 120;
        }

        for (int x = 0; x <= 10; x++) {
            af_current = af_current_base - x * 2;

            if (af_current < 0) {
                af_current = 0;
            }

            dw9714_set_current(af_current);
            ISP.af_ctrl0.af_manual_update = 1;

            while (!ISP.int_raw.af_fdone);

            ISP.int_clr.af_fdone = 0x1;
            sum[0] = ISP.af_sum_a.af_suma;
            sum[1] = ISP.af_sum_b.af_sumb;
            sum[2] = ISP.af_sum_c.af_sumc;
            af_sum = sum[0] + sum[1] + sum[2];

            if (af_sum > af_sum_max) {
                af_sum_max = af_sum;
                af_current_best = af_current;
            }

            //else if(af_sum < af_sum_max*0.5) {
            // break;
            //}
            printf("af_sum: %d, ", af_sum);
            printf("af_current: %d.%d\n", (int)af_current, (int)((int)(af_current * 1000) % 1000));
        }

        // af done
        printf("//----------- af done ----------//\n");
        printf("af_sum_max: %d, ", af_sum_max);
        printf("af_current_best: %d.%d\n", (int)af_current_best, (int)((int)(af_current_best * 1000) % 1000));
        dw9714_set_current(af_current_best);

        // update env threshold
        printf("//------- update env threshold -------//\n");

        for (int x = 0; x < ref_x + 1; x++) {
            ISP.af_ctrl0.af_manual_update = 1;

            while (!ISP.int_raw.af_fdone);

            ISP.int_clr.af_fdone = 0x1;
            sum[0] = ISP.af_sum_a.af_suma;
            sum[1] = ISP.af_sum_b.af_sumb;
            sum[2] = ISP.af_sum_c.af_sumc;
            lum[0] = ISP.af_lum_a.af_luma;
            lum[1] = ISP.af_lum_b.af_lumb;
            lum[2] = ISP.af_lum_c.af_lumc;
            af_sum_tmp[x] = sum[0] + sum[1] + sum[2];
            af_lum_tmp[x] = lum[0] + lum[1] + lum[2];

            if ((x >= 1) && (abs(af_sum_tmp[x] - af_sum_max) < af_sum_max * 0.3) && (abs(af_sum_tmp[x - 1] - af_sum_max) < af_sum_max * 0.3)) {
                ref_x = x;
                break;
            }
        }

        for (int x = 0; x < ref_x + 1; x++) {
            printf("af_sum[%d]: %d, af_lum[%d]: %d\n", x, af_sum_tmp[x], x, af_lum_tmp[x]);
        }

        af_sum = af_sum_tmp[ref_x];
        af_lum = af_lum_tmp[ref_x];

        af_sum_env_th = af_sum * 0.5;
        af_lum_env_th = af_lum * 0.05;
        printf("//------- update af env threshold done -------//\n");
        printf("af_sum: %d, af_sum_env_th: %d\n", af_sum, af_sum_env_th);
        printf("af_lum: %d, af_lum_env_th: %d\n", af_lum, af_lum_env_th);
        printf("//----------- af update done ----------//");
        ISP.af_env_user_th_sum = af_sum_env_th;
        ISP.af_env_user_th_lum.af_env_user_threshold_lum = af_lum_env_th;

        ISP.int_clr.af_env = 0x1;
        ISP.cntl.af_en = 1;
    }

    printf("\n");
    printf("\n");
    printf("\n");
    return 0;
}
#endif

#if TEST_CSI_HIST
static int isp_hist_config()
{
    //HIST
    ISP.hist_mode.hist_mode = 4;  /*statistic mode. 0: RAW_B, 1: RAW_GB, 2: RAW_GR 3: RAW_R, 4: RGB, 5:YUV_Y, 6:YUV_U, 7:YUV_V*/
    ISP.hist_coeff.hist_coeff_r = 85;  /*coefficient of R when set hist_mode to RGB, sum of coeff_r and coeff_g and coeff_b should be 256*/
    ISP.hist_coeff.hist_coeff_g = 85;  /*coefficient of G when set hist_mode to RGB, sum of coeff_r and coeff_g and coeff_b should be 256*/
    ISP.hist_coeff.hist_coeff_b = 85;  /*coefficient of B when set hist_mode to RGB, sum of coeff_r and coeff_g and coeff_b should be 256*/
    ISP.hist_offs.hist_x_offs = (MIPI_CSI_DISP_HSIZE / 2) - 50;  /*x coordinate of first window*/
    ISP.hist_size.hist_x_size = 100; /*x direction size of subwindow, should >= 18*/
    ISP.hist_offs.hist_y_offs = (MIPI_CSI_DISP_VSIZE / 2) - 50;  /*y coordinate of first window*/
    ISP.hist_size.hist_y_size = 100; /*y direction size of subwindow*/

    ISP.hist_seg0.hist_seg_0_1   = 16;  /*threshold of histogram bin 0 and bin 1*/
    ISP.hist_seg0.hist_seg_1_2   = 32;  /*threshold of histogram bin 1 and bin 2*/
    ISP.hist_seg0.hist_seg_2_3   = 48;  /*threshold of histogram bin 2 and bin 3*/
    ISP.hist_seg0.hist_seg_3_4   = 64;  /*threshold of histogram bin 3 and bin 4*/
    ISP.hist_seg1.hist_seg_4_5   = 80;  /*threshold of histogram bin 4 and bin 5*/
    ISP.hist_seg1.hist_seg_5_6   = 96;  /*threshold of histogram bin 5 and bin 6*/
    ISP.hist_seg1.hist_seg_6_7   = 112;  /*threshold of histogram bin 6 and bin 7*/
    ISP.hist_seg1.hist_seg_7_8   = 128;  /*threshold of histogram bin 7 and bin 8*/
    ISP.hist_seg2.hist_seg_8_9   = 144;  /*threshold of histogram bin 8 and bin 9*/
    ISP.hist_seg2.hist_seg_9_10  = 160;  /*threshold of histogram bin 9 and bin 10*/
    ISP.hist_seg2.hist_seg_10_11 = 176;  /*threshold of histogram bin 10 and bin 11*/
    ISP.hist_seg2.hist_seg_11_12 = 192;  /*threshold of histogram bin 11 and bin 12*/
    ISP.hist_seg3.hist_seg_12_13 = 208;  /*threshold of histogram bin 12 and bin 13*/
    ISP.hist_seg3.hist_seg_13_14 = 224;  /*threshold of histogram bin 13 and bin 14*/
    ISP.hist_seg3.hist_seg_14_15 = 240;  /*threshold of histogram bin 14 and bin 15*/

    ISP.hist_weight0.hist_weight_00 = 10;  /*weight of subwindow 00 and sum of all weight should be 256*/
    ISP.hist_weight0.hist_weight_01 = 10;
    ISP.hist_weight0.hist_weight_02 = 10;
    ISP.hist_weight0.hist_weight_03 = 10;
    ISP.hist_weight1.hist_weight_04 = 10;
    ISP.hist_weight1.hist_weight_10 = 10;
    ISP.hist_weight1.hist_weight_11 = 10;
    ISP.hist_weight1.hist_weight_12 = 10;
    ISP.hist_weight2.hist_weight_13 = 10;
    ISP.hist_weight2.hist_weight_14 = 10;
    ISP.hist_weight2.hist_weight_20 = 10;
    ISP.hist_weight2.hist_weight_21 = 10;
    ISP.hist_weight3.hist_weight_22 = 10;
    ISP.hist_weight3.hist_weight_23 = 10;
    ISP.hist_weight3.hist_weight_24 = 10;
    ISP.hist_weight3.hist_weight_30 = 10;
    ISP.hist_weight4.hist_weight_31 = 10;
    ISP.hist_weight4.hist_weight_32 = 10;
    ISP.hist_weight4.hist_weight_33 = 10;
    ISP.hist_weight4.hist_weight_34 = 10;
    ISP.hist_weight5.hist_weight_40 = 10;
    ISP.hist_weight5.hist_weight_41 = 10;
    ISP.hist_weight5.hist_weight_42 = 10;
    ISP.hist_weight5.hist_weight_43 = 10;
    ISP.hist_weight6.hist_weight_44 = 10;

    ISP.cntl.hist_en = 0x1;

    return 0;
}

static int isp_hist_update()
{
    uint32_t hist_bin[16] = {0};

    while (!ISP.int_raw.hist_fdone);

    ISP.int_clr.hist_fdone = 0x1;
    ISP.cntl.hist_en = 0x0;

    hist_bin[0] = ISP.hist_bin0.hist_bin_0;
    hist_bin[1] = ISP.hist_bin1.hist_bin_1;
    hist_bin[2] = ISP.hist_bin2.hist_bin_2;
    hist_bin[3] = ISP.hist_bin3.hist_bin_3;
    hist_bin[4] = ISP.hist_bin4.hist_bin_4;
    hist_bin[5] = ISP.hist_bin5.hist_bin_5;
    hist_bin[6] = ISP.hist_bin6.hist_bin_6;
    hist_bin[7] = ISP.hist_bin7.hist_bin_7;
    hist_bin[8] = ISP.hist_bin8.hist_bin_8;
    hist_bin[9] = ISP.hist_bin9.hist_bin_9;
    hist_bin[10] = ISP.hist_bin10.hist_bin_10;
    hist_bin[11] = ISP.hist_bin11.hist_bin_11;
    hist_bin[12] = ISP.hist_bin12.hist_bin_12;
    hist_bin[13] = ISP.hist_bin13.hist_bin_13;
    hist_bin[14] = ISP.hist_bin14.hist_bin_14;
    hist_bin[15] = ISP.hist_bin15.hist_bin_15;

    ISP.cntl.hist_en = 0x1;

    for (int x = 0; x < 16; x++) {
        printf("hist_bin[%d]: 0x%x\n", x, hist_bin[x]);
    }
}
#endif

static void isp_task(void *arg)
{
    while (1) {
#if TEST_CSI_AWB
        isp_awb_update();
#endif
#if TEST_CSI_AE
        isp_ae_update();
#endif
#if TEST_CSI_AF
        isp_af_update();
#endif
#if TEST_CSI_HIST
        isp_hist_update();
#endif
         vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

int isp_init()
{
    printf("ISP_EN: %d\n", ISP.cntl.en);

    // ISP.clk_en.val = ~0;
    ISP.cntl.val = 0;
    ISP.cntl.ccm_en = 1;
    ISP.cntl.demosaic_en = 1;
    ISP.cntl.dpc_en = 1;
    // ISP.cntl.dpc_check_en = 0;
    // ISP.cntl.dpc_sta_en = 0;
    // ISP.cntl.dpc_dyn_en = 1;
    // ISP.cntl.dpc_black_en = 0;
    ISP.cntl.median_en = 1;
    ISP.cntl.gamma_en = 1;
    ISP.cntl.sharp_en = 1;
    ISP.cntl.out_type = (TEST_DSI_COLOR_MODE == MIPI_DSI_RGB565_MODE) ? 4 : 2;
    ISP.yuv_format.yuv_mode = 1;
    ISP.yuv_format.yuv_range = 0;
    ISP.cntl.rgb2yuv_en = 1;
    ISP.cntl.yuv2rgb_en = 1;
    ISP.cntl.color_en = 1;
    ISP.cntl.blc_en = 1;
    ISP.cntl.bf_en = 1;
    ISP.cntl.lsc_en = 0;
    ISP.cntl.data_type = (TEST_CSI_COLOR_MODE == MIPI_CSI_RAW8_MODE) ? 0x0 : ((TEST_CSI_COLOR_MODE == MIPI_CSI_RAW10_MODE) ? 0x1 : 0x2);
    ISP.cntl.in_src = 0;
    ISP.cntl.mipi_data_en = 1;
    ISP.frame_cfg.hsync_start_exist = TEST_CSI_LINESYNC;
    ISP.frame_cfg.hsync_end_exist = TEST_CSI_LINESYNC;
    ISP.frame_cfg.bayer_mode = 0;
    ISP.int_ena.val = 0;
    ISP.int_clr.val = ~0;

#if TEST_CSI_AWB
    isp_awb_config();
#endif
#if TEST_CSI_AE
    isp_ae_config();
#endif
#if TEST_CSI_AF
    isp_af_config();
#endif
#if TEST_CSI_HIST
    isp_hist_config();
#endif

    // chip912 gamma 的参数更新有变化，在 isp_en 之前，需要先向 reg_gamma_update 写 1，再等该寄存器硬件置 0 更新完成
    ISP.gamma_ctrl.gamma_update = 1;
    while (ISP.gamma_ctrl.gamma_update);

    if (TEST_CSI_COLOR_MODE >= MIPI_CSI_RAW8_MODE) {
        ISP.frame_cfg.hadr_num = MIPI_CSI_DISP_HSIZE - 1;
        ISP.frame_cfg.vadr_num = MIPI_CSI_DISP_VSIZE - 1;
        ISP.cntl.en = 1;
    } else {
        ISP.frame_cfg.hadr_num = ceil((float)(MIPI_CSI_DISP_HSIZE * TEST_CSI_COLOR_WIDTH) / 32.0) - 1;
        ISP.frame_cfg.vadr_num = MIPI_CSI_DISP_VSIZE - 1;
        ISP.cntl.en = 0;
    }

    xTaskCreatePinnedToCore(isp_task, "isp_task", 8192, NULL,
                                5, NULL, 0);
    return 0;
}