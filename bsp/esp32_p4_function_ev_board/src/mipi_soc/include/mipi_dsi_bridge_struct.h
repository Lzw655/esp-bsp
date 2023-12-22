/*
 * SPDX-FileCopyrightText: 2017-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _SOC_MIPI_DSI_BRIDGE_STRUCT_H_
#define _SOC_MIPI_DSI_BRIDGE_STRUCT_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
    union {
        struct {
            uint32_t clk_en                        :    1;  /*this bit configures force_on of dsi_bridge register clock gate*/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } clk_en;
    union {
        struct {
            uint32_t dsi_en                        :    1;  /*this bit configures module enable of dsi_bridge. 0: disable, 1: enable*/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } en;
    union {
        struct {
            uint32_t dma_burst_len                 :    12;  /*this field configures the num of 64-bit in one dma burst transfer, valid only when dsi_bridge as flow controller*/
            uint32_t reserved12                    :    20;
        };
        uint32_t val;
    } dma_req_cfg;
    union {
        struct {
            uint32_t raw_num_total                 :    22;  /*this field configures number of total pix bits/64*/
            uint32_t unalign_64bit_en              :    1;  /*this field configures whether the total pix bits is a multiple of 64bits. 0: align to 64-bit, 1: unalign to 64-bit*/
            uint32_t reserved23                    :    8;
            uint32_t raw_num_total_set             :    1;  /*this bit configures enable of reload reg_raw_num_total to internal cnt. 0: disable, 1: enable. valid only when dsi_bridge as flow controller*/
        };
        uint32_t val;
    } raw_num_cfg;
    union {
        struct {
            uint32_t credit_thrd                   :    15;  /*this field configures the threshold whether dsi_bridge fifo can receive one more 64-bit, valid only when dsi_bridge as flow controller*/
            uint32_t reserved15                    :    1;
            uint32_t credit_burst_thrd             :    15;  /*this field configures the threshold whether dsi_bridge fifo can receive one more dma burst, valid only when dsi_bridge as flow controller*/
            uint32_t credit_reset                  :    1;  /*this bit configures internal credit cnt clear, 0: non, 1: reset. valid only when dsi_bridge as flow controller*/
        };
        uint32_t val;
    } raw_buf_credit_ctl;
    union {
        struct {
            uint32_t raw_buf_depth                 :    14;  /*this field configures the depth of dsi_bridge fifo depth*/
            uint32_t reserved14                    :    18;
        };
        uint32_t val;
    } fifo_flow_status;
    union {
        struct {
            uint32_t raw_type                      :    4;  /*this field configures the pixel type. 0: rgb888, 1:rgb666, 2:rgb565*/
            uint32_t dpi_config                    :    2;  /*this field configures the pixel arrange type of dpi interface*/
            uint32_t data_in_type                  :    1;  /*input data type, 0: rgb, 1: yuv*/
            uint32_t reserved7                     :    25;
        };
        uint32_t val;
    } pixel_type;
    union {
        struct {
            uint32_t dma_block_slot                :    10;  /*this field configures the max block_slot_cnt*/
            uint32_t dma_block_interval            :    18;  /*this field configures the max block_interval_cnt, block_interval_cnt increased by 1 when block_slot_cnt if full*/
            uint32_t raw_num_total_auto_reload     :    1;  /*this bit configures enable of auto reload reg_raw_num_total, 0: disable, 1: enable*/
            uint32_t dma_block_interval_en         :    1;  /*this bit configures enable of interval between dma block transfer, 0: disable, 1: enable*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } dma_block_interval;
    union {
        struct {
            uint32_t dma_req_interval              :    16;  /*this field configures the interval between dma req events*/
            uint32_t reserved16                    :    16;
        };
        uint32_t val;
    } dma_req_interval;
    union {
        struct {
            uint32_t dpishutdn                     :    1;  /*this bit configures dpishutdn signal in dpi interface*/
            uint32_t dpicolorm                     :    1;  /*this bit configures dpicolorm signal in dpi interface*/
            uint32_t dpiupdatecfg                  :    1;  /*this bit configures dpiupdatecfg signal in dpi interface*/
            uint32_t reserved3                     :    29;
        };
        uint32_t val;
    } dpi_lcd_ctl;
    union {
        struct {
            uint32_t dpi_rsv_data                  :    30;  /*this field controls the pixel data sent to dsi_host when dsi_bridge fifo underflow*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } dpi_rsv_dpi_data;
    uint32_t reserved_2c;
    union {
        struct {
            uint32_t vtotal                        :    12;  /*this field configures the total length of one frame (by line) for dpi output, must meet: reg_vtotal > reg_vdisp+reg_vsync+reg_vbank*/
            uint32_t reserved12                    :    4;
            uint32_t vdisp                         :    12;  /*this field configures the length of valid line (by line) for dpi output*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } dpi_v_cfg0;
    union {
        struct {
            uint32_t vbank                         :    12;  /*this field configures the length between vsync and valid line (by line) for dpi output*/
            uint32_t reserved12                    :    4;
            uint32_t vsync                         :    12;  /*this field configures the length of vsync (by line) for dpi output*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } dpi_v_cfg1;
    union {
        struct {
            uint32_t htotal                        :    12;  /*this field configures the total length of one line (by pixel num) for dpi output, must meet: reg_htotal > reg_hdisp+reg_hsync+reg_hbank*/
            uint32_t reserved12                    :    4;
            uint32_t hdisp                         :    12;  /*this field configures the length of valid pixel data (by pixel num) for dpi output*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } dpi_h_cfg0;
    union {
        struct {
            uint32_t hbank                         :    12;  /*this field configures the length between hsync and pixel data valid (by pixel num) for dpi output*/
            uint32_t reserved12                    :    4;
            uint32_t hsync                         :    12;  /*this field configures the length of hsync (by pixel num) for dpi output*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } dpi_h_cfg1;
    union {
        struct {
            uint32_t dpi_en                        :    1;  /*this bit configures enable of dpi output, 0: disable, 1: enable*/
            uint32_t reserved1                     :    3;
            uint32_t fifo_underrun_discard_vcnt    :    12;  /*this field configures the underrun interrupt musk, when underrun occurs and line cnt is less then this field*/
            uint32_t reserved16                    :    16;
        };
        uint32_t val;
    } dpi_misc_config;
    union {
        struct {
            uint32_t dpi_config_update             :    1;  /*write 1 to this bit to update dpi config register MIPI_DSI_DPI_**/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } dpi_config_update;
    uint32_t reserved_48;
    uint32_t reserved_4c;
    union {
        struct {
            uint32_t dpi_underrun                  :    1;  /*write 1 to enables dpi_underrun_int_st field of MIPI_DSI_INT_ST_REG controlled by dpi_underrun  interrupt signal*/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } int_ena;
    union {
        struct {
            uint32_t dpi_underrun                  :    1;  /*write 1 to this bit to clear dpi_underrun_int_raw field of MIPI_DSI_INT_RAW_REG*/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } int_clr;
    union {
        struct {
            uint32_t dpi_underrun                  :    1;  /*the raw interrupt status of dpi_underrun*/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } int_raw;
    union {
        struct {
            uint32_t dpi_underrun                  :    1;  /*the masked interrupt status of dpi_underrun*/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } int_st;
    union {
        struct {
            uint32_t bistok                        :    1;  /*bistok*/
            uint32_t biston                        :    1;  /*biston*/
            uint32_t reserved2                     :    30;
        };
        uint32_t val;
    } host_bist_ctl;
    union {
        struct {
            uint32_t tx_trigger_rev_en             :    1;  /*tx_trigger reverse. 0: disable, 1: enable*/
            uint32_t rx_trigger_rev_en             :    1;  /*rx_trigger reverse. 0: disable, 1: enable*/
            uint32_t reserved2                     :    30;
        };
        uint32_t val;
    } host_trigger_rev;
    union {
        struct {
            uint32_t blk_raw_num_total             :    22;  /*this field configures number of total block pix bits/64*/
            uint32_t reserved22                    :    9;
            uint32_t blk_raw_num_total_set         :    1;  /*write 1 to reload reg_blk_raw_num_total to internal cnt*/
        };
        uint32_t val;
    } blk_raw_num_cfg;
    union {
        struct {
            uint32_t dma_frame_slot                :    10;  /*this field configures the max frame_slot_cnt*/
            uint32_t dma_frame_interval            :    18;  /*this field configures the max frame_interval_cnt, frame_interval_cnt increased by 1 when frame_slot_cnt if full*/
            uint32_t dma_multiblk_en               :    1;  /*this bit configures enable multi-blk transfer, 0: disable, 1: enable*/
            uint32_t dma_frame_interval_en         :    1;  /*this bit configures enable interval between frame transfer, 0: disable, 1: enable*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } dma_frame_interval;
    union {
        struct {
            uint32_t dsi_mem_aux_ctrl              :    14;  /*this field configures dsi_bridge fifo memory aux ctrl*/
            uint32_t reserved14                    :    18;
        };
        uint32_t val;
    } mem_aux_ctrl;
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
    union {
        struct {
            uint32_t dsi_cfg_ref_clk_en            :    1;  /*this bit configures the clk enable refclk and cfg_clk of dsi_host. 0: disable, 1: enable*/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } host_ctrl;
    union {
        struct {
            uint32_t dsi_bridge_mem_clk_force_on   :    1;  /*this bit configures the clock force on of dsi_bridge fifo memory. 0: disable, 1: force on*/
            uint32_t dsi_mem_clk_force_on          :    1;  /*this bit configures the clock force on of dpi fifo memory. 0: disable, 1: force on*/
            uint32_t reserved2                     :    30;
        };
        uint32_t val;
    } mem_clk_ctrl;
    union {
        struct {
            uint32_t dsi_dma_flow_controller       :    1;  /*this bit configures the flow controller, 0: dmac as flow controller, 1:dsi_bridge as flow controller*/
            uint32_t reserved1                     :    3;
            uint32_t dma_flow_multiblk_num         :    4;  /*this field configures the num of blocks when multi-blk is enable and dmac as flow controller*/
            uint32_t reserved8                     :    24;
        };
        uint32_t val;
    } dma_flow_ctrl;
    union {
        struct {
            uint32_t dsi_raw_buf_almost_empty_thrd :    11;  /*this field configures the fifo almost empty threshold, is valid only when dmac as flow controller*/
            uint32_t reserved11                    :    21;
        };
        uint32_t val;
    } raw_buf_almost_empty_thrd;
    union {
        struct {
            uint32_t yuv_protocal                  :    1;  /*this bit configures yuv protoocl, 0: bt.601, 1: bt.709*/
            uint32_t yuv_pix_endian                :    1;  /*this bit configures yuv pixel endian, 0: y0u0y1v1y2u2y3v3, 1: y3u3y2v2y1u1y0v0*/
            uint32_t yuv422_format                 :    2;  /*this field configures yuv422 store format, 0: yuyv, 1: yvyu, 2: uyvy, 3: vyuy*/
            uint32_t reserved4                     :    28;
        };
        uint32_t val;
    } yuv_cfg;
} mipi_dsi_bridge_dev_t;
extern mipi_dsi_bridge_dev_t MIPI_DSI_BRIDGE;
#ifdef __cplusplus
}
#endif



#endif /*_SOC_MIPI_DSI_BRIDGE_STRUCT_H_ */
