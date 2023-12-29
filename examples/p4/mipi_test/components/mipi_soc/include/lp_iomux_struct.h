/*
 * SPDX-FileCopyrightText: 2017-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _SOC_LP_IOMUX_STRUCT_H_
#define _SOC_LP_IOMUX_STRUCT_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
    union {
        struct {
            uint32_t clk_en                        :    1;  /*Reserved*/
            uint32_t reserved1                     :    31;
        };
        uint32_t val;
    } clk_en;
    union {
        struct {
            uint32_t ver_date                      :    28;  /*Reserved*/
            uint32_t reserved28                    :    4;
        };
        uint32_t val;
    } ver_date;
    union {
        struct {
            uint32_t drv                           :    2;  /*Reserved*/
            uint32_t rde                           :    1;  /*Reserved*/
            uint32_t rue                           :    1;  /*Reserved*/
            uint32_t mux_sel                       :    1;  /*1:use LP GPIO,0: use digital GPIO*/
            uint32_t fun_sel                       :    2;  /*function sel*/
            uint32_t ssel                          :    1;  /*1: enable sleep mode during sleep,0: no sleep mode*/
            uint32_t sie                           :    1;  /*input enable in sleep mode*/
            uint32_t soe                           :    1;  /*output enable in sleep mode*/
            uint32_t fun_ie                        :    1;  /*input enable in work mode*/
            uint32_t filter_en                     :    1;  /*need des*/
            uint32_t reserved12                    :    20;
        };
        uint32_t val;
    } pad[16];
    union {
        struct {
            uint32_t xtl_ext_ctr_sel               :    5;  /*select LP GPIO 0 ~ 15 to control XTAL*/
            uint32_t ext_wakeup0_sel               :    5;  /*Reserved*/
            uint32_t reserved10                    :    22;
        };
        uint32_t val;
    } ext_wakeup0_sel;
    union {
        struct {
            uint32_t gpio                          :    16;  /*Reserved*/
            uint32_t reserved16                    :    16;
        };
        uint32_t val;
    } pad_hold;
} lp_iomux_dev_t;
extern lp_iomux_dev_t LP_IOMUX;
#ifdef __cplusplus
}
#endif



#endif /*_SOC_LP_IOMUX_STRUCT_H_ */
