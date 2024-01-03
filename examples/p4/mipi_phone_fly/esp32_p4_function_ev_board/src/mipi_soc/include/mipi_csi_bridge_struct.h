/*
 * SPDX-FileCopyrightText: 2017-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _SOC_MIPI_CSI_BRIDGE_STRUCT_H_
#define _SOC_MIPI_CSI_BRIDGE_STRUCT_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
    union {
        struct {
            uint32_t clk_en                        :    1;  /*0: enable clock gating. 1: disable clock gating, clock always on.*/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } clk_en;
    union {
        struct {
            uint32_t csi_en                   :    1;  /*0: disable csi bridge. 1: enable csi bridge.*/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } en;
    union {
        struct {
            uint32_t dma_burst_len                 :    12;  /*DMA burst length.*/
            uint32_t dma_cfg_upd_by_blk            :    1;  /*1: reg_dma_burst_len & reg_dma_burst_len will be updated by dma block finish. 0: updated by frame.*/
            uint32_t reserved13                    :    3;
            uint32_t dma_force_rd_status           :    1;  /*1: mask dma request when reading frame info. 0: disable mask. */
            uint32_t reserved17                    :    15;
        };
        uint32_t val;
    } dma_req_cfg;
    union {
        struct {
            uint32_t csi_buf_afull_thrd            :    14;  /*buffer almost full threshold.*/
            uint32_t reserved14                    :    2;
            uint32_t csi_buf_depth                 :    14;  /*buffer data count.*/
            uint32_t reserved30                    :    2;
        };
        uint32_t val;
    } buf_flow_ctl;
    union {
        struct {
            uint32_t data_type_min                 :    6;  /*the min value of data type used for pixel filter.*/
            uint32_t reserved6                     :    2;
            uint32_t data_type_max                 :    6;  /*the max value of data type used for pixel filter.*/
            uint32_t reserved14                    :    18;
        };
        uint32_t val;
    } data_type_cfg;
    union {
        struct {
            uint32_t vadr_num                      :    12;  /*vadr of frame data.*/
            uint32_t hadr_num                      :    12;  /*hadr of frame data.*/
            uint32_t has_hsync_e                   :    1;  /*0: frame data doesn't contain hsync. 1: frame data contains hsync.*/
            uint32_t vadr_num_check                :    1;  /*0: disable vadr check. 1: enable vadr check.*/
            uint32_t reserved26                    :    6;
        };
        uint32_t val;
    } frame_cfg;
    union {
        struct {
            uint32_t byte_endian_order             :    1;  /*endianness order in bytes. 2'h0 is normal mode and 2'h3 is useful to YUV420(Legacy) when isp is bapassed.*/
            uint32_t bit_endian_order              :    1;
            uint32_t reserved2                     :    30;
        };
        uint32_t val;
    } endian_mode;
    union {
        struct {
            uint32_t vadr_num_gt                   :    1;  /*reg_vadr_num is greater than real interrupt raw.*/
            uint32_t vadr_num_lt                   :    1;  /*reg_vadr_num is less than real interrupt raw.*/
            uint32_t discard                       :    1;  /*an incomplete frame of data was sent interrupt raw.*/
            uint32_t csi_buf_overrun               :    1;  /*buffer overrun interrupt raw.*/
            uint32_t csi_async_fifo_ovf            :    1;  /*buffer overflow interrupt raw.*/
            uint32_t dma_cfg_has_updated           :    1;  /*dma configuration update complete interrupt raw.*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } int_raw;
    union {
        struct {
            uint32_t vadr_num_gt_real              :    1;  /*reg_vadr_num is greater than real interrupt clr.*/
            uint32_t vadr_num_lt_real              :    1;  /*reg_vadr_num is less than real interrupt clr.*/
            uint32_t discard                       :    1;  /*an incomplete frame of data was sent interrupt clr.*/
            uint32_t csi_buf_overrun               :    1;  /*buffer overrun interrupt clr.*/
            uint32_t csi_async_fifo_ovf            :    1;  /*buffer overflow interrupt clr.*/
            uint32_t dma_cfg_has_updated           :    1;  /*dma configuration update complete interrupt clr.*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } int_clr;
    union {
        struct {
            uint32_t vadr_num_gt                   :    1;  /*reg_vadr_num is greater than real interrupt st.*/
            uint32_t vadr_num_lt                   :    1;  /*reg_vadr_num is less than real interrupt st.*/
            uint32_t discard                       :    1;  /*an incomplete frame of data was sent interrupt st.*/
            uint32_t csi_buf_overrun               :    1;  /*buffer overrun interrupt st.*/
            uint32_t csi_async_fifo_ovf            :    1;  /*buffer overflow interrupt st.*/
            uint32_t dma_cfg_has_updated           :    1;  /*dma configuration update complete interrupt st.*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } int_st;
    union {
        struct {
            uint32_t vadr_num_gt                   :    1;  /*reg_vadr_num is greater than real interrupt enable.*/
            uint32_t vadr_num_lt                   :    1;  /*reg_vadr_num is less than real interrupt enable.*/
            uint32_t discard                       :    1;  /*an incomplete frame of data was sent interrupt enable.*/
            uint32_t csi_buf_overrun               :    1;  /*buffer overrun interrupt enable.*/
            uint32_t csi_async_fifo_ovf            :    1;  /*buffer overflow interrupt enable.*/
            uint32_t dma_cfg_has_updated           :    1;  /*dma configuration update complete interrupt enable.*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } int_ena;
    union {
        struct {
            uint32_t dma_req_interval              :    16;  /*16'b1: 1 cycle. 16'b11: 2 cycle. ... ... 16'hFFFF: 16 cycle.*/
            uint32_t reserved16                    :    16;
        };
        uint32_t val;
    } dma_req_interval;
    union {
        struct {
            uint32_t dmablk_size                   :    13;  /*the number of reg_dma_burst_len in a block*/
            uint32_t reserved13                    :    19;
        };
        uint32_t val;
    } dmablk_size;
    union {
        struct {
            uint32_t rdn_eco_en                    :    1;
            uint32_t rdn_eco_result                :    1;
            uint32_t reserved2                     :    30;
        };
        uint32_t val;
    } rdn_eco_cs;
    uint32_t rdn_eco_low;
    uint32_t rdn_eco_high;
    union {
        struct {
            uint32_t csi_enableclk                 :    1;  /*enable clock lane module of csi phy.*/
            uint32_t csi_cfg_clk_en                :    1;  /*enable cfg_clk of csi host module.*/
            uint32_t loopbk_test_en                :    1;  /*for phy test by loopback dsi phy to csi phy.*/
            uint32_t reserved3                     :    29;
        };
        uint32_t val;
    } host_ctrl;
    union {
        struct {
            uint32_t csi_bridge_mem_clk_force_on   :    1;  /*csi bridge memory clock gating force on.*/
            uint32_t csi_mem_aux_ctrl              :    14;
            uint32_t reserved15                    :    17;
        };
        uint32_t val;
    } mem_ctrl;
} mipi_csi_bridge_dev_t;
extern mipi_csi_bridge_dev_t MIPI_CSI_BRIDGE;
#ifdef __cplusplus
}
#endif



#endif /*_SOC_MIPI_CSI_BRIDGE_STRUCT_H_ */
