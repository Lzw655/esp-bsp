/**
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 *  SPDX-License-Identifier: Apache-2.0
 */
#pragma once


#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
    uint32_t bt_select;
    uint32_t out;
    uint32_t out_w1ts;
    uint32_t out_w1tc;
    union {
        struct {
            uint32_t data                          :    25;  /*GPIO output register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } out1;
    union {
        struct {
            uint32_t out1_w1ts                     :    25;  /*GPIO output set register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } out1_w1ts;
    union {
        struct {
            uint32_t out1_w1tc                     :    25;  /*GPIO output clear register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } out1_w1tc;
    union {
        struct {
            uint32_t ulpi_probe_mux_sel            :    1;  /*GPIO ulpi out and hp_top_probe[15:8] select register, see gpio_map_hp.csv*/
            uint32_t reserved1                     :    31;  /*reserved*/
        };
        uint32_t val;
    } ulpi_probe_mux_sel;
    uint32_t enable;
    uint32_t enable_w1ts;
    uint32_t enable_w1tc;
    union {
        struct {
            uint32_t data                          :    25;  /*GPIO output enable register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } enable1;
    union {
        struct {
            uint32_t enable1_w1ts                  :    25;  /*GPIO output enable set register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } enable1_w1ts;
    union {
        struct {
            uint32_t enable1_w1tc                  :    25;  /*GPIO output enable clear register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } enable1_w1tc;
    union {
        struct {
            uint32_t strapping                     :    16;  /*pad strapping register*/
            uint32_t reserved16                    :    16;  /*reserved*/
        };
        uint32_t val;
    } strap;
    uint32_t in;
    union {
        struct {
            uint32_t data                          :    25;  /*GPIO input register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } in1;
    uint32_t status;
    uint32_t status_w1ts;
    uint32_t status_w1tc;
    union {
        struct {
            uint32_t intr_st                       :    25;  /*GPIO interrupt status register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } status1;
    union {
        struct {
            uint32_t status1_w1ts                  :    25;  /*GPIO interrupt status set register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } status1_w1ts;
    union {
        struct {
            uint32_t status1_w1tc                  :    25;  /*GPIO interrupt status clear register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } status1_w1tc;
    uint32_t intr_0;
    union {
        struct {
            uint32_t int1_0                        :    25;  /*GPIO interrupt 0 status register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } intr1_0;
    uint32_t intr_1;
    union {
        struct {
            uint32_t int1_1                        :    25;  /*GPIO interrupt 1 status register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } intr1_1;
    uint32_t status_next;
    union {
        struct {
            uint32_t intr_st_next                  :    25;  /*GPIO interrupt source register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } status_next1;
    union {
        struct {
            uint32_t sync2_bypass                  :    2;  /*set GPIO input_sync2 signal mode. 0:disable. 1:trigger at negedge. 2or3:trigger at posedge.*/
            uint32_t pad_driver                    :    1;  /*set this bit to select pad driver. 1:open-drain. 0:normal.*/
            uint32_t sync1_bypass                  :    2;  /*set GPIO input_sync1 signal mode. 0:disable. 1:trigger at negedge. 2or3:trigger at posedge.*/
            uint32_t reserved5                     :    2;  /*reserved*/
            uint32_t int_type                      :    3;  /*set this value to choose interrupt mode. 0:disable GPIO interrupt. 1:trigger at posedge. 2:trigger at negedge. 3:trigger at any edge. 4:valid at low level. 5:valid at high level*/
            uint32_t wakeup_enable                 :    1;  /*set this bit to enable GPIO wakeup.(can only wakeup CPU from Light-sleep Mode)*/
            uint32_t config                        :    2;  /*reserved*/
            uint32_t int_ena                       :    5;  /*set bit 13 to enable CPU interrupt. set bit 14 to enable CPU(not shielded) interrupt.*/
            uint32_t reserved18                    :    14;  /*reserved*/
        };
        uint32_t val;
    } pin[57];
    union {
        struct {
            uint32_t func_sel                      :    6;  /*set this value: s=0-56: connect GPIO[s] to this port. s=0x3F: set this port always high level. s=0x3E: set this port always low level.*/
            uint32_t sig_in_inv                    :    1;  /*set this bit to invert input signal. 1:invert. 0:not invert.*/
            uint32_t sig_in_sel                    :    1;  /*set this bit to bypass GPIO. 1:do not bypass GPIO. 0:bypass GPIO.*/
            uint32_t reserved8                     :    24;  /*reserved*/
        };
        uint32_t val;
    } func_in_sel_cfg[256];
    union {
        struct {
            uint32_t func_sel                      :    9;  /*The value of the bits: 0<=s<=256. Set the value to select output signal. s=0-255: output of GPIO[n] equals input of peripheral[s]. s=256: output of GPIO[n] equals GPIO_OUT_REG[n].*/
            uint32_t inv_sel                       :    1;  /*set this bit to invert output signal.1:invert.0:not invert.*/
            uint32_t oe_sel                        :    1;  /*set this bit to select output enable signal.1:use GPIO_ENABLE_REG[n] as output enable signal.0:use peripheral output enable signal.*/
            uint32_t oe_inv_sel                    :    1;  /*set this bit to invert output enable signal.1:invert.0:not invert.*/
            uint32_t reserved12                    :    20;  /*reserved*/
        };
        uint32_t val;
    } func_out_sel_cfg[57];
    uint32_t intr_2;
    union {
        struct {
            uint32_t int1_2                        :    25;  /*GPIO interrupt 2 status register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } intr1_2;
    uint32_t intr_3;
    union {
        struct {
            uint32_t int1_3                        :    25;  /*GPIO interrupt 3 status register for GPIO32-56*/
            uint32_t reserved25                    :    7;  /*reserved*/
        };
        uint32_t val;
    } intr1_3;
    union {
        struct {
            uint32_t clk_en                        :    1;  /*set this bit to enable GPIO clock gate*/
            uint32_t reserved1                     :    31;  /*reserved*/
        };
        uint32_t val;
    } clock_gate;
    uint32_t reserved_650;
    uint32_t reserved_654;
    uint32_t reserved_658;
    uint32_t reserved_65c;
    uint32_t reserved_660;
    uint32_t reserved_664;
    uint32_t reserved_668;
    uint32_t reserved_66c;
    uint32_t reserved_670;
    uint32_t reserved_674;
    uint32_t reserved_678;
    uint32_t reserved_67c;
    uint32_t reserved_680;
    uint32_t reserved_684;
    uint32_t reserved_688;
    uint32_t reserved_68c;
    uint32_t reserved_690;
    uint32_t reserved_694;
    uint32_t reserved_698;
    uint32_t reserved_69c;
    uint32_t reserved_6a0;
    uint32_t reserved_6a4;
    uint32_t reserved_6a8;
    uint32_t reserved_6ac;
    uint32_t reserved_6b0;
    uint32_t reserved_6b4;
    uint32_t reserved_6b8;
    uint32_t reserved_6bc;
    uint32_t reserved_6c0;
    uint32_t reserved_6c4;
    uint32_t reserved_6c8;
    uint32_t reserved_6cc;
    uint32_t reserved_6d0;
    uint32_t reserved_6d4;
    uint32_t reserved_6d8;
    uint32_t reserved_6dc;
    uint32_t reserved_6e0;
    uint32_t reserved_6e4;
    uint32_t reserved_6e8;
    uint32_t reserved_6ec;
    uint32_t reserved_6f0;
    uint32_t reserved_6f4;
    uint32_t reserved_6f8;
    uint32_t reserved_6fc;
    union {
        struct {
            uint32_t comp0_pos_int_raw             :    1;  /*analog comparator pos edge interrupt raw*/
            uint32_t comp0_neg_int_raw             :    1;  /*analog comparator neg edge interrupt raw*/
            uint32_t comp0_all_int_raw             :    1;  /*analog comparator neg or pos edge interrupt raw*/
            uint32_t comp1_pos_int_raw             :    1;  /*analog comparator pos edge interrupt raw*/
            uint32_t comp1_neg_int_raw             :    1;  /*analog comparator neg edge interrupt raw*/
            uint32_t comp1_all_int_raw             :    1;  /*analog comparator neg or pos edge interrupt raw*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } int_raw;
    union {
        struct {
            uint32_t comp0_pos_int_st              :    1;  /*analog comparator pos edge interrupt status*/
            uint32_t comp0_neg_int_st              :    1;  /*analog comparator neg edge interrupt status*/
            uint32_t comp0_all_int_st              :    1;  /*analog comparator neg or pos edge interrupt status*/
            uint32_t comp1_pos_int_st              :    1;  /*analog comparator pos edge interrupt status*/
            uint32_t comp1_neg_int_st              :    1;  /*analog comparator neg edge interrupt status*/
            uint32_t comp1_all_int_st              :    1;  /*analog comparator neg or pos edge interrupt status*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } int_st;
    union {
        struct {
            uint32_t comp0_pos_int_ena             :    1;  /*analog comparator pos edge interrupt enable*/
            uint32_t comp0_neg_int_ena             :    1;  /*analog comparator neg edge interrupt enable*/
            uint32_t comp0_all_int_ena             :    1;  /*analog comparator neg or pos edge interrupt enable*/
            uint32_t comp1_pos_int_ena             :    1;  /*analog comparator pos edge interrupt enable*/
            uint32_t comp1_neg_int_ena             :    1;  /*analog comparator neg edge interrupt enable*/
            uint32_t comp1_all_int_ena             :    1;  /*analog comparator neg or pos edge interrupt enable*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } int_ena;
    union {
        struct {
            uint32_t comp0_pos_int_clr             :    1;  /*analog comparator pos edge interrupt clear*/
            uint32_t comp0_neg_int_clr             :    1;  /*analog comparator neg edge interrupt clear*/
            uint32_t comp0_all_int_clr             :    1;  /*analog comparator neg or pos edge interrupt clear*/
            uint32_t comp1_pos_int_clr             :    1;  /*analog comparator pos edge interrupt clear*/
            uint32_t comp1_neg_int_clr             :    1;  /*analog comparator neg edge interrupt clear*/
            uint32_t comp1_all_int_clr             :    1;  /*analog comparator neg or pos edge interrupt clear*/
            uint32_t reserved6                     :    26;
        };
        uint32_t val;
    } int_clr;
    uint32_t zero_det0_filter_cnt;
    uint32_t zero_det1_filter_cnt;
    uint32_t reserved_718;
    uint32_t reserved_71c;
    uint32_t reserved_720;
    uint32_t reserved_724;
    uint32_t reserved_728;
    uint32_t reserved_72c;
    uint32_t reserved_730;
    uint32_t reserved_734;
    uint32_t reserved_738;
    uint32_t reserved_73c;
    uint32_t reserved_740;
    uint32_t reserved_744;
    uint32_t reserved_748;
    uint32_t reserved_74c;
    uint32_t reserved_750;
    uint32_t reserved_754;
    uint32_t reserved_758;
    uint32_t reserved_75c;
    uint32_t reserved_760;
    uint32_t reserved_764;
    uint32_t reserved_768;
    uint32_t reserved_76c;
    uint32_t reserved_770;
    uint32_t reserved_774;
    uint32_t reserved_778;
    uint32_t reserved_77c;
    uint32_t reserved_780;
    uint32_t reserved_784;
    uint32_t reserved_788;
    uint32_t reserved_78c;
    uint32_t reserved_790;
    uint32_t reserved_794;
    uint32_t reserved_798;
    uint32_t reserved_79c;
    uint32_t reserved_7a0;
    uint32_t reserved_7a4;
    uint32_t reserved_7a8;
    uint32_t reserved_7ac;
    uint32_t reserved_7b0;
    uint32_t reserved_7b4;
    uint32_t reserved_7b8;
    uint32_t reserved_7bc;
    uint32_t reserved_7c0;
    uint32_t reserved_7c4;
    uint32_t reserved_7c8;
    uint32_t reserved_7cc;
    uint32_t reserved_7d0;
    uint32_t reserved_7d4;
    uint32_t reserved_7d8;
    uint32_t reserved_7dc;
    uint32_t reserved_7e0;
    uint32_t reserved_7e4;
    uint32_t reserved_7e8;
    uint32_t reserved_7ec;
    uint32_t reserved_7f0;
    uint32_t reserved_7f4;
    uint32_t reserved_7f8;
    union {
        struct {
            uint32_t date                          :    28;  /*version register*/
            uint32_t reserved28                    :    4;  /*reserved*/
        };
        uint32_t val;
    } date;
} gpio_dev_t;
extern gpio_dev_t GPIO;
#ifdef __cplusplus
}
#endif
