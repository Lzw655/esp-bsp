#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "csi.h"

static void delay_us(uint32_t t)
{
#if TEST_CSI_FPGA
    for (uint32_t tu = 0; tu < t; tu++);
#else
    ets_delay_us(t);
#endif
}
static void delay_ms(uint32_t t)
{
    delay_us(1000 * t);
}

/* *************************************************************** */
/* Global Function Declaration                                     */
/* *************************************************************** */

// Initialization of MIPI-CSI, including Bridge, Host-Controller and D-PHY.
int  mipi_csi_initialization (void);

// Initialization of MIPI-CSI Host-Controller and D-PHY.
int  mipi_csi_host_phy_initialization (void);

// Initialization of MIPI-CSI Bridge.
int  mipi_csi_bridge_initialization   (void);

// Write register "CSI2_HOST_PHY_TEST_CTRL0".
void mipi_csi_update_phy_test_ctrl0 (uint32_t phy_testclk ,
                                     uint32_t phy_testclr );

// Write register "CSI2_HOST_PHY_TEST_CTRL1".
void mipi_csi_update_phy_test_ctrl1 (uint32_t phy_testen  ,
                                     uint32_t phy_testdin );

/* *************************************************************** */
/* Global Variable Declaration                                     */
/* *************************************************************** */

/* *************************************************************** */
/* Implementation                                                  */
/* *************************************************************** */

/* --------------------------------------------------------
 * @brief  : Write register "CSI2_HOST_PHY_TEST_CTRL0".
 * @params : <phy_testclk> 
 *           <phy_testclr>
 * @ret    : void
 **/
void mipi_csi_update_phy_test_ctrl0 (uint32_t phy_testclk ,
                                     uint32_t phy_testclr )
{
    MIPI_CSI_HOST.phy_test_ctrl0.val = ( 0x00000000 | ((phy_testclk<<1)&0x00000002) | ((phy_testclr<<0)&0x00000001) );
}

/* --------------------------------------------------------
 * @brief  : Write register "CSI2_HOST_PHY_TEST_CTRL1".
 * @params : <phy_testen> 
 *           <phy_testdin>
 * @ret    : void
 **/
void mipi_csi_update_phy_test_ctrl1 (uint32_t phy_testen  ,
                                     uint32_t phy_testdin )
{
    MIPI_CSI_HOST.phy_test_ctrl1.val = ( 0x00000000 | ((phy_testen<<16)&0x00010000) | ((phy_testdin<<0)&0x000000ff) );
}

static void mipi_csi_dphy_write_control (uint32_t testcode, uint32_t testwrite)
{
    MIPI_CSI_HOST.phy_test_ctrl1.val  = 0x00010000| testcode ;
    MIPI_CSI_HOST.phy_test_ctrl0.val  = 0x00000002           ;
    MIPI_CSI_HOST.phy_test_ctrl0.val  = 0x00000000           ;
    MIPI_CSI_HOST.phy_test_ctrl1.val  = 0x00000000| testwrite;
    MIPI_CSI_HOST.phy_test_ctrl0.val  = 0x00000002           ;
    MIPI_CSI_HOST.phy_test_ctrl0.val  = 0x00000000           ;
}

/* --------------------------------------------------------
 * @brief  : Initialization of MIPI-CSI Host-Controller and D-PHY.
 * @params : void
 * @ret    : int
 **/
