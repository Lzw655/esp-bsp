## esp32p4 系统结构总结

### 时钟复位

https://espressifsystems.sharepoint.com/sites/DigitalTeam/Docs%20for%20interteam%20collaboration/Forms/AllItems.aspx?id=%2Fsites%2FDigitalTeam%2FDocs%20for%20interteam%20collaboration%2F9%2E1%2E1%5Fhpc%5Fclkrst%5Fdiagram%5Fv34%5F1206%2Epdf&parent=%2Fsites%2FDigitalTeam%2FDocs%20for%20interteam%20collaboration

1) SYS_CLKRST

2) HP_CLKRST

3) PERI1_CLKRST

4) PERI2_CLKRST

### MEM

1) TCM

2) L2MEM

3) L1CACHE

4) L2CACHE

### 外设

1) GDMA

2) PDMA

3) MIPI

4) ISP

5) LP_IOMUX

6) IOMUX

### debug_probe

```c
void debug_probe(int debug_module, int debug_group, int debug_high)
{
    static const int debug_probe_pin[16]  = {6, 7, 8, 9, \
                                            10, 12, 13, 14, \
                                            15, 16, 17, 20, \
                                            21, 22, 23, 24};
    static const int debug_probe_sig[16] = {HP_PROBE_TOP_OUT0_IDX, HP_PROBE_TOP_OUT1_IDX, HP_PROBE_TOP_OUT2_IDX, HP_PROBE_TOP_OUT3_IDX, \
                                            PWM0_CH2_A_PAD_OUT_IDX, PWM0_CH2_B_PAD_OUT_IDX, PWM1_CH2_A_PAD_OUT_IDX, PWM1_CH2_B_PAD_OUT_IDX, \
                                            PWM0_CH0_A_PAD_OUT_IDX, PWM0_CH0_B_PAD_OUT_IDX, PWM0_CH1_A_PAD_OUT_IDX, PWM0_CH1_B_PAD_OUT_IDX, \
                                            PWM1_CH0_A_PAD_OUT_IDX, PWM1_CH0_B_PAD_OUT_IDX, PWM1_CH1_A_PAD_OUT_IDX, PWM1_CH1_B_PAD_OUT_IDX};
    for (int x = 0; x < 16; x++) {
        if (debug_probe_pin[x] < 24) {
            LP_IO_MUX.pad[debug_probe_pin[x]].mux_sel = 0;
        }
        if (debug_probe_pin[x] < 32) {
            GPIO.enable_w1ts = 1 << debug_probe_pin[x];
        } else {
            GPIO.enable1_w1ts = 1 << (debug_probe_pin[x] - 32);
        }
        GPIO.func_out_sel[debug_probe_pin[x]].func_sel = debug_probe_sig[x];
        GPIO.func_out_sel[debug_probe_pin[x]].inv_sel = 0;
        GPIO.func_out_sel[debug_probe_pin[x]].oen_sel = 0;
        GPIO.func_out_sel[debug_probe_pin[x]].oen_inv_sel = 0;
    }
    HP_SYS.hp_probe_ctrl.pwm_probe_mux_sel = 0;
    HP_SYS.hp_probea_ctrl.probe_global_en = 1;
    HP_SYS.hp_probea_ctrl.probe_l_sel = debug_high ? 1 : 0;
    HP_SYS.hp_probea_ctrl.probe_h_sel = debug_high ? 0 : 1;
    HP_SYS.hp_probea_ctrl.probe_a_mod_sel = debug_group;
    HP_SYS.hp_probea_ctrl.probe_a_top_sel = debug_module;
    HP_SYS.hp_probeb_ctrl.probe_b_en = 0;
    printf("hp_probe_ctrl: 0x%x\n", HP_SYS.hp_probe_ctrl.val);
    printf("hp_probea_ctrl: 0x%x\n", HP_SYS.hp_probea_ctrl.val);
    printf("hp_probeb_ctrl: 0x%x\n", HP_SYS.hp_probeb_ctrl.val);
    printf("hp_probe_out: 0x%x\n", HP_SYS.hp_probe_out);
}
```
debug_probe(0, 0, 1);

sys_clk -> hp_clk_probe[1] -> IO23
peri1_clk -> hp_clk_probe[2] -> IO22
peri2_clk -> hp_clk_probe[3] -> IO21

可以通过 debug_probe 抓取以上 clk 的二分频波形。

debug_probe(4, 0, 0);

usb_otg11_phy_clk_div2 -> IO10

可以通过 debug_probe 抓取 usb_otg11_phy_clk 的二分频波形。