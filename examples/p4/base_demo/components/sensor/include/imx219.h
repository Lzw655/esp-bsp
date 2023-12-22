#pragma once

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Chip ID */
#define IMX219_REG_CHIP_ID		0x0000
#define IMX219_CHIP_ID			0x0219

/* External clock frequency is 24.0M */
#define IMX219_XCLK_FREQ		24000000

/* Pixel rate is fixed at 182.4M for all the modes */
#define IMX219_PIXEL_RATE		182400000

#define IMX219_DEFAULT_LINK_FREQ	456000000

/* V_TIMING internal */
#define IMX219_REG_VTS			0x0160
#define IMX219_VTS_15FPS		0x0dc6
#define IMX219_VTS_30FPS_1080P		0x06e3
#define IMX219_VTS_30FPS_BINNED		0x06e3
#define IMX219_VTS_30FPS_640x480	0x06e3
#define IMX219_VTS_MAX			0xffff

#define IMX219_VBLANK_MIN		4

/*Frame Length Line*/
#define IMX219_FLL_MIN			0x08a6
#define IMX219_FLL_MAX			0xffff
#define IMX219_FLL_STEP			1
#define IMX219_FLL_DEFAULT		0x0c98

/* HBLANK control - read only */
#define IMX219_PPL_DEFAULT		3448

/* Exposure control */
#define IMX219_REG_EXPOSURE		0x015a
#define IMX219_EXPOSURE_MIN		4
#define IMX219_EXPOSURE_STEP		1
#define IMX219_EXPOSURE_DEFAULT		0x640
#define IMX219_EXPOSURE_MAX		65535

/* Analog gain control */
#define IMX219_REG_ANALOG_GAIN		0x0157
#define IMX219_ANA_GAIN_MIN		0
#define IMX219_ANA_GAIN_MAX		232
#define IMX219_ANA_GAIN_STEP		1
#define IMX219_ANA_GAIN_DEFAULT		0x0

/* Digital gain control */
#define IMX219_REG_DIGITAL_GAIN		0x0158
#define IMX219_DGTL_GAIN_MIN		0x0100
#define IMX219_DGTL_GAIN_MAX		0x0fff
#define IMX219_DGTL_GAIN_DEFAULT	0x0100
#define IMX219_DGTL_GAIN_STEP		1

#define IMX219_REG_ORIENTATION		0x0172

/* Test Pattern Control */
#define IMX219_REG_TEST_PATTERN		0x0600
#define IMX219_TEST_PATTERN_DISABLE	0
#define IMX219_TEST_PATTERN_SOLID_COLOR	1
#define IMX219_TEST_PATTERN_COLOR_BARS	2
#define IMX219_TEST_PATTERN_GREY_COLOR	3
#define IMX219_TEST_PATTERN_PN9		4

/* Test pattern colour components */
#define IMX219_REG_TESTP_RED		0x0602
#define IMX219_REG_TESTP_GREENR		0x0604
#define IMX219_REG_TESTP_BLUE		0x0606
#define IMX219_REG_TESTP_GREENB		0x0608
#define IMX219_TESTP_COLOUR_MIN		0
#define IMX219_TESTP_COLOUR_MAX		0x03ff
#define IMX219_TESTP_COLOUR_STEP	1
#define IMX219_TESTP_RED_DEFAULT	IMX219_TESTP_COLOUR_MAX
#define IMX219_TESTP_GREENR_DEFAULT	0
#define IMX219_TESTP_BLUE_DEFAULT	0
#define IMX219_TESTP_GREENB_DEFAULT	0

/* IMX219 native and active pixel array size. */
#define IMX219_NATIVE_WIDTH		3296U
#define IMX219_NATIVE_HEIGHT		2480U
#define IMX219_PIXEL_ARRAY_LEFT		8U
#define IMX219_PIXEL_ARRAY_TOP		8U
#define IMX219_PIXEL_ARRAY_WIDTH	3280U
#define IMX219_PIXEL_ARRAY_HEIGHT	2464U

int imx219_init();

#ifdef __cplusplus
}
#endif