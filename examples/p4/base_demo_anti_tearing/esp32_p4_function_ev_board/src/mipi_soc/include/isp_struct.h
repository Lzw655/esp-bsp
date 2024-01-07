/*
 * SPDX-FileCopyrightText: 2017-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _SOC_STRUCT_H_
#define _SOC_STRUCT_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
    uint32_t ver_date;
    union {
        struct {
            uint32_t clk_en                    :    1;  /*this bit configures the clk force on of isp reg. 0: disable, 1: enable*/
            uint32_t clk_blc_force_on          :    1;  /*this bit configures the clk force on of blc. 0: disable, 1: enable*/
            uint32_t clk_dpc_force_on          :    1;  /*this bit configures the clk force on of dpc. 0: disable, 1: enable*/
            uint32_t clk_bf_force_on           :    1;  /*this bit configures the clk force on of bf. 0: disable, 1: enable*/
            uint32_t clk_lsc_force_on          :    1;  /*this bit configures the clk force on of lsc. 0: disable, 1: enable*/
            uint32_t clk_demosaic_force_on     :    1;  /*this bit configures the clk force on of demosaic. 0: disable, 1: enable*/
            uint32_t clk_median_force_on       :    1;  /*this bit configures the clk force on of median. 0: disable, 1: enable*/
            uint32_t clk_ccm_force_on          :    1;  /*this bit configures the clk force on of ccm. 0: disable, 1: enable*/
            uint32_t clk_gamma_force_on        :    1;  /*this bit configures the clk force on of gamma. 0: disable, 1: enable*/
            uint32_t clk_rgb2yuv_force_on      :    1;  /*this bit configures the clk force on of rgb2yuv. 0: disable, 1: enable*/
            uint32_t clk_sharp_force_on        :    1;  /*this bit configures the clk force on of sharp. 0: disable, 1: enable*/
            uint32_t clk_color_force_on        :    1;  /*this bit configures the clk force on of color. 0: disable, 1: enable*/
            uint32_t clk_yuv2rgb_force_on      :    1;  /*this bit configures the clk force on of yuv2rgb. 0: disable, 1: enable*/
            uint32_t clk_ae_force_on           :    1;  /*this bit configures the clk force on of ae. 0: disable, 1: enable*/
            uint32_t clk_af_force_on           :    1;  /*this bit configures the clk force on of af. 0: disable, 1: enable*/
            uint32_t clk_awb_force_on          :    1;  /*this bit configures the clk force on of awb. 0: disable, 1: enable*/
            uint32_t clk_hist_force_on         :    1;  /*this bit configures the clk force on of hist. 0: disable, 1: enable*/
            uint32_t clk_mipi_idi_force_on     :    1;  /*this bit configures the clk force on of mipi idi input. 0: disable, 1: enable*/
            uint32_t mem_clk_force_on      :    1;  /*this bit configures the clk force on of all isp memory. 0: disable, 1: enable*/
            uint32_t reserved19                    :    13;
        };
        uint32_t val;
    } clk_en;
    union {
        struct {
            uint32_t mipi_data_en              :    1;  /*this bit configures mipi input data enable. 0: disable, 1: enable*/
            uint32_t en                    :    1;  /*this bit configures isp global enable. 0: disable, 1: enable*/
            uint32_t blc_en                    :    1;  /*this bit configures blc enable. 0: disable, 1: enable*/
            uint32_t dpc_en                    :    1;  /*this bit configures dpc enable. 0: disable, 1: enable*/
            uint32_t bf_en                     :    1;  /*this bit configures bf enable. 0: disable, 1: enable*/
            uint32_t lsc_en                    :    1;  /*this bit configures lsc enable. 0: disable, 1: enable*/
            uint32_t demosaic_en               :    1;  /*this bit configures demosaic enable. 0: disable, 1: enable*/
            uint32_t median_en                 :    1;  /*this bit configures median enable. 0: disable, 1: enable*/
            uint32_t ccm_en                    :    1;  /*this bit configures ccm enable. 0: disable, 1: enable*/
            uint32_t gamma_en                  :    1;  /*this bit configures gamma enable. 0: disable, 1: enable*/
            uint32_t rgb2yuv_en                :    1;  /*this bit configures rgb2yuv enable. 0: disable, 1: enable*/
            uint32_t sharp_en                  :    1;  /*this bit configures sharp enable. 0: disable, 1: enable*/
            uint32_t color_en                  :    1;  /*this bit configures color enable. 0: disable, 1: enable*/
            uint32_t yuv2rgb_en                :    1;  /*this bit configures yuv2rgb enable. 0: disable, 1: enable*/
            uint32_t ae_en                     :    1;  /*this bit configures ae enable. 0: disable, 1: enable*/
            uint32_t af_en                     :    1;  /*this bit configures af enable. 0: disable, 1: enable*/
            uint32_t awb_en                    :    1;  /*this bit configures awb enable. 0: disable, 1: enable*/
            uint32_t hist_en                   :    1;  /*this bit configures hist enable. 0: disable, 1: enable*/
            uint32_t reserved18                    :    6;
            uint32_t byte_endian_order         :    1;  /*select input idi data byte_endian_order when isp is bypass, 0: csi_data[31:0], 1: {[7:0], [15:8], [23:16], [31:24]}*/
            uint32_t data_type             :    2;  /*this field configures input data type, 0:RAW8 1:RAW10 2:RAW12*/
            uint32_t in_src                :    2;  /*this field configures input data source, 0:CSI HOST 1:CAM 2:DMA*/
            uint32_t out_type              :    3;  /*this field configures pixel output type, 0: RAW8 1: YUV422 2: RGB888 3: YUV420 4: RGB565*/
        };
        uint32_t val;
    } cntl;
    union {
        struct {
            uint32_t hsync_cnt                 :    8;  /*this field configures the number of clock before hsync and after vsync and line_end when decodes pix data from idi to isp*/
            uint32_t reserved8                     :    24;
        };
        uint32_t val;
    } hsync_cnt;
    union {
        struct {
            uint32_t vadr_num                  :    12;  /*this field configures input image size in y-direction, image row number - 1*/
            uint32_t hadr_num                  :    12;  /*this field configures input image size in x-direction, image line number - 1*/
            uint32_t reserved24                    :    3;
            uint32_t bayer_mode                :    2;  /*this field configures the bayer mode of input pixel. 00 : BG/GR    01 : GB/RG   10 : GR/BG  11 : RG/GB*/
            uint32_t hsync_start_exist         :    1;  /*this bit configures the line end packet exist or not. 0: not exist, 1: exist*/
            uint32_t hsync_end_exist           :    1;  /*this bit configures the line start packet exist or not. 0: not exist, 1: exist*/
            uint32_t reserved31                    :    1;
        };
        uint32_t val;
    } frame_cfg;
    union {
        struct {
            uint32_t ccm_coef_rr               :    13;  /*this field configures the color correction matrix coefficient*/
            uint32_t ccm_coef_rg               :    13;  /*this field configures the color correction matrix coefficient*/
            uint32_t reserved26                    :    6;
        };
        uint32_t val;
    } ccm_coef0;
    union {
        struct {
            uint32_t ccm_coef_rb               :    13;  /*this field configures the color correction matrix coefficient*/
            uint32_t ccm_coef_gr               :    13;  /*this field configures the color correction matrix coefficient*/
            uint32_t reserved26                    :    6;
        };
        uint32_t val;
    } ccm_coef1;
    union {
        struct {
            uint32_t ccm_coef_gg               :    13;  /*this field configures the color correction matrix coefficient*/
            uint32_t ccm_coef_gb               :    13;  /*this field configures the color correction matrix coefficient*/
            uint32_t reserved26                    :    6;
        };
        uint32_t val;
    } ccm_coef3;
    union {
        struct {
            uint32_t ccm_coef_br               :    13;  /*this field configures the color correction matrix coefficient*/
            uint32_t ccm_coef_bg               :    13;  /*this field configures the color correction matrix coefficient*/
            uint32_t reserved26                    :    6;
        };
        uint32_t val;
    } ccm_coef4;
    union {
        struct {
            uint32_t ccm_coef_bb               :    13;  /*this field configures the color correction matrix coefficient*/
            uint32_t reserved13                    :    19;
        };
        uint32_t val;
    } ccm_coef5;
    union {
        struct {
            uint32_t bf_tail_pixen_pulse_tl    :    8;  /*matrix tail pixen low level threshold, should not to large to prevent expanding to next frame, only bf_tail_pixen_pulse_th!=0 and bf_tail_pixen_pulse_tl!=0 and bf_tail_pixen_pulse_th < bf_tail_pixen_pulse_tl will enable tail pulse function*/
            uint32_t bf_tail_pixen_pulse_th    :    8;  /*matrix tail pixen high level threshold, must < hnum-1, only bf_tail_pixen_pulse_th!=0 and bf_tail_pixen_pulse_tl!=0 and bf_tail_pixen_pulse_th < bf_tail_pixen_pulse_tl will enable tail pulse function*/
            uint32_t bf_padding_data           :    8;  /*this field configures bf matrix padding data*/
            uint32_t bf_padding_mode           :    1;  /*this bit configures the padding mode of bf matrix.  0: use pixel in image to do padding   1: use padding_data to do padding*/
            uint32_t reserved25                    :    7;
        };
        uint32_t val;
    } bf_matrix_ctrl;
    union {
        struct {
            uint32_t sigma                     :    6;  /*this field configures the bayer denoising level, valid data from 2 to 20*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } bf_sigma;
    union {
        struct {
            uint32_t gau_template21            :    4;  /*this field configures index 21 of gausian template*/
            uint32_t gau_template20            :    4;  /*this field configures index 20 of gausian template*/
            uint32_t gau_template12            :    4;  /*this field configures index 12 of gausian template*/
            uint32_t gau_template11            :    4;  /*this field configures index 11 of gausian template*/
            uint32_t gau_template10            :    4;  /*this field configures index 10 of gausian template*/
            uint32_t gau_template02            :    4;  /*this field configures index 02 of gausian template*/
            uint32_t gau_template01            :    4;  /*this field configures index 01 of gausian template*/
            uint32_t gau_template00            :    4;  /*this field configures index 00 of gausian template*/
        };
        uint32_t val;
    } bf_gau0;
    union {
        struct {
            uint32_t gau_template22            :    4;  /*this field configures index 22 of gausian template*/
            uint32_t reserved4                     :    28;
        };
        uint32_t val;
    } bf_gau1;
    union {
        struct {
            uint32_t dpc_check_en              :    1;  /*this bit configures the check mode enable. 0: disable, 1: enable*/
            uint32_t dpc_sta_en                :    1;  /*this bit configures the sta dpc enable. 0: disable, 1: enable*/
            uint32_t dpc_dyn_en                :    1;  /*this bit configures the dyn dpc enable. 0: disable, 1: enable*/
            uint32_t dpc_black_en              :    1;  /*this bit configures input image type select when in check mode, 0: white img, 1: black img*/
            uint32_t dpc_method_sel            :    1;  /*this bit configures dyn dpc method select. 0: simple method, 1: hard method*/
            uint32_t dpc_check_od_en           :    1;  /*this bit configures output pixel data when in check mode or not. 0: no data output, 1: data output*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } dpc_ctrl;
    union {
        struct {
            uint32_t dpc_threshold_l           :    8;  /*this bit configures the threshold to detect black img in check mode, or the low threshold(use 8 bit 0~255) in dyn method 0, or the low threshold factor (use 5 bit 10000-> 16/16, 00001->1/16, 0/16~16/16) in dyn method 1*/
            uint32_t dpc_threshold_h           :    8;  /*this bit configures the threshold to detect white img in check mode, or the high threshold(use 8 bit 0~255) in dyn method 0, or the high threshold factor (use 5 bit 10000-> 16/16, 00001->1/16, 0/16~16/16) in dyn method 1*/
            uint32_t dpc_factor_dark           :    6;  /*this field configures the dynamic correction method 1 dark   factor*/
            uint32_t dpc_factor_brig           :    6;  /*this field configures the dynamic correction method 1 bright factor */
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } dpc_conf;
    union {
        struct {
            uint32_t dpc_tail_pixen_pulse_tl   :    8;  /*matrix tail pixen low level threshold,  should not to large to prevent expanding to next frame, only dpc_tail_pixen_pulse_th!=0 and dpc_tail_pixen_pulse_tl!=0 and dpc_tail_pixen_pulse_th < dpc_tail_pixen_pulse_tl will enable tail pulse function*/
            uint32_t dpc_tail_pixen_pulse_th   :    8;  /*matrix tail pixen high level threshold, must < hnum-1, only dpc_tail_pixen_pulse_th!=0 and dpc_tail_pixen_pulse_tl!=0 and dpc_tail_pixen_pulse_th < dpc_tail_pixen_pulse_tl will enable tail pulse function*/
            uint32_t dpc_padding_data          :    8;  /*this field configures dpc matrix padding data*/
            uint32_t dpc_padding_mode          :    1;  /*this bit configures the padding mode of dpc matrix.  0: use pixel in image to do padding   1: use padding_data to do padding*/
            uint32_t reserved25                    :    7;
        };
        uint32_t val;
    } dpc_matrix_ctrl;
    union {
        struct {
            uint32_t dpc_deadpix_cnt           :    10;  /*this field represents the dead pixel count */
            uint32_t reserved10                    :    22;
        };
        uint32_t val;
    } dpc_deadpix_cnt;
    union {
        struct {
            uint32_t lut_addr                  :    12;  /*this field configures the lut access addr, when select lsc lut, [11:10]:00 sel gb_b lut, 01 sel r_gr lut*/
            uint32_t lut_num                   :    4;  /*this field configures the lut selection. 0000:LSC LUT 0001:DPC LUT*/
            uint32_t lut_cmd                   :    1;  /*this bit configures the access event of lut. 0:rd 1: wr */
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } lut_cmd;
    uint32_t lut_wdata;
    uint32_t lut_rdata;
    union {
        struct {
            uint32_t lsc_xtablesize            :    5;  /*this field configures lsc table size in x-direction*/
            uint32_t reserved5                     :    27;
        };
        uint32_t val;
    } lsc_tablesize;
    union {
        struct {
            uint32_t demosaic_tail_pixen_pulse_tl:    8;  /*matrix tail pixen low level threshold,  should not to large to prevent expanding to next frame, only demosaic_tail_pixen_pulse_th!=0 and demosaic_tail_pixen_pulse_tl!=0 and demosaic_tail_pixen_pulse_th < demosaic_tail_pixen_pulse_tl will enable tail pulse function*/
            uint32_t demosaic_tail_pixen_pulse_th:    8;  /*matrix tail pixen high level threshold, must < hnum-1, only demosaic_tail_pixen_pulse_th!=0 and demosaic_tail_pixen_pulse_tl!=0 and demosaic_tail_pixen_pulse_th < demosaic_tail_pixen_pulse_tl will enable tail pulse function*/
            uint32_t demosaic_padding_data     :    8;  /*this field configures demosaic matrix padding data*/
            uint32_t demosaic_padding_mode     :    1;  /*this bit configures the padding mode of demosaic matrix.  0: use pixel in image to do padding   1: use padding_data to do padding*/
            uint32_t reserved25                    :    7;
        };
        uint32_t val;
    } demosaic_matrix_ctrl;
    union {
        struct {
            uint32_t demosaic_grad_ratio       :    6;  /*this field configures demosaic gradient select ratio*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } demosaic_grad_ratio;
    union {
        struct {
            uint32_t median_padding_data       :    8;  /*this field configures median matrix padding data*/
            uint32_t median_padding_mode       :    1;  /*this bit configures the padding mode of median matrix.  0: use pixel in image to do padding   1: use padding_data to do padding*/
            uint32_t reserved9                     :    23;
        };
        uint32_t val;
    } median_matrix_ctrl;
    union {
        struct {
            uint32_t data_type_err :    1;  /*the raw interrupt status of input data type error. isp only support RGB bayer data type, other type will report type_err_int*/
            uint32_t async_fifo_ovf:    1;  /*the raw interrupt status of isp input fifo overflow*/
            uint32_t buf_full      :    1;  /*the raw interrupt status of isp input buffer full*/
            uint32_t hvnum_setting_err:    1;  /*the raw interrupt status of hnum and vnum setting format error*/
            uint32_t data_type_setting_err:    1;  /*the raw interrupt status of setting invalid data_type*/
            uint32_t mipi_hnum_unmatch:    1;  /*the raw interrupt status of hnum setting unmatch with mipi input*/
            uint32_t dpc_check_done    :    1;  /*the raw interrupt status of dpc check done*/
            uint32_t gamma_xcoord_err  :    1;  /*the raw interrupt status of gamma setting error. it report the sum of the lengths represented by gamma_x00~x0F isn't equal to 256*/
            uint32_t ae_monitor        :    1;  /*the raw interrupt status of ae monitor*/
            uint32_t ae_frame_done     :    1;  /*the raw interrupt status of ae.*/
            uint32_t af_fdone          :    1;  /*the raw interrupt status of af statistic. when auto_update enable, each frame done will send one int pulse when manual_update, each time when write 1 to manual_update will send a int pulse when next frame done*/
            uint32_t af_env            :    1;  /*the raw interrupt status of af monitor. send a int pulse when env_det function enabled and environment changes detected*/
            uint32_t awb_fdone         :    1;  /*the raw interrupt status of awb. send a int pulse when statistic of one awb frame done*/
            uint32_t hist_fdone        :    1;  /*the raw interrupt status of histogram. send a int pulse when statistic of one frame histogram done*/
            uint32_t frame             :    1;  /*the raw interrupt status of isp frame end*/
            uint32_t blc_frame         :    1;  /*the raw interrupt status of blc frame done*/
            uint32_t lsc_frame         :    1;  /*the raw interrupt status of lsc frame done*/
            uint32_t dpc_frame         :    1;  /*the raw interrupt status of dpc frame done*/
            uint32_t bf_frame          :    1;  /*the raw interrupt status of bf frame done*/
            uint32_t demosaic_frame    :    1;  /*the raw interrupt status of demosaic frame done*/
            uint32_t median_frame      :    1;  /*the raw interrupt status of median frame done*/
            uint32_t ccm_frame         :    1;  /*the raw interrupt status of ccm frame done*/
            uint32_t gamma_frame       :    1;  /*the raw interrupt status of gamma frame done*/
            uint32_t rgb2yuv_frame     :    1;  /*the raw interrupt status of rgb2yuv frame done*/
            uint32_t sharp_frame       :    1;  /*the raw interrupt status of sharp frame done*/
            uint32_t color_frame       :    1;  /*the raw interrupt status of color frame done*/
            uint32_t yuv2rgb_frame     :    1;  /*the raw interrupt status of yuv2rgb frame done*/
            uint32_t tail_idi_frame    :    1;  /*the raw interrupt status of tail idi frame_end*/
            uint32_t header_idi_frame  :    1;  /*the raw interrupt status of real input frame end of input*/
            uint32_t reserved29                    :    3;
        };
        uint32_t val;
    } int_raw;
    union {
        struct {
            uint32_t data_type_err  :    1;  /*the masked interrupt status of input data type error*/
            uint32_t async_fifo_ovf :    1;  /*the masked interrupt status of isp input fifo overflow*/
            uint32_t buf_full       :    1;  /*the masked interrupt status of isp input buffer full*/
            uint32_t hvnum_setting_err:    1;  /*the masked interrupt status of hnum and vnum setting format error*/
            uint32_t data_type_setting_err:    1;  /*the masked interrupt status of setting invalid data_type*/
            uint32_t mipi_hnum_unmatch:    1;  /*the masked interrupt status of hnum setting unmatch with mipi input*/
            uint32_t dpc_check_done     :    1;  /*the masked interrupt status of dpc check done*/
            uint32_t gamma_xcoord_err   :    1;  /*the masked interrupt status of gamma setting error*/
            uint32_t ae_monitor         :    1;  /*the masked interrupt status of ae monitor*/
            uint32_t ae_frame_done      :    1;  /*the masked interrupt status of ae*/
            uint32_t af_fdone           :    1;  /*the masked interrupt status of af statistic*/
            uint32_t af_env             :    1;  /*the masked interrupt status of af monitor*/
            uint32_t awb_fdone          :    1;  /*the masked interrupt status of awb*/
            uint32_t hist_fdone         :    1;  /*the masked interrupt status of histogram*/
            uint32_t frame              :    1;  /*the masked interrupt status of isp frame end*/
            uint32_t blc_frame          :    1;  /*the masked interrupt status of blc frame done*/
            uint32_t lsc_frame          :    1;  /*the masked interrupt status of lsc frame done*/
            uint32_t dpc_frame          :    1;  /*the masked interrupt status of dpc frame done*/
            uint32_t bf_frame           :    1;  /*the masked interrupt status of bf frame done*/
            uint32_t demosaic_frame     :    1;  /*the masked interrupt status of demosaic frame done*/
            uint32_t median_frame       :    1;  /*the masked interrupt status of median frame done*/
            uint32_t ccm_frame          :    1;  /*the masked interrupt status of ccm frame done*/
            uint32_t gamma_frame        :    1;  /*the masked interrupt status of gamma frame done*/
            uint32_t rgb2yuv_frame      :    1;  /*the masked interrupt status of rgb2yuv frame done*/
            uint32_t sharp_frame        :    1;  /*the masked interrupt status of sharp frame done*/
            uint32_t color_frame        :    1;  /*the masked interrupt status of color frame done*/
            uint32_t yuv2rgb_frame      :    1;  /*the masked interrupt status of yuv2rgb frame done*/
            uint32_t tail_idi_frame     :    1;  /*the masked interrupt status of tail idi frame_end*/
            uint32_t header_idi_frame   :    1;  /*the masked interrupt status of real input frame end of input*/
            uint32_t reserved29                    :    3;
        };
        uint32_t val;
    } int_st;
    union {
        struct {
            uint32_t data_type_err :    1;  /*write 1 to enable input data type error*/
            uint32_t async_fifo_ovf:    1;  /*write 1 to enable isp input fifo overflow*/
            uint32_t buf_full      :    1;  /*write 1 to enable isp input buffer full*/
            uint32_t hvnum_setting_err:    1;  /*write 1 to enable hnum and vnum setting format error*/
            uint32_t data_type_setting_err:    1;  /*write 1 to enable setting invalid data_type*/
            uint32_t mipi_hnum_unmatch:    1;  /*write 1 to enable hnum setting unmatch with mipi input*/
            uint32_t dpc_check_done    :    1;  /*write 1 to enable dpc check done*/
            uint32_t gamma_xcoord_err  :    1;  /*write 1 to enable gamma setting error*/
            uint32_t ae_monitor        :    1;  /*write 1 to enable ae monitor*/
            uint32_t ae_frame_done     :    1;  /*write 1 to enable ae*/
            uint32_t af_fdone          :    1;  /*write 1 to enable af statistic*/
            uint32_t af_env            :    1;  /*write 1 to enable af monitor*/
            uint32_t awb_fdone         :    1;  /*write 1 to enable awb*/
            uint32_t hist_fdone        :    1;  /*write 1 to enable histogram*/
            uint32_t frame             :    1;  /*write 1 to enable isp frame end*/
            uint32_t blc_frame         :    1;  /*write 1 to enable blc frame done*/
            uint32_t lsc_frame         :    1;  /*write 1 to enable lsc frame done*/
            uint32_t dpc_frame         :    1;  /*write 1 to enable dpc frame done*/
            uint32_t bf_frame          :    1;  /*write 1 to enable bf frame done*/
            uint32_t demosaic_frame    :    1;  /*write 1 to enable demosaic frame done*/
            uint32_t median_frame      :    1;  /*write 1 to enable median frame done*/
            uint32_t ccm_frame         :    1;  /*write 1 to enable ccm frame done*/
            uint32_t gamma_frame       :    1;  /*write 1 to enable gamma frame done*/
            uint32_t rgb2yuv_frame     :    1;  /*write 1 to enable rgb2yuv frame done*/
            uint32_t sharp_frame       :    1;  /*write 1 to enable sharp frame done*/
            uint32_t color_frame       :    1;  /*write 1 to enable color frame done*/
            uint32_t yuv2rgb_frame     :    1;  /*write 1 to enable yuv2rgb frame done*/
            uint32_t tail_idi_frame    :    1;  /*write 1 to enable tail idi frame_end*/
            uint32_t header_idi_frame  :    1;  /*write 1 to enable real input frame end of input*/
            uint32_t reserved29                    :    3;
        };
        uint32_t val;
    } int_ena;
    union {
        struct {
            uint32_t data_type_err :    1;  /*write 1 to clear input data type error*/
            uint32_t async_fifo_ovf:    1;  /*write 1 to clear isp input fifo overflow*/
            uint32_t buf_full      :    1;  /*write 1 to clear isp input buffer full*/
            uint32_t hvnum_setting_err:    1;  /*write 1 to clear hnum and vnum setting format error*/
            uint32_t data_type_setting_err:    1;  /*write 1 to clear setting invalid data_type*/
            uint32_t mipi_hnum_unmatch:    1;  /*write 1 to clear hnum setting unmatch with mipi input*/
            uint32_t dpc_check_done    :    1;  /*write 1 to clear dpc check done*/
            uint32_t gamma_xcoord_err  :    1;  /*write 1 to clear gamma setting error*/
            uint32_t ae_monitor        :    1;  /*write 1 to clear ae monitor*/
            uint32_t ae_frame_done     :    1;  /*write 1 to clear ae*/
            uint32_t af_fdone          :    1;  /*write 1 to clear af statistic*/
            uint32_t af_env            :    1;  /*write 1 to clear af monitor*/
            uint32_t awb_fdone         :    1;  /*write 1 to clear awb*/
            uint32_t hist_fdone        :    1;  /*write 1 to clear histogram*/
            uint32_t frame             :    1;  /*write 1 to clear isp frame end*/
            uint32_t blc_frame         :    1;  /*write 1 to clear blc frame done*/
            uint32_t lsc_frame         :    1;  /*write 1 to clear lsc frame done*/
            uint32_t dpc_frame         :    1;  /*write 1 to clear dpc frame done*/
            uint32_t bf_frame          :    1;  /*write 1 to clear bf frame done*/
            uint32_t demosaic_frame    :    1;  /*write 1 to clear demosaic frame done*/
            uint32_t median_frame      :    1;  /*write 1 to clear median frame done*/
            uint32_t ccm_frame         :    1;  /*write 1 to clear ccm frame done*/
            uint32_t gamma_frame       :    1;  /*write 1 to clear gamma frame done*/
            uint32_t rgb2yuv_frame     :    1;  /*write 1 to clear rgb2yuv frame done*/
            uint32_t sharp_frame       :    1;  /*write 1 to clear sharp frame done*/
            uint32_t color_frame       :    1;  /*write 1 to clear color frame done*/
            uint32_t yuv2rgb_frame     :    1;  /*write 1 to clear yuv2rgb frame done*/
            uint32_t tail_idi_frame    :    1;  /*write 1 to clear tail idi frame_end*/
            uint32_t header_idi_frame  :    1;  /*write 1 to clear real input frame end of input*/
            uint32_t reserved29                    :    3;
        };
        uint32_t val;
    } int_clr;
    union {
        struct {
            uint32_t gamma_update              :    1;  /*Indicates that gamma register configuration is complete*/
            uint32_t gamma_b_last_correct      :    1;  /*this bit configures enable of last b segment correcction. 0: disable, 1: enable*/
            uint32_t gamma_g_last_correct      :    1;  /*this bit configures enable of last g segment correcction. 0: disable, 1: enable*/
            uint32_t gamma_r_last_correct      :    1;  /*this bit configures enable of last r segment correcction. 0: disable, 1: enable*/
            uint32_t reserved4                     :    28;
        };
        uint32_t val;
    } gamma_ctrl;
    union {
        struct {
            uint32_t gamma_r_y03               :    8;  /*this field configures the point 3 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y02               :    8;  /*this field configures the point 2 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y01               :    8;  /*this field configures the point 1 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y00               :    8;  /*this field configures the point 0 of Y-axis of r channel gamma curve*/
        };
        uint32_t val;
    } gamma_ry1;
    union {
        struct {
            uint32_t gamma_r_y07               :    8;  /*this field configures the point 7 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y06               :    8;  /*this field configures the point 6 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y05               :    8;  /*this field configures the point 5 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y04               :    8;  /*this field configures the point 4 of Y-axis of r channel gamma curve*/
        };
        uint32_t val;
    } gamma_ry2;
    union {
        struct {
            uint32_t gamma_r_y0b               :    8;  /*this field configures the point 11 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y0a               :    8;  /*this field configures the point 10 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y09               :    8;  /*this field configures the point 9 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y08               :    8;  /*this field configures the point 8 of Y-axis of r channel gamma curve*/
        };
        uint32_t val;
    } gamma_ry3;
    union {
        struct {
            uint32_t gamma_r_y0f               :    8;  /*this field configures the point 15 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y0e               :    8;  /*this field configures the point 14 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y0d               :    8;  /*this field configures the point 13 of Y-axis of r channel gamma curve*/
            uint32_t gamma_r_y0c               :    8;  /*this field configures the point 12 of Y-axis of r channel gamma curve*/
        };
        uint32_t val;
    } gamma_ry4;
    union {
        struct {
            uint32_t gamma_g_y03               :    8;  /*this field configures the point 3 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y02               :    8;  /*this field configures the point 2 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y01               :    8;  /*this field configures the point 1 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y00               :    8;  /*this field configures the point 0 of Y-axis of g channel gamma curve*/
        };
        uint32_t val;
    } gamma_gy1;
    union {
        struct {
            uint32_t gamma_g_y07               :    8;  /*this field configures the point 7 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y06               :    8;  /*this field configures the point 6 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y05               :    8;  /*this field configures the point 5 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y04               :    8;  /*this field configures the point 4 of Y-axis of g channel gamma curve*/
        };
        uint32_t val;
    } gamma_gy2;
    union {
        struct {
            uint32_t gamma_g_y0b               :    8;  /*this field configures the point 11 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y0a               :    8;  /*this field configures the point 10 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y09               :    8;  /*this field configures the point 9 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y08               :    8;  /*this field configures the point 8 of Y-axis of g channel gamma curve*/
        };
        uint32_t val;
    } gamma_gy3;
    union {
        struct {
            uint32_t gamma_g_y0f               :    8;  /*this field configures the point 15 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y0e               :    8;  /*this field configures the point 14 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y0d               :    8;  /*this field configures the point 13 of Y-axis of g channel gamma curve*/
            uint32_t gamma_g_y0c               :    8;  /*this field configures the point 12 of Y-axis of g channel gamma curve*/
        };
        uint32_t val;
    } gamma_gy4;
    union {
        struct {
            uint32_t gamma_b_y03               :    8;  /*this field configures the point 3 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y02               :    8;  /*this field configures the point 2 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y01               :    8;  /*this field configures the point 1 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y00               :    8;  /*this field configures the point 0 of Y-axis of b channel gamma curve*/
        };
        uint32_t val;
    } gamma_by1;
    union {
        struct {
            uint32_t gamma_b_y07               :    8;  /*this field configures the point 7 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y06               :    8;  /*this field configures the point 6 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y05               :    8;  /*this field configures the point 5 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y04               :    8;  /*this field configures the point 4 of Y-axis of b channel gamma curve*/
        };
        uint32_t val;
    } gamma_by2;
    union {
        struct {
            uint32_t gamma_b_y0b               :    8;  /*this field configures the point 11 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y0a               :    8;  /*this field configures the point 10 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y09               :    8;  /*this field configures the point 9 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y08               :    8;  /*this field configures the point 8 of Y-axis of b channel gamma curve*/
        };
        uint32_t val;
    } gamma_by3;
    union {
        struct {
            uint32_t gamma_b_y0f               :    8;  /*this field configures the point 15 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y0e               :    8;  /*this field configures the point 14 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y0d               :    8;  /*this field configures the point 13 of Y-axis of b channel gamma curve*/
            uint32_t gamma_b_y0c               :    8;  /*this field configures the point 12 of Y-axis of b channel gamma curve*/
        };
        uint32_t val;
    } gamma_by4;
    union {
        struct {
            uint32_t gamma_r_x07               :    3;  /*this field configures the point 7 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x06               :    3;  /*this field configures the point 6 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x05               :    3;  /*this field configures the point 5 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x04               :    3;  /*this field configures the point 4 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x03               :    3;  /*this field configures the point 3 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x02               :    3;  /*this field configures the point 2 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x01               :    3;  /*this field configures the point 1 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x00               :    3;  /*this field configures the point 0 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t reserved24                    :    8;
        };
        uint32_t val;
    } gamma_rx1;
    union {
        struct {
            uint32_t gamma_r_x0f               :    3;  /*this field configures the point 15 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x0e               :    3;  /*this field configures the point 14 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x0d               :    3;  /*this field configures the point 13 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x0c               :    3;  /*this field configures the point 12 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x0b               :    3;  /*this field configures the point 11 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x0a               :    3;  /*this field configures the point 10 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x09               :    3;  /*this field configures the point 9 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_r_x08               :    3;  /*this field configures the point 8 of X-axis of r channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t reserved24                    :    8;
        };
        uint32_t val;
    } gamma_rx2;
    union {
        struct {
            uint32_t gamma_g_x07               :    3;  /*this field configures the point 7 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x06               :    3;  /*this field configures the point 6 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x05               :    3;  /*this field configures the point 5 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x04               :    3;  /*this field configures the point 4 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x03               :    3;  /*this field configures the point 3 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x02               :    3;  /*this field configures the point 2 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x01               :    3;  /*this field configures the point 1 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x00               :    3;  /*this field configures the point 0 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t reserved24                    :    8;
        };
        uint32_t val;
    } gamma_gx1;
    union {
        struct {
            uint32_t gamma_g_x0f               :    3;  /*this field configures the point 15 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x0e               :    3;  /*this field configures the point 14 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x0d               :    3;  /*this field configures the point 13 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x0c               :    3;  /*this field configures the point 12 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x0b               :    3;  /*this field configures the point 11 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x0a               :    3;  /*this field configures the point 10 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x09               :    3;  /*this field configures the point 9 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_g_x08               :    3;  /*this field configures the point 8 of X-axis of g channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t reserved24                    :    8;
        };
        uint32_t val;
    } gamma_gx2;
    union {
        struct {
            uint32_t gamma_b_x07               :    3;  /*this field configures the point 7 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x06               :    3;  /*this field configures the point 6 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x05               :    3;  /*this field configures the point 5 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x04               :    3;  /*this field configures the point 4 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x03               :    3;  /*this field configures the point 3 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x02               :    3;  /*this field configures the point 2 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x01               :    3;  /*this field configures the point 1 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x00               :    3;  /*this field configures the point 0 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t reserved24                    :    8;
        };
        uint32_t val;
    } gamma_bx1;
    union {
        struct {
            uint32_t gamma_b_x0f               :    3;  /*this field configures the point 15 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x0e               :    3;  /*this field configures the point 14 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x0d               :    3;  /*this field configures the point 13 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x0c               :    3;  /*this field configures the point 12 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x0b               :    3;  /*this field configures the point 11 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x0a               :    3;  /*this field configures the point 10 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x09               :    3;  /*this field configures the point 9 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t gamma_b_x08               :    3;  /*this field configures the point 8 of X-axis of b channel gamma curve, it represents the power of the distance from the previous point*/
            uint32_t reserved24                    :    8;
        };
        uint32_t val;
    } gamma_bx2;
    union {
        struct {
            uint32_t ae_update                 :    1;  /*write 1 to this bit triggers one statistic event*/
            uint32_t ae_select                 :    1;  /*this field configures ae input data source, 0: data from median, 1: data from gama*/
            uint32_t reserved2                     :    30;
        };
        uint32_t val;
    } ae_ctrl;
    union {
        struct {
            uint32_t ae_monitor_tl             :    8;  /*this field configures the lower lum threshold of ae monitor*/
            uint32_t ae_monitor_th             :    8;  /*this field configures the higher lum threshold of ae monitor*/
            uint32_t ae_monitor_period         :    6;  /*this field cnfigures ae monitor frame period*/
            uint32_t reserved22                    :    10;
        };
        uint32_t val;
    } ae_monitor;
    union {
        struct {
            uint32_t ae_x_bsize                :    11;  /*this field configures every block x size*/
            uint32_t ae_x_start                :    11;  /*this field configures first block start x address*/
            uint32_t reserved22                    :    10;
        };
        uint32_t val;
    } ae_bx;
    union {
        struct {
            uint32_t ae_y_bsize                :    11;  /*this field configures every block y size*/
            uint32_t ae_y_start                :    11;  /*this field configures first block start y address*/
            uint32_t reserved22                    :    10;
        };
        uint32_t val;
    } ae_by;
    union {
        struct {
            uint32_t ae_subwin_pixnum          :    17;  /*this field configures the pixel number of each sub win*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } ae_winpixnum;
    union {
        struct {
            uint32_t ae_subwin_recip           :    20;  /*this field configures the reciprocal of each subwin_pixnum, 20bit fraction*/
            uint32_t reserved20                    :    12;
        };
        uint32_t val;
    } ae_win_reciprocal;
    union {
        struct {
            uint32_t ae_b03_mean               :    8;  /*this field configures block03 Y mean data*/
            uint32_t ae_b02_mean               :    8;  /*this field configures block02 Y mean data*/
            uint32_t ae_b01_mean               :    8;  /*this field configures block01 Y mean data  */
            uint32_t ae_b00_mean               :    8;  /*this field configures block00 Y mean data*/
        };
        uint32_t val;
    } ae_block_mean_0;
    union {
        struct {
            uint32_t ae_b12_mean               :    8;  /*this field configures block12 Y mean data*/
            uint32_t ae_b11_mean               :    8;  /*this field configures block11 Y mean data*/
            uint32_t ae_b10_mean               :    8;  /*this field configures block10 Y mean data*/
            uint32_t ae_b04_mean               :    8;  /*this field configures block04 Y mean data*/
        };
        uint32_t val;
    } ae_block_mean_1;
    union {
        struct {
            uint32_t ae_b21_mean               :    8;  /*this field configures block21 Y mean data*/
            uint32_t ae_b20_mean               :    8;  /*this field configures block20 Y mean data*/
            uint32_t ae_b14_mean               :    8;  /*this field configures block14 Y mean data*/
            uint32_t ae_b13_mean               :    8;  /*this field configures block13 Y mean data*/
        };
        uint32_t val;
    } ae_block_mean_2;
    union {
        struct {
            uint32_t ae_b30_mean               :    8;  /*this field configures block30 Y mean data*/
            uint32_t ae_b24_mean               :    8;  /*this field configures block24 Y mean data*/
            uint32_t ae_b23_mean               :    8;  /*this field configures block23 Y mean data*/
            uint32_t ae_b22_mean               :    8;  /*this field configures block22 Y mean data*/
        };
        uint32_t val;
    } ae_block_mean_3;
    union {
        struct {
            uint32_t ae_b34_mean               :    8;  /*this field configures block34 Y mean data*/
            uint32_t ae_b33_mean               :    8;  /*this field configures block33 Y mean data*/
            uint32_t ae_b32_mean               :    8;  /*this field configures block32 Y mean data*/
            uint32_t ae_b31_mean               :    8;  /*this field configures block31 Y mean data*/
        };
        uint32_t val;
    } ae_block_mean_4;
    union {
        struct {
            uint32_t ae_b43_mean               :    8;  /*this field configures block43 Y mean data*/
            uint32_t ae_b42_mean               :    8;  /*this field configures block42 Y mean data*/
            uint32_t ae_b41_mean               :    8;  /*this field configures block41 Y mean data*/
            uint32_t ae_b40_mean               :    8;  /*this field configures block40 Y mean data*/
        };
        uint32_t val;
    } ae_block_mean_5;
    union {
        struct {
            uint32_t reserved0                     :    24;
            uint32_t ae_b44_mean               :    8;  /*this field configures block44 Y mean data*/
        };
        uint32_t val;
    } ae_block_mean_6;
    union {
        struct {
            uint32_t sharp_threshold_low       :    8;  /*this field configures sharpen threshold for detail*/
            uint32_t sharp_threshold_high      :    8;  /*this field configures sharpen threshold for edge*/
            uint32_t sharp_amount_low          :    8;  /*this field configures sharpen amount for detail*/
            uint32_t sharp_amount_high         :    8;  /*this field configures sharpen amount for edge*/
        };
        uint32_t val;
    } sharp_ctrl0;
    union {
        struct {
            uint32_t sharp_filter_coe00        :    5;  /*this field configures unsharp masking(usm) filter coefficient*/
            uint32_t sharp_filter_coe01        :    5;  /*this field configures usm filter coefficient*/
            uint32_t sharp_filter_coe02        :    5;  /*this field configures usm filter coefficient*/
            uint32_t reserved15                    :    17;
        };
        uint32_t val;
    } sharp_filter0;
    union {
        struct {
            uint32_t sharp_filter_coe10        :    5;  /*this field configures usm filter coefficient*/
            uint32_t sharp_filter_coe11        :    5;  /*this field configures usm filter coefficient*/
            uint32_t sharp_filter_coe12        :    5;  /*this field configures usm filter coefficient*/
            uint32_t reserved15                    :    17;
        };
        uint32_t val;
    } sharp_filter1;
    union {
        struct {
            uint32_t sharp_filter_coe20        :    5;  /*this field configures usm filter coefficient*/
            uint32_t sharp_filter_coe21        :    5;  /*this field configures usm filter coefficient*/
            uint32_t sharp_filter_coe22        :    5;  /*this field configures usm filter coefficient*/
            uint32_t reserved15                    :    17;
        };
        uint32_t val;
    } sharp_filter2;
    union {
        struct {
            uint32_t sharp_tail_pixen_pulse_tl :    8;  /*matrix tail pixen low level threshold,  should not to large to prevent expanding to next frame, only demosaic_tail_pixen_pulse_th!=0 and demosaic_tail_pixen_pulse_tl!=0 and demosaic_tail_pixen_pulse_th < demosaic_tail_pixen_pulse_tl will enable tail pulse function*/
            uint32_t sharp_tail_pixen_pulse_th :    8;  /*matrix tail pixen high level threshold, must < hnum-1, only sharp_tail_pixen_pulse_th!=0 and sharp_tail_pixen_pulse_tl!=0 and sharp_tail_pixen_pulse_th < sharp_tail_pixen_pulse_tl will enable tail pulse function*/
            uint32_t sharp_padding_data        :    8;  /*this field configures sharp padding data*/
            uint32_t sharp_padding_mode        :    1;  /*this field configures sharp padding mode*/
            uint32_t reserved25                    :    7;
        };
        uint32_t val;
    } sharp_matrix_ctrl;
    union {
        struct {
            uint32_t sharp_gradient_max        :    8;  /*this field configures sharp max gradient, refresh at the end of each frame end*/
            uint32_t reserved8                     :    24;
        };
        uint32_t val;
    } sharp_ctrl1;
    union {
        struct {
            uint32_t dma_en                    :    1;  /*write 1 to triger dma to get 1 frame*/
            uint32_t dma_update_reg            :    1;  /*write 1 to update dma_burst_len & dma_data_type*/
            uint32_t dma_data_type             :    6;  /*this field configures the idi data type for image data*/
            uint32_t dma_burst_len             :    12;  /*this field configures dma burst len when data source is dma. set according to dma_msize, it is the number of 64bits in a dma transfer*/
            uint32_t dma_req_interval          :    12;  /*this field configures dma req interval, 12'b1: 1 cycle, 12'b11 2 cycle ...*/
        };
        uint32_t val;
    } dma_cntl;
    union {
        struct {
            uint32_t dma_raw_num_total         :    22;  /*this field configures the the number of 64bits in a frame*/
            uint32_t reserved22                    :    9;
            uint32_t dma_raw_num_total_set     :    1;  /*write 1 to update dma_raw_num_total*/
        };
        uint32_t val;
    } dma_raw_data;
    union {
        struct {
            uint32_t cam_en                    :    1;  /*write 1 to start recive camera data, write 0 to disable*/
            uint32_t cam_update_reg            :    1;  /*write 1 to update CAM_CONF*/
            uint32_t cam_reset                 :    1;  /*this bit configures cam clk domain reset, 1: reset cam input logic, 0: release reset*/
            uint32_t cam_clk_inv               :    1;  /*this bit configures the invertion of cam clk from pad. 0: not invert cam clk, 1: invert cam clk*/
            uint32_t reserved4                     :    28;
        };
        uint32_t val;
    } cam_cntl;
    union {
        struct {
            uint32_t cam_data_order            :    1;  /*this field configures data order of cam port, 0: cam_data_in, 1:{cam_data_in[7:0], cam_data_in[15:8]}*/
            uint32_t cam_2byte_mode            :    1;  /*this field configures enable of cam 2 byte mode(input 2 bytes each clock). 0: disable, 1: enable*/
            uint32_t cam_data_type             :    6;  /*this field configures idi data type for image data, 0x2a: RAW8, 0x2b: RAW10, 0x2c: RAW12*/
            uint32_t cam_de_inv                :    1;  /*this bit configures cam data enable invert. 0: not invert, 1: invert   */
            uint32_t cam_hsync_inv             :    1;  /*this bit configures cam hsync invert. 0: not invert, 1: invert*/
            uint32_t cam_vsync_inv             :    1;  /*this bit configures cam vsync invert. 0: not invert, 1: invert*/
            uint32_t cam_vsync_filter_thres    :    3;  /*this bit configures the number of clock of vsync filter length*/
            uint32_t cam_vsync_filter_en       :    1;  /*this bit configures vsync filter en*/
            uint32_t reserved15                    :    17;
        };
        uint32_t val;
    } cam_conf;
    union {
        struct {
            uint32_t af_auto_update            :    1;  /*this bit configures auto_update enable. when set to 1, will update sum and lum each frame*/
            uint32_t reserved1                     :    3;
            uint32_t af_manual_update          :    1;  /*write 1 to this bit will update the sum and lum once*/
            uint32_t reserved5                     :    3;
            uint32_t af_env_threshold          :    4;  /*this field configures env threshold. when both sum and lum changes larger than this value, consider environment changes and need to trigger a new autofocus. 4Bit fractional*/
            uint32_t reserved12                    :    4;
            uint32_t af_env_period             :    8;  /*this field configures environment changes detection period (frame). When set to 0, disable this function*/
            uint32_t reserved24                    :    8;
        };
        uint32_t val;
    } af_ctrl0;
    union {
        struct {
            uint32_t af_thpixnum               :    22;  /*this field configures pixnum used when calculating the autofocus threshold. Set to 0 to disable threshold calculation*/
            uint32_t reserved22                    :    10;
        };
        uint32_t val;
    } af_ctrl1;
    union {
        struct {
            uint32_t af_gen_threshold_min      :    16;  /*this field configures min threshold when use auto_threshold*/
            uint32_t af_gen_threshold_max      :    16;  /*this field configures max threshold when use auto_threshold*/
        };
        uint32_t val;
    } af_gen_th_ctrl;
    uint32_t af_env_user_th_sum;
    union {
        struct {
            uint32_t af_env_user_threshold_lum :    30;  /*this field configures user setup env detect lum threshold*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } af_env_user_th_lum;
    union {
        struct {
            uint32_t af_threshold              :    16;  /*this field configures user threshold. When set to non-zero, autofocus will use this threshold*/
            uint32_t af_gen_threshold          :    16;  /*this field represents the last calculated threshold*/
        };
        uint32_t val;
    } af_threshold;
    union {
        struct {
            uint32_t af_rpoint_a               :    12;  /*this field configures left coordinate of focus window a, must >= 2*/
            uint32_t reserved12                    :    4;
            uint32_t af_lpoint_a               :    12;  /*this field configures top coordinate of focus window a, must >= 2*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } af_hscale_a;
    union {
        struct {
            uint32_t af_bpoint_a               :    12;  /*this field configures right coordinate of focus window a, must <= hnum-2*/
            uint32_t reserved12                    :    4;
            uint32_t af_tpoint_a               :    12;  /*this field configures bottom coordinate of focus window a, must <= hnum-2*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } af_vscale_a;
    union {
        struct {
            uint32_t af_rpoint_b               :    12;  /*this field configures left coordinate of focus window b, must >= 2*/
            uint32_t reserved12                    :    4;
            uint32_t af_lpoint_b               :    12;  /*this field configures top coordinate of focus window b, must >= 2*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } af_hscale_b;
    union {
        struct {
            uint32_t af_bpoint_b               :    12;  /*this field configures right coordinate of focus window b, must <= hnum-2*/
            uint32_t reserved12                    :    4;
            uint32_t af_tpoint_b               :    12;  /*this field configures bottom coordinate of focus window b, must <= hnum-2*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } af_vscale_b;
    union {
        struct {
            uint32_t af_rpoint_c               :    12;  /*this field configures left coordinate of focus window c, must >= 2*/
            uint32_t reserved12                    :    4;
            uint32_t af_lpoint_c               :    12;  /*this field configures top coordinate of focus window c, must >= 2*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } af_hscale_c;
    union {
        struct {
            uint32_t af_bpoint_c               :    12;  /*this field configures right coordinate of focus window c, must <= hnum-2*/
            uint32_t reserved12                    :    4;
            uint32_t af_tpoint_c               :    12;  /*this field configures bottom coordinate of focus window c, must <= hnum-2*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } af_vscale_c;
    union {
        struct {
            uint32_t af_suma                   :    30;  /*this field represents the result of accumulation of pix grad of focus window a*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } af_sum_a;
    union {
        struct {
            uint32_t af_sumb                   :    30;  /*this field represents the result of accumulation of pix grad of focus window b*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } af_sum_b;
    union {
        struct {
            uint32_t af_sumc                   :    30;  /*this field represents the result of accumulation of pix grad of focus window c*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } af_sum_c;
    union {
        struct {
            uint32_t af_luma                   :    28;  /*this field represents the result of accumulation of pix light of focus window a*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } af_lum_a;
    union {
        struct {
            uint32_t af_lumb                   :    28;  /*this field represents the result of accumulation of pix light of focus window b*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } af_lum_b;
    union {
        struct {
            uint32_t af_lumc                   :    28;  /*this field represents the result of accumulation of pix light of focus window c*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } af_lum_c;
    union {
        struct {
            uint32_t awb_mode                  :    2;  /*this field configures awb algo sel. 00: none sellected. 01: sel algo0. 10: sel algo1. 11: sel both algo0 and algo1*/
            uint32_t reserved2                     :    2;
            uint32_t awb_sample                :    1;  /*this bit configures awb sample location, 0:before ccm, 1:after ccm*/
            uint32_t reserved5                     :    27;
        };
        uint32_t val;
    } awb_mode;
    union {
        struct {
            uint32_t awb_rpoint                :    12;  /*this field configures awb window right coordinate*/
            uint32_t reserved12                    :    4;
            uint32_t awb_lpoint                :    12;  /*this field configures awb window left coordinate*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } awb_hscale;
    union {
        struct {
            uint32_t awb_bpoint                :    12;  /*this field configures awb window bottom coordinate*/
            uint32_t reserved12                    :    4;
            uint32_t awb_tpoint                :    12;  /*this field configures awb window top coordinate*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } awb_vscale;
    union {
        struct {
            uint32_t awb_min_lum               :    10;  /*this field configures lower threshold of r+g+b*/
            uint32_t reserved10                    :    6;
            uint32_t awb_max_lum               :    10;  /*this field configures upper threshold of r+g+b*/
            uint32_t reserved26                    :    6;
        };
        uint32_t val;
    } awb_th_lum;
    union {
        struct {
            uint32_t awb_min_rg                :    10;  /*this field configures lower threshold of r/g, 2bit integer and 8bit fraction*/
            uint32_t reserved10                    :    6;
            uint32_t awb_max_rg                :    10;  /*this field configures upper threshold of r/g, 2bit integer and 8bit fraction*/
            uint32_t reserved26                    :    6;
        };
        uint32_t val;
    } awb_th_rg;
    union {
        struct {
            uint32_t awb_min_bg                :    10;  /*this field configures lower threshold of b/g, 2bit integer and 8bit fraction*/
            uint32_t reserved10                    :    6;
            uint32_t awb_max_bg                :    10;  /*this field configures upper threshold of b/g, 2bit integer and 8bit fraction*/
            uint32_t reserved26                    :    6;
        };
        uint32_t val;
    } awb_th_bg;
    union {
        struct {
            uint32_t awb0_white_cnt            :    24;  /*this field configures number of white point detected of algo0*/
            uint32_t reserved24                    :    8;
        };
        uint32_t val;
    } awb0_white_cnt;
    uint32_t awb0_acc_r;
    uint32_t awb0_acc_g;
    uint32_t awb0_acc_b;
    union {
        struct {
            uint32_t color_saturation          :    8;  /*this field configures the color saturation value*/
            uint32_t color_hue                 :    8;  /*this field configures the color hue angle*/
            uint32_t color_contrast            :    8;  /*this field configures the color contrast value*/
            uint32_t color_brightness          :    8;  /*this field configures the color brightness value, signed 2's complement*/
        };
        uint32_t val;
    } color_ctrl;
    union {
        struct {
            uint32_t blc_r3_value              :    8;  /*this field configures the black level of bottom right channel of bayer img*/
            uint32_t blc_r2_value              :    8;  /*this field configures the black level of bottom left channel of bayer img*/
            uint32_t blc_r1_value              :    8;  /*this field configures the black level of top right channel of bayer img*/
            uint32_t blc_r0_value              :    8;  /*this field configures the black level of top left channel of bayer img*/
        };
        uint32_t val;
    } blc_value;
    union {
        struct {
            uint32_t blc_r3_stretch            :    1;  /*this bit configures the stretch feature of bottom right channel. 0: stretch disable, 1: stretch enable*/
            uint32_t blc_r2_stretch            :    1;  /*this bit configures the stretch feature of bottom left channel. 0: stretch disable, 1: stretch enable*/
            uint32_t blc_r1_stretch            :    1;  /*this bit configures the stretch feature of top right channel. 0: stretch disable, 1: stretch enable*/
            uint32_t blc_r0_stretch            :    1;  /*this bit configures the stretch feature of top left channel. 0: stretch disable, 1: stretch enable*/
            uint32_t reserved4                     :    28;
        };
        uint32_t val;
    } blc_ctrl0;
    union {
        struct {
            uint32_t blc_window_top            :    11;  /*this field configures blc average calculation window top*/
            uint32_t blc_window_left           :    11;  /*this field configures blc average calculation window left*/
            uint32_t blc_window_vnum           :    4;  /*this field configures blc average calculation window vnum*/
            uint32_t blc_window_hnum           :    4;  /*this field configures blc average calculation window hnum*/
            uint32_t blc_filter_en             :    1;  /*this bit configures enable blc average input filter. 0: disable, 1: enable*/
            uint32_t reserved31                    :    1;
        };
        uint32_t val;
    } blc_ctrl1;
    union {
        struct {
            uint32_t blc_r3_th                 :    8;  /*this field configures black threshold when get blc average of bottom right channel*/
            uint32_t blc_r2_th                 :    8;  /*this field configures black threshold when get blc average of bottom left channel*/
            uint32_t blc_r1_th                 :    8;  /*this field configures black threshold when get blc average of top right channel*/
            uint32_t blc_r0_th                 :    8;  /*this field configures black threshold when get blc average of top left channel*/
        };
        uint32_t val;
    } blc_ctrl2;
    union {
        struct {
            uint32_t blc_r3_mean               :    8;  /*this field represents the average black value of bottom right channel*/
            uint32_t blc_r2_mean               :    8;  /*this field represents the average black value of bottom left channel*/
            uint32_t blc_r1_mean               :    8;  /*this field represents the average black value of top right channel*/
            uint32_t blc_r0_mean               :    8;  /*this field represents the average black value of top left channel*/
        };
        uint32_t val;
    } blc_mean;
    union {
        struct {
            uint32_t hist_mode                 :    3;  /*this field configures statistic mode. 0: RAW_B, 1: RAW_GB, 2: RAW_GR 3: RAW_R, 4: RGB, 5:YUV_Y, 6:YUV_U, 7:YUV_V*/
            uint32_t reserved3                     :    29;
        };
        uint32_t val;
    } hist_mode;
    union {
        struct {
            uint32_t hist_coeff_b              :    8;  /*this field configures coefficient of B when set hist_mode to RGB, sum of coeff_r and coeff_g and coeff_b should be 256*/
            uint32_t hist_coeff_g              :    8;  /*this field configures coefficient of G when set hist_mode to RGB, sum of coeff_r and coeff_g and coeff_b should be 256*/
            uint32_t hist_coeff_r              :    8;  /*this field configures coefficient of R when set hist_mode to RGB, sum of coeff_r and coeff_g and coeff_b should be 256*/
            uint32_t reserved24                    :    8;
        };
        uint32_t val;
    } hist_coeff;
    union {
        struct {
            uint32_t hist_y_offs               :    12;  /*this field configures y coordinate of first window*/
            uint32_t reserved12                    :    4;
            uint32_t hist_x_offs               :    12;  /*this field configures x coordinate of first window*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } hist_offs;
    union {
        struct {
            uint32_t hist_y_size               :    9;  /*this field configures y direction size of subwindow*/
            uint32_t reserved9                     :    7;
            uint32_t hist_x_size               :    9;  /*this field configures x direction size of subwindow*/
            uint32_t reserved25                    :    7;
        };
        uint32_t val;
    } hist_size;
    union {
        struct {
            uint32_t hist_seg_3_4              :    8;  /*this field configures threshold of histogram bin 3 and bin 4*/
            uint32_t hist_seg_2_3              :    8;  /*this field configures threshold of histogram bin 2 and bin 3*/
            uint32_t hist_seg_1_2              :    8;  /*this field configures threshold of histogram bin 1 and bin 2*/
            uint32_t hist_seg_0_1              :    8;  /*this field configures threshold of histogram bin 0 and bin 1*/
        };
        uint32_t val;
    } hist_seg0;
    union {
        struct {
            uint32_t hist_seg_7_8              :    8;  /*this field configures threshold of histogram bin 7 and bin 8*/
            uint32_t hist_seg_6_7              :    8;  /*this field configures threshold of histogram bin 6 and bin 7*/
            uint32_t hist_seg_5_6              :    8;  /*this field configures threshold of histogram bin 5 and bin 6*/
            uint32_t hist_seg_4_5              :    8;  /*this field configures threshold of histogram bin 4 and bin 5*/
        };
        uint32_t val;
    } hist_seg1;
    union {
        struct {
            uint32_t hist_seg_11_12            :    8;  /*this field configures threshold of histogram bin 11 and bin 12*/
            uint32_t hist_seg_10_11            :    8;  /*this field configures threshold of histogram bin 10 and bin 11*/
            uint32_t hist_seg_9_10             :    8;  /*this field configures threshold of histogram bin 9 and bin 10*/
            uint32_t hist_seg_8_9              :    8;  /*this field configures threshold of histogram bin 8 and bin 9*/
        };
        uint32_t val;
    } hist_seg2;
    union {
        struct {
            uint32_t hist_seg_14_15            :    8;  /*this field configures threshold of histogram bin 14 and bin 15*/
            uint32_t hist_seg_13_14            :    8;  /*this field configures threshold of histogram bin 13 and bin 14*/
            uint32_t hist_seg_12_13            :    8;  /*this field configures threshold of histogram bin 12 and bin 13*/
            uint32_t reserved24                    :    8;
        };
        uint32_t val;
    } hist_seg3;
    union {
        struct {
            uint32_t hist_weight_03            :    8;  /*this field configures weight of subwindow 03*/
            uint32_t hist_weight_02            :    8;  /*this field configures weight of subwindow 02*/
            uint32_t hist_weight_01            :    8;  /*this field configures weight of subwindow 01*/
            uint32_t hist_weight_00            :    8;  /*this field configures weight of subwindow 00 and sum of all weight should be 256*/
        };
        uint32_t val;
    } hist_weight0;
    union {
        struct {
            uint32_t hist_weight_12            :    8;  /*this field configures weight of subwindow 12*/
            uint32_t hist_weight_11            :    8;  /*this field configures weight of subwindow 11*/
            uint32_t hist_weight_10            :    8;  /*this field configures weight of subwindow 10*/
            uint32_t hist_weight_04            :    8;  /*this field configures weight of subwindow 04*/
        };
        uint32_t val;
    } hist_weight1;
    union {
        struct {
            uint32_t hist_weight_21            :    8;  /*this field configures weight of subwindow 21*/
            uint32_t hist_weight_20            :    8;  /*this field configures weight of subwindow 20*/
            uint32_t hist_weight_14            :    8;  /*this field configures weight of subwindow 04*/
            uint32_t hist_weight_13            :    8;  /*this field configures weight of subwindow 13*/
        };
        uint32_t val;
    } hist_weight2;
    union {
        struct {
            uint32_t hist_weight_30            :    8;  /*this field configures weight of subwindow 30*/
            uint32_t hist_weight_24            :    8;  /*this field configures weight of subwindow 24*/
            uint32_t hist_weight_23            :    8;  /*this field configures weight of subwindow 23*/
            uint32_t hist_weight_22            :    8;  /*this field configures weight of subwindow 22*/
        };
        uint32_t val;
    } hist_weight3;
    union {
        struct {
            uint32_t hist_weight_34            :    8;  /*this field configures weight of subwindow 34*/
            uint32_t hist_weight_33            :    8;  /*this field configures weight of subwindow 33*/
            uint32_t hist_weight_32            :    8;  /*this field configures weight of subwindow 32*/
            uint32_t hist_weight_31            :    8;  /*this field configures weight of subwindow 31*/
        };
        uint32_t val;
    } hist_weight4;
    union {
        struct {
            uint32_t hist_weight_43            :    8;  /*this field configures weight of subwindow 43*/
            uint32_t hist_weight_42            :    8;  /*this field configures weight of subwindow 42*/
            uint32_t hist_weight_41            :    8;  /*this field configures weight of subwindow 41*/
            uint32_t hist_weight_40            :    8;  /*this field configures weight of subwindow 40*/
        };
        uint32_t val;
    } hist_weight5;
    union {
        struct {
            uint32_t hist_weight_44            :    8;  /*this field configures weight of subwindow 44*/
            uint32_t reserved8                     :    24;
        };
        uint32_t val;
    } hist_weight6;
    union {
        struct {
            uint32_t hist_bin_0                :    17;  /*this field represents result of histogram bin 0*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin0;
    union {
        struct {
            uint32_t hist_bin_1                :    17;  /*this field represents result of histogram bin 1*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin1;
    union {
        struct {
            uint32_t hist_bin_2                :    17;  /*this field represents result of histogram bin 2*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin2;
    union {
        struct {
            uint32_t hist_bin_3                :    17;  /*this field represents result of histogram bin 3*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin3;
    union {
        struct {
            uint32_t hist_bin_4                :    17;  /*this field represents result of histogram bin 4*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin4;
    union {
        struct {
            uint32_t hist_bin_5                :    17;  /*this field represents result of histogram bin 5*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin5;
    union {
        struct {
            uint32_t hist_bin_6                :    17;  /*this field represents result of histogram bin 6*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin6;
    union {
        struct {
            uint32_t hist_bin_7                :    17;  /*this field represents result of histogram bin 7*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin7;
    union {
        struct {
            uint32_t hist_bin_8                :    17;  /*this field represents result of histogram bin 8*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin8;
    union {
        struct {
            uint32_t hist_bin_9                :    17;  /*this field represents result of histogram bin 9*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin9;
    union {
        struct {
            uint32_t hist_bin_10               :    17;  /*this field represents result of histogram bin 10*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin10;
    union {
        struct {
            uint32_t hist_bin_11               :    17;  /*this field represents result of histogram bin 11*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin11;
    union {
        struct {
            uint32_t hist_bin_12               :    17;  /*this field represents result of histogram bin 12*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin12;
    union {
        struct {
            uint32_t hist_bin_13               :    17;  /*this field represents result of histogram bin 13*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin13;
    union {
        struct {
            uint32_t hist_bin_14               :    17;  /*this field represents result of histogram bin 14*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin14;
    union {
        struct {
            uint32_t hist_bin_15               :    17;  /*this field represents result of histogram bin 15*/
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } hist_bin15;
    union {
        struct {
            uint32_t header_mem_aux_ctrl       :    14;  /*this field configures the mem_aux of isp input buffer memory*/
            uint32_t reserved14                    :    2;
            uint32_t dpc_lut_mem_aux_ctrl      :    14;  /*this field represents this field configures the mem_aux of dpc lut memory*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } mem_aux_ctrl_0;
    union {
        struct {
            uint32_t lsc_lut_r_gr_mem_aux_ctrl :    14;  /*this field configures the mem_aux of lsc r gr lut memory*/
            uint32_t reserved14                    :    2;
            uint32_t lsc_lut_gb_b_mem_aux_ctrl :    14;  /*this field configures the mem_aux of lsc gb b lut memory*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } mem_aux_ctrl_1;
    union {
        struct {
            uint32_t bf_matrix_mem_aux_ctrl    :    14;  /*this field configures the mem_aux of bf line buffer memory*/
            uint32_t reserved14                    :    2;
            uint32_t dpc_matrix_mem_aux_ctrl   :    14;  /*this field configures the mem_aux of dpc line buffer memory*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } mem_aux_ctrl_2;
    union {
        struct {
            uint32_t sharp_matrix_y_mem_aux_ctrl:    14;  /*this field configures the mem_aux of sharp y line buffer memory*/
            uint32_t reserved14                    :    2;
            uint32_t demosaic_matrix_mem_aux_ctrl:    14;  /*this field configures the mem_aux of demosaic line buffer memory*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } mem_aux_ctrl_3;
    union {
        struct {
            uint32_t sharp_matrix_uv_mem_aux_ctrl:    14;  /*this field configures the mem_aux of sharp uv line buffer memory*/
            uint32_t reserved14                    :    18;
        };
        uint32_t val;
    } mem_aux_ctrl_4;
    union {
        struct {
            uint32_t yuv_mode                  :    1;  /*this bit configures the yuv mode. 0: ITU-R BT.601, 1: ITU-R BT.709*/
            uint32_t yuv_range                 :    1;  /*this bit configures the yuv range. 0: full range, 1: limit range*/
            uint32_t reserved2                     :    30;
        };
        uint32_t val;
    } yuv_format;
    union {
        struct {
            uint32_t rdn_eco_en                    :    1;  /*rdn_eco_en*/
            uint32_t rdn_eco_result                :    1;  /*rdn_eco_result*/
            uint32_t reserved2                     :    30;
        };
        uint32_t val;
    } rdn_eco_cs;
    uint32_t rdn_eco_low;
    uint32_t rdn_eco_high;
} isp_dev_t;
extern isp_dev_t ISP;
#ifdef __cplusplus
}
#endif



#endif /*_SOC_STRUCT_H_ */