int mipi_csi_host_phy_initialization (void)
{
#if !TEST_CSI_FPGA
    // ---------------------------------------
    // 1. Release D-PHY test codes from reset.
    // ---------------------------------------
    mipi_csi_update_phy_test_ctrl1(0,0);    // phy_testen  = 0; phy_testdin = 8'd0;
    mipi_csi_update_phy_test_ctrl0(0,1);    // phy_testclk = 0; phy_testclr = 1;
    // Release phy_testclr.
    mipi_csi_update_phy_test_ctrl0(0,0);    // phy_testclk = 0; phy_testclr = 0;

    uint8_t hs_freq = 0x1A;
	struct
	{
		uint32_t freq;	   /* upper margin of frequency range */
		uint8_t hs_freq;   /* hsfreqrange */
		uint8_t vco_range; /* vcorange */
	} pll_ranges[] =
		{
			{90, 0x00, 0x00},
			{100, 0x10, 0x00},
			{110, 0x20, 0x00},
			{130, 0x01, 0x00},
			{140, 0x11, 0x00},
			{150, 0x21, 0x00},
			{170, 0x02, 0x00},
			{180, 0x12, 0x00},
			{200, 0x22, 0x00},
			{220, 0x03, 0x01},
			{240, 0x13, 0x01},
			{250, 0x23, 0x01},
			{270, 0x04, 0x01},
			{300, 0x14, 0x01},
			{330, 0x05, 0x02},
			{360, 0x15, 0x02},
			{400, 0x25, 0x02},
			{450, 0x06, 0x02},
			{500, 0x16, 0x02},
			{550, 0x07, 0x03},
			{600, 0x17, 0x03},
			{650, 0x08, 0x03},
			{700, 0x18, 0x03},
			{750, 0x09, 0x04},
			{800, 0x19, 0x04},
			{850, 0x29, 0x04},
			{900, 0x39, 0x04},
			{950, 0x0A, 0x05},
			{1000, 0x1A, 0x05},
			{1050, 0x2A, 0x05},
			{1100, 0x3A, 0x05},
			{1150, 0x0B, 0x06},
			{1200, 0x1B, 0x06},
			{1250, 0x2B, 0x06},
			{1300, 0x3B, 0x06},
			{1350, 0x0C, 0x07},
			{1400, 0x1C, 0x07},
			{1450, 0x2C, 0x07},
			{1500, 0x3C, 0x07}
    };

    for (int x = 0; ; x++) {
        if (pll_ranges[x].freq == 1500) {
            hs_freq = pll_ranges[x].hs_freq;
            break;
        }
        if ((MIPI_CSI_LINE_RATE >= pll_ranges[x].freq * 1000000) && (MIPI_CSI_LINE_RATE < pll_ranges[x+1].freq * 1000000)) {
            hs_freq = pll_ranges[x].hs_freq;
            break;
        }
    }
    // ---------------------------------------
    // 2. Configure D-PHY frequency range (for 1Gbits).
    // ---------------------------------------
    mipi_csi_dphy_write_control (0x44, hs_freq << 1);
#endif

    MIPI_CSI_HOST.phy_shutdownz.phy_shutdownz = 1;
    MIPI_CSI_HOST.dphy_rstz.dphy_rstz = 1;
    MIPI_CSI_HOST.csi2_resetn.csi2_resetn = 1;

    // ---------------------------------------
    // 4. Configure the host controller.
    // ---------------------------------------

    // Configure the number of active lanes.
    MIPI_CSI_HOST.n_lanes.val = (2 - 1);

    // Enable VCX.
    MIPI_CSI_HOST.vc_extension.val = 0x00000000;

    // Set Scrambler.
    MIPI_CSI_HOST.scrambling.val = 0x00000000;

    // ---------------------------------------
    // 5. Done.
    // ---------------------------------------
    printf("[MIPI-HAL] MIPI CSI Host Controller & D-PHY initialization done.\n");
    return 0;
}

/* --------------------------------------------------------
 * @brief  : Initialization of MIPI-CSI Bridge.
 * @params : void
 * @ret    : int
 **/
int mipi_csi_bridge_initialization (void) 
{
    uint32_t rtn;

    // Set Frame lines.
    MIPI_CSI_BRIDGE.frame_cfg.hadr_num = MIPI_CSI_IMAGE_HSIZE;
    MIPI_CSI_BRIDGE.frame_cfg.vadr_num = MIPI_CSI_IMAGE_VSIZE;

    MIPI_CSI_BRIDGE.buf_flow_ctl.csi_buf_afull_thrd = 960;
    MIPI_CSI_BRIDGE.dma_req_cfg.dma_burst_len = 256;

    // MIPI_CSI_BRIDGE.frame_cfg.has_hsync_e = 0x1;
    printf("has_hsync_e: 0x%x\n", MIPI_CSI_BRIDGE.frame_cfg.has_hsync_e);

    // MIPI_CSI_BRIDGE.data_type_cfg.data_type_min = 0x20;
    printf("csi data_type_min: 0x%x, data_type_max: 0x%x, dma_req_interval: %d\n", MIPI_CSI_BRIDGE.data_type_cfg.data_type_min, MIPI_CSI_BRIDGE.data_type_cfg.data_type_max, MIPI_CSI_BRIDGE.dma_req_interval.dma_req_interval);
    MIPI_CSI_BRIDGE.data_type_cfg.data_type_min = 0x12;

#if TEST_CSI_COLOR_MODE == MIPI_CSI_YUV422_MODE
    MIPI_CSI_BRIDGE.endian_mode.byte_endian_order = 1;
#else
    MIPI_CSI_BRIDGE.endian_mode.byte_endian_order = 0;
#endif

    // Enable CSI Bridge.
    MIPI_CSI_BRIDGE.en.val = 0x1;

    printf("[MIPI-HAL] MIPI CSI Bridge initialization done.\n");
    return 0;
}

/* --------------------------------------------------------
 * @brief  : Initialization of MIPI-CSI, including Bridge,
 *           Host-Controller and D-PHY.
 * @params : void
 * @ret    : int
 **/
int mipi_csi_initialization (void) 
{
    int stat;

    stat = mipi_csi_host_phy_initialization();
    if(stat) {
        printf("[MIPI-HAL] Error - Initialize MIPI CSI Host&PHY failed.\n");
        return 1;
    }

    // stat = mipi_csi_bridge_initialization();
    // if(stat) {
    //     printf("[MIPI-HAL] Error - Initialize MIPI CSI Bridge failed.\n");
    //     return 1;
    // }

    printf("[MIPI-HAL] MIPI CSI initialization done.\n");    
    return 0;
}
