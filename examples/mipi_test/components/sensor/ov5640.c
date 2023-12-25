#include "ov5640.h"
#include "csi.h"
#include "sccb.h"

static uint8_t OV5640_ADDR;

static struct reginfo sensor_init_data[] =
{    
        //[7]=0 Software reset; [6]=1 Software power down; Default=0x02
        {0x3008, 0x42},
        //[1]=1 System input clock from PLL; Default read = 0x11
        {0x3103, 0x03},
        //[3:0]=0000 MD2P,MD2N,MCP,MCN input; Default=0x00
        {0x3017, 0x00},
        //[7:2]=000000 MD1P,MD1N, D3:0 input; Default=0x00
        {0x3018, 0x00},
        //[6:4]=001 PLL charge pump, [3:0]=1000 MIPI 8-bit mode
        {0x3034, (TEST_CSI_COLOR_MODE==MIPI_CSI_RAW10_MODE) ? 0x1A : 0x18},
        //PLL1 configuration
        //[7:4]=0001 System clock divider /1, [3:0]=0001 Scale divider for MIPI /1
        {0x3035, 0x11},
        //[7:0]=56 PLL multiplier
        {0x3036, 0x38},
        //[4]=1 PLL root divider /2, [3:0]=1 PLL pre-divider /1
        {0x3037, 0x11},
        //[5:4]=00 PCLK root divider /1, [3:2]=00 SCLK2x root divider /1, [1:0]=01 SCLK root divider /2
        {0x3108, 0x01},
        //PLL2 configuration
        //[5:4]=01 PRE_DIV_SP /1.5, [2]=1 R_DIV_SP /1, [1:0]=00 DIV12_SP /1
        {0x303D, 0x10},
        //[4:0]=11001 PLL2 multiplier DIV_CNT5B = 25
        {0x303B, 0x19},

        {0x3630, 0x2e},
        {0x3631, 0x0e},
        {0x3632, 0xe2},
        {0x3633, 0x23},
        {0x3621, 0xe0},
        {0x3704, 0xa0},
        {0x3703, 0x5a},
        {0x3715, 0x78},
        {0x3717, 0x01},
        {0x370b, 0x60},
        {0x3705, 0x1a},
        {0x3905, 0x02},
        {0x3906, 0x10},
        {0x3901, 0x0a},
        {0x3731, 0x02},
        //VCM debug mode
        {0x3600, 0x37},
        {0x3601, 0x33},
        //System control register changing not recommended
        {0x302d, 0x60},
        //??
        {0x3620, 0x52},
        {0x371b, 0x20},
        //?? DVP
        {0x471c, 0x50},

        {0x3a13, 0x43},
        {0x3a18, 0x00},
        {0x3a19, 0xf8},
        {0x3635, 0x13},
        {0x3636, 0x06},
        {0x3634, 0x44},
        {0x3622, 0x01},
        {0x3c01, 0x34},
        {0x3c04, 0x28},
        {0x3c05, 0x98},
        {0x3c06, 0x00},
        {0x3c07, 0x08},
        {0x3c08, 0x00},
        {0x3c09, 0x1c},
        {0x3c0a, 0x9c},
        {0x3c0b, 0x40},

        //[7]=1 color bar enable, [3:2]=00 eight color bar
        {0x503d, 0x00},
        //[2]=1 ISP vflip, [1]=1 sensor vflip
        {0x3820, 0x46},

        //[7:5]=001 Two lane mode, [4]=0 MIPI HS TX no power down, [3]=0 MIPI LP RX no power down, [2]=1 MIPI enable, [1:0]=10 Debug mode; Default=0x58
        {0x300e, 0x45},
        //[5]=0 Clock free running, [4]=1 Send line short packet, [3]=0 Use lane1 as default, [2]=1 MIPI bus LP11 when no packet; Default=0x04
        {0x4800, TEST_CSI_LINESYNC ? 0x14 : 0x04},
        {0x302e, 0x08},
        //[7:4]=0x3 YUV422, [3:0]=0x0 YUYV
        //{0x4300, 0x30},
        //[7:4]=0x6 RGB565, [3:0]=0x0 {b[4:0],g[5:3],g[2:0],r[4:0]}
        {0x4300, 0x6f},
        {0x501f, 0x01},

        {0x4713, 0x03},
        {0x4407, 0x04},
        {0x440e, 0x00},
        {0x460b, 0x35},
        //[1]=0 DVP PCLK divider manual control by 0x3824[4:0]
        {0x460c, 0x20},
        //[4:0]=1 SCALE_DIV=INT(3824[4:0]/2)
        {0x3824, 0x01},
        //[7]=1 LENC correction enabled, [5]=1 RAW gamma enabled, [2]=1 Black pixel cancellation enabled, [1]=1 White pixel cancellation enabled, [0]=1 Color interpolation enabled
        {0x5000, 0x07},
        //[7]=0 Special digital effects, [5]=0 scaling, [2]=0 UV average disabled, [1]=1 Color matrix enabled, [0]=1 Auto white balance enabled
        {0x5001, 0x03},

        {SEQUENCE_END, 0x00}
};

//              +----------------+        +------------------+         +---------------------+        +---------------------+
//XVCLK         | PRE_DIV0       |        | Mult (4+252)     |         | Sys divider (0=16)  |        | MIPI divider (0=16) |
//+-------+-----> 3037[3:0]=0101 +--------> 3036[7:0]=40    +---------> 3035[7:4]=0010      +--------> 3035[3:0]=0001       |
//12MHz   |     | / 1.5          |  8MHz  | * 40             | 320MHz  | / 2                 | 160MHz | / 1                 |
//        |     +----------------+        +------------------+         +----------+----------+        +----------+----------+
//        |                                                                       |                              |
//        |                                                                       |                      MIPISCLK|160MHz
//        |                                                                       |                              |
//        |     +----------------+        +------------------+         +----------v----------+        +----------v----------+
//        |     | PRE_DIVSP      |        | R_DIV_SP         |         | PLL R divider       |        | MIPI PHY            | MIPI_CLK
//        +-----> 303d[5:4]=01   +--------> 303d[2]=0 (+1)   |         | 3037[4]=0           |        |                     +------->
//              | / 1.5          |  8MHz  | / 1              |         | / 1                 |        | / 2                 | 80MHz
//              +----------------+        +---------+--------+         +----------+----------+        +---------------------+
//                                                  |                             |
//                                                  |                             |160MHz
//                                                  |                             |
//              +----------------+        +---------v--------+         +----------v----------+        +---------------------+
//              | SP divider     |        | Mult             |         | BIT div (MIPI 8/10) |        | SCLK divider        | SCLK
//              | 303c[3:0]=0x1  +<-------+ 303b[4:0]=0x19   |         | 3034[3:0]=0x8)      +----+---> 3108[1:0]=01 (2^)   +------->
//              | / 1            | 200MHz | * 25             |         | / 2                 |    |   | / 2                 | 40MHz
//              +--------+-------+        +------------------+         +----------+----------+    |   +---------------------+
//                       |                                                        |               |
//                       |                                                        | 80MHz         |
//                       |                                                        |               |
//              +--------v-------+                                     +----------v----------+    |   +---------------------+
//              | R_SELD5 div    | ADCCLK                              | PCLK div            |    |   | SCLK2x divider      |
//              | 303d[1:0]=001  +------->                             | 3108[5:4]=01 (2^)   |    +---> 3108[3:2]=00 (2^)   +------->
//              | / 1            | 200MHz                              | / 2                 |        | / 1                 | 80MHz
//              +----------------+                                     +----------+----------+        +---------------------+
//                                                                                |
//                                                                                | 40MHz
//                                                                                |
//                                                                     +----------v----------+        +---------------------+
//                                                                     | P divider (* #lanes)| PCLK   | Scale divider       |
//                                                                     | 3035[3:0]=0001      +--------> 3824[4:0]           +------->
//                                                                     | / 1                 | 40MHz  | / 2                  | 20MHz
//                                                                     +---------------------+        +---------------------+     
const struct reginfo cfg_720p_preview[] =
{
        //RGB888, RAW8, MIPISCLK=160M, SCLK=40Mz, PCLK=40M
        //PLL1 configuration
        //[7:4]=0010 System clock divider /2, [3:0]=0001 Scale divider for MIPI /1
        {0x3035, 0x21},
        //[7:0]=40 PLL multiplier
        {0x3036, MIPI_IDI_CLOCK_RATE / 1000000},
        //[4]=0 PLL root divider /1, [3:0]=5 PLL pre-divider /1.5
        {0x3037, 0x05},
        //[5:4]=01 PCLK root divider /2, [3:2]=00 SCLK2x root divider /1, [1:0]=01 SCLK root divider /2
        {0x3108, 0x11},

        //[3:0]=0 X address start high byte
        {0x3800, (0 >> 8) & 0x0F},
        //[7:0]=0 X address start low byte
        {0x3801, 0 & 0xFF},
        //[2:0]=0 Y address start high byte
        {0x3802, (8 >> 8) & 0x07},
        //[7:0]=0 Y address start low byte
        {0x3803, 8 & 0xFF},

        //[3:0] X address end high byte
        {0x3804, (2480 >> 8) & 0x0F},
        //[7:0] X address end low byte
        {0x3805, 2480 & 0xFF},
        //[2:0] Y address end high byte
        {0x3806, (1947 >> 8) & 0x07},
        //[7:0] Y address end low byte
        {0x3807, 1947 & 0xFF},

        //[3:0]=0 timing hoffset high byte
        {0x3810, (0 >> 8) & 0x0F},
        //[7:0]=0 timing hoffset low byte
        {0x3811, 0 & 0xFF},
        //[2:0]=0 timing voffset high byte
        {0x3812, (0 >> 8) & 0x07},
        //[7:0]=0 timing voffset low byte
        {0x3813, 0 & 0xFF},

        //[3:0] Output horizontal width high byte
        {0x3808, (MIPI_CSI_IMAGE_HSIZE >> 8) & 0x0F},
        //[7:0] Output horizontal width low byte
        {0x3809, MIPI_CSI_IMAGE_HSIZE & 0xFF},
        //[2:0] Output vertical height high byte
        {0x380a, (MIPI_CSI_IMAGE_VSIZE >> 8) & 0x7F},
        //[7:0] Output vertical height low byte
        {0x380b, MIPI_CSI_IMAGE_VSIZE & 0xFF},

        //HTS line exposure time in # of pixels
        {0x380c, (1896 >> 8) & 0x1F},
        {0x380d, 1896 & 0xFF},
        //VTS frame exposure time in # lines
        {0x380e, (984 >> 8) & 0xFF},
        {0x380f, 984 & 0xFF},

        //[7:4]=0x3 horizontal odd subsample increment, [3:0]=0x1 horizontal even subsample increment
        {0x3814, 0x31},
        //[7:4]=0x3 vertical odd subsample increment, [3:0]=0x1 vertical even subsample increment
        {0x3815, 0x31},

        //[2]=0 ISP mirror, [1]=0 sensor mirror, [0]=1 horizontal binning
        {0x3821, 0x01},

        //little MIPI shit: global timing unit, period of PCLK in ns * 2(depends on # of lanes)
        {0x4837, (1000000000 / MIPI_IDI_CLOCK_RATE) * 2}, // 1/40M*2

        //Undocumented anti-green settings
        {0x3618, 0x00}, // Removes vertical lines appearing under bright light
        {0x3612, 0x59},
        {0x3708, 0x64},
        {0x3709, 0x52},
        {0x370c, 0x03},

        // FORMAT CONTROL 00 0xF8 RW
        //           0x0: RAW
        //                Bit[3:0]: Output sequence
        //                          0x0: BGBG... / GRGR...
        //                          0x1: GBGB... / RGRG...
        //                          0x2: GRGR... / BGBG...
        //                          0x3: RGRG... / GBGB...
        //                          0x4~0xF: Not allowed
        //           0x1: Y8
        //                Bit[3:0]: Does not matter
        //           0x2: YUV444/RGB888 (not available for full resolution)
        //                Bit[3:0]: Output sequence
        //                          0x0: YUVYUV..., or GBRGBR...
        //                          0x1: YVUYVU..., or GRBGRB...
        //                          0x2: UYVUYV..., or BGRBGR...
        //                          0x3: VYUVYU..., or RGBRGB...
        //                          0x4: UVYUVY..., or BRGBRG...
        //                          0x5: VUYVUY..., or RBGRBG...
        //                          0x6~0xE: Not allowed
        //                          0xF: UYVUYV..., or BGRBGR...
        //           0x3: YUV422
        //                Bit[3:0]: Output sequence
        //                          0x0: YUYV...
        //                          0x1: YVYU...
        //                          0x2: UYVY...
        //                          0x3: VYUY...
        //                          0x4~0xE: Not allowed
        //                          0xF: UYVY...
        //           0x4: YUV420
        //                Bit[3:0]: Output sequence
        //                          0x0: YYYY... / YUYV...
        //                          0x1: YYYY... / YVYU...
        //                          0x2: YYYY... / UYVY...
        //                          0x3: YYYY... / VYUY...
        //                          0x4: YUYV... / YYYY...
        //                          0x5: YVYU... / YYYY...
        //                          0x6: UYVY... / YYYY...
        //                          0x7: VYUY... / YYYY...
        //                          0x8~0xE: Not allowed
        //                          0xF: YYYY... / UYVY...
        //           0x5: YUV420 (for MIPI only)
        //                Bit[3:0]: Output sequence
        //                          0x0~0xD: Not allowed
        //                          0xE: VYYVYY... / UYYUYY...
        //                          0xF: UYYUYY... / VYYVYY...
        //           0x6: RGB565
        //                Bit[3:0]: Output sequence
        //                          0x0: {b[4:0],g[5:3]}, {g[2:0],r[4:0]}
        //                          0x1: {r[4:0],g[5:3]}, {g[2:0],b[4:0]}
        //                          0x2: {g[4:0],r[5:3]}, {r[2:0],b[4:0]}
        //                          0x3: {b[4:0],r[5:3]}, {r[2:0],g[4:0]}
        //                          0x4: {g[4:0],b[5:3]}, {b[2:0],r[4:0]}
        //                          0x5: {r[4:0],b[5:3]}, {b[2:0],g[4:0]}
        //                          0x6~0xE: Not allowed
        //                          0xF: {g[2:0],b[4:0]}, {r[4:0],g[5:3]}
        //           0x7: RGB555 format 1
        //                Bit[3:0]: Output sequence
        //                          0x0: {b[4:0],g[4:2]},  {g[1:0],1'b0,r[4:0]}
        //                          0x1: {r[4:0],g[4:2]}, {g[1:0],1'b0,b[4:0]}
        //                          0x2: {g[4:0],r[4:2]}, {r[1:0],1'b0,b[4:0]}
        //                          0x3: {b[4:0],r[4:2]}, {r[1:0],1'b0,g[4:0]}
        //                          0x4: {r[4:0],b[4:2]}, {b[1:0],1'b0,g[4:0]}
        //                          0x5: {g[4:0],b[4:2]}, {b[1:0],1'b0,r[4:0]}
        //                          0x6~0xE: Not allowed
        //                          0xF: {g[1:0],1'b0,b[4:0]}, {r[4:0],g[4:2]}
        //           0x8: RGB555 format 2
        //                Bit[3:0]: Output sequence
        //                          0x0: {1'b0,b[4:0],g[4:3]}, {g[2:0],r[4:0]}
        //                          0x1: {1'b0,r[4:0],g[4:2]}, {g[2:0],b[4:0]}
        //                          0x2: {1'b0,g[4:0],r[4:2]}, {r[2:0],b[4:0]}
        //                          0x3: {1'b0,b[4:0],r[4:2]}, {r[2:0],g[4:0]}
        //                          0x4: {1'b0,r[4:0],b[4:2]}, {b[2:0],g[4:0]}
        //                          0x5: {1'b0,g[4:0],b[4:2]}, {b[2:0],r[4:0]}
        //                          0x6: {b[4:0],1'b0,g[4:3]}, {g[2:0],r[4:0]}
        //                          0x7: {r[4:0],1'b0,g[4:2]}, {g[2:0],b[4:0]}
        //                          0x8: {g[4:0],1'b0,r[4:2]}, {r[2:0],b[4:0]}
        //                          0x9: {b[4:0],1'b0,r[4:2]}, {r[2:0],g[4:0]}
        //                          0xA: {r[4:0],1'b0,b[4:2]}, {b[2:0],g[4:0]}
        //                          0xB: {g[4:0],1'b0,b[4:2]}, {b[2:0],r[4:0]}
        //                          0xC~0xF: Not allowed
        //           0x9: RGB444 format 1
        //                Bit[3:0]: Output sequence
        //                          0x0: {1'b0,b[3:0],2'h0,g[3]}, {g[2:0],1'b0,r[3:0]}
        //                          0x1: {1'b0,r[3:0],2'h0,g[3]}, {g[2:0],1'b0,b[3:0]}
        //                          0x2: {1'b0,g[3:0],2'h0,r[3]}, {r[2:0],1'b0,b[3:0]}
        //                          0x3: {1'b0,b[3:0],2'h0,r[3]}, {r[2:0],1'b0,g[3:0]}
        //                          0x4: {1'b0,r[3:0],2'h0,b[3]}, {b[2:0],1'b0,g[3:0]}
        //                          0x5: {1'b0,g[3:0],2'h0,b[3]}, {b[2:0],1'h0,r[3:0]}
        //                          0x6: {b[3:0],1'b0,g[3:1]}, {g[0],2'h0,r[3:0],1'b0}
        //                          0x7: {r[3:0],1'b0,g[3:1]}, {g[0],2'h0,b[3:0],1'b0}
        //                          0x8: {g[3:0],1'b0,r[3:1]}, {r[0],2'h0,b[3:0],1'b0}
        //                          0x9: {b[3:0],1'b0,r[3:1]}, {r[0],2'h0,g[3:0],1'b0}
        //                          0xA: {r[3:0],1'b0,b[3:1]}, {b[0],2'h0,g[3:0],1'b0}
        //                          0xB: {g[3:0],1'b0,b[3:1]}, {b[0],2'h0,r[3:0],1'b0}
        //                          0xC~0xE: Not allowed
        //                          0xF: {g[0],2'h2,b[3:0],1'b1}, {r[3:0],1'b1,g[3:1]}
        //           0xA: RGB444 format 2
        //                Bit[3:0]: Output sequence
        //                          0x0: {4'b0,b[3:0]}, {g[3:0],r[3:0]}
        //                          0x1: {4'b0,r[3:0]}, {g[3:0],b[3:0]}
        //                          0x2: {4'b0,b[3:0]}, {r[3:0],g[3:0]}
        //                          0x3: {4'b0,r[3:0]}, {b[3:0],g[3:0]}
        //                          0x4: {4'b0,g[3:0]}, {b[3:0],r[3:0]}
        //                          0x5: {4'b0,g[3:0]}, {r[3:0],b[3:0]}
        //                          0x6: {b[3:0],g[3:0],2'h0}, {r[3:0],b[3:0],2'h0,g[3:0],r[3:0],2'h0}
        //                          0x7: {r[3:0],g[3:0],2'h0}, {b[3:0],r[3:0],2'h0,g[3:0],b[3:0],2'h0}
        //                          0x8: {b[3:0],r[3:0],2'h0}, {g[3:0],b[3:0],2'h0,r[3:0],g[3:0],2'h0}
        //                          0x9: {r[3:0],b[3:0],2'h0}, {g[3:0],r[3:0],2'h0,b[3:0],g[3:0],2'h0}
        //                          0xA: {g[3:0],b[3:0],2'h0}, {r[3:0],g[3:0],2'h0,b[3:0],r[3:0],2'h0}
        //                          0xB: {g[3:0],r[3:0],2'h0}, {b[3:0],g[3:0],2'h0,r[3:0],b[3:0],2'h0}
        //                          0xC~0xF: Not allowed
        //           0xB~0xE: Not allowed
        //           0xF: Bypass formatter module (not recommended)
        //                Bit[3:0]: Output format
        //                          0x8: Raw
        //                          0x9: YUV422
        //                          0xA: YUV444
        //                          0xE: VYYVYY.../UYYUYY
        //                          0xF: UYYUYY.../VYYVYY

#if (TEST_CSI_COLOR_MODE == MIPI_CSI_RGB565_MODE)
        {0x4300, 0x6F},
        //[2:0]=0x0 Format select YUV422
        {0x501f, 0x01},
        {0x5001 ,0x23},
#elif (TEST_CSI_COLOR_MODE == MIPI_CSI_RGB888_MODE)
        {0x4300, 0x25},
        //[2:0]=0x0 Format select YUV422
        {0x501f, 0x00},
        {0x5001 ,0x25},
#elif (TEST_CSI_COLOR_MODE >= MIPI_CSI_RAW8_MODE)
        //[7:4]=0x0 Formatter RAW, [3:0]=0x0 BGBG/GRGR
        {0x4300, 0x00},
        //[2:0]=0x3 Format select ISP RAW (DPC)
        {0x501f, 0x03},
        {0x5001 ,0x03},
#endif
#if (TEST_CSI_PATTERN)
        {0x4741, 0x05},
        {0x503D, 0x80},
#endif
        {SEQUENCE_END, 0x00}
};

const struct reginfo cfg_1080p_preview[] =
{
        //RGB888, RAW8, MIPISCLK=160M, SCLK=40Mz, PCLK=40M
        //PLL1 configuration
        //[7:4]=0010 System clock divider /2, [3:0]=0001 Scale divider for MIPI /1
        {0x3035, 0x21},
        //[7:0]=40 PLL multiplier
        {0x3036, MIPI_IDI_CLOCK_RATE / 1000000},
        //[4]=0 PLL root divider /1, [3:0]=5 PLL pre-divider /1.5
        {0x3037, 0x05},
        //[5:4]=01 PCLK root divider /2, [3:2]=00 SCLK2x root divider /1, [1:0]=01 SCLK root divider /2
        {0x3108, 0x11},

        //[3:0]=0 X address start high byte
        {0x3800, (0 >> 8) & 0x0F},
        //[7:0]=0 X address start low byte
        {0x3801, 0 & 0xFF},
        //[2:0]=0 Y address start high byte
        {0x3802, (0 >> 8) & 0x07},
        //[7:0]=0 Y address start low byte
        {0x3803, 0 & 0xFF},

        //[3:0] X address end high byte
        {0x3804, (2623 >> 8) & 0x0F},
        //[7:0] X address end low byte
        {0x3805, 2623 & 0xFF},
        //[2:0] Y address end high byte
        {0x3806, (1951 >> 8) & 0x07},
        //[7:0] Y address end low byte
        {0x3807, 1951 & 0xFF},

        //[3:0]=0 timing hoffset high byte
        {0x3810, (16 >> 8) & 0x0F},
        //[7:0]=0 timing hoffset low byte
        {0x3811, 16 & 0xFF},
        //[2:0]=0 timing voffset high byte
        {0x3812, (4 >> 8) & 0x07},
        //[7:0]=0 timing voffset low byte
        {0x3813, 4 & 0xFF},

        //[3:0] Output horizontal width high byte
        {0x3808, (MIPI_CSI_IMAGE_HSIZE >> 8) & 0x0F},
        //[7:0] Output horizontal width low byte
        {0x3809, MIPI_CSI_IMAGE_HSIZE & 0xFF},
        //[2:0] Output vertical height high byte
        {0x380a, (MIPI_CSI_IMAGE_VSIZE >> 8) & 0x7F},
        //[7:0] Output vertical height low byte
        {0x380b, MIPI_CSI_IMAGE_VSIZE & 0xFF},

        //HTS line exposure time in # of pixels Tline=HTS/sclk
        {0x380c, (2844 >> 8) & 0x1F},
        {0x380d, 2844 & 0xFF},
        //VTS frame exposure time in # lines
        {0x380e, (1968 >> 8) & 0xFF},
        {0x380f, 1968 & 0xFF},

        //[7:4]=0x1 horizontal odd subsample increment, [3:0]=0x1 horizontal even subsample increment
        {0x3814, 0x11},
        //[7:4]=0x1 vertical odd subsample increment, [3:0]=0x1 vertical even subsample increment
        {0x3815, 0x11},

        //[2]=0 ISP mirror, [1]=0 sensor mirror, [0]=1 horizontal binning
        {0x3821, 0x00},

        //little MIPI shit: global timing unit, period of PCLK in ns * 2(depends on # of lanes)
        {0x4837, (1000000000 / MIPI_IDI_CLOCK_RATE) * 2}, // 1/40M*2

        //Undocumented anti-green settings
        {0x3618, 0x04}, // Removes vertical lines appearing under bright light
        {0x3612, 0x2b},
        // {0x3708, 0x64},
        {0x3709, 0x12},
        {0x370c, 0x00},

        // FORMAT CONTROL 00 0xF8 RW
        //           0x0: RAW
        //                Bit[3:0]: Output sequence
        //                          0x0: BGBG... / GRGR...
        //                          0x1: GBGB... / RGRG...
        //                          0x2: GRGR... / BGBG...
        //                          0x3: RGRG... / GBGB...
        //                          0x4~0xF: Not allowed
        //           0x1: Y8
        //                Bit[3:0]: Does not matter
        //           0x2: YUV444/RGB888 (not available for full resolution)
        //                Bit[3:0]: Output sequence
        //                          0x0: YUVYUV..., or GBRGBR...
        //                          0x1: YVUYVU..., or GRBGRB...
        //                          0x2: UYVUYV..., or BGRBGR...
        //                          0x3: VYUVYU..., or RGBRGB...
        //                          0x4: UVYUVY..., or BRGBRG...
        //                          0x5: VUYVUY..., or RBGRBG...
        //                          0x6~0xE: Not allowed
        //                          0xF: UYVUYV..., or BGRBGR...
        //           0x3: YUV422
        //                Bit[3:0]: Output sequence
        //                          0x0: YUYV...
        //                          0x1: YVYU...
        //                          0x2: UYVY...
        //                          0x3: VYUY...
        //                          0x4~0xE: Not allowed
        //                          0xF: UYVY...
        //           0x4: YUV420
        //                Bit[3:0]: Output sequence
        //                          0x0: YYYY... / YUYV...
        //                          0x1: YYYY... / YVYU...
        //                          0x2: YYYY... / UYVY...
        //                          0x3: YYYY... / VYUY...
        //                          0x4: YUYV... / YYYY...
        //                          0x5: YVYU... / YYYY...
        //                          0x6: UYVY... / YYYY...
        //                          0x7: VYUY... / YYYY...
        //                          0x8~0xE: Not allowed
        //                          0xF: YYYY... / UYVY...
        //           0x5: YUV420 (for MIPI only)
        //                Bit[3:0]: Output sequence
        //                          0x0~0xD: Not allowed
        //                          0xE: VYYVYY... / UYYUYY...
        //                          0xF: UYYUYY... / VYYVYY...
        //           0x6: RGB565
        //                Bit[3:0]: Output sequence
        //                          0x0: {b[4:0],g[5:3]}, {g[2:0],r[4:0]}
        //                          0x1: {r[4:0],g[5:3]}, {g[2:0],b[4:0]}
        //                          0x2: {g[4:0],r[5:3]}, {r[2:0],b[4:0]}
        //                          0x3: {b[4:0],r[5:3]}, {r[2:0],g[4:0]}
        //                          0x4: {g[4:0],b[5:3]}, {b[2:0],r[4:0]}
        //                          0x5: {r[4:0],b[5:3]}, {b[2:0],g[4:0]}
        //                          0x6~0xE: Not allowed
        //                          0xF: {g[2:0],b[4:0]}, {r[4:0],g[5:3]}
        //           0x7: RGB555 format 1
        //                Bit[3:0]: Output sequence
        //                          0x0: {b[4:0],g[4:2]},  {g[1:0],1'b0,r[4:0]}
        //                          0x1: {r[4:0],g[4:2]}, {g[1:0],1'b0,b[4:0]}
        //                          0x2: {g[4:0],r[4:2]}, {r[1:0],1'b0,b[4:0]}
        //                          0x3: {b[4:0],r[4:2]}, {r[1:0],1'b0,g[4:0]}
        //                          0x4: {r[4:0],b[4:2]}, {b[1:0],1'b0,g[4:0]}
        //                          0x5: {g[4:0],b[4:2]}, {b[1:0],1'b0,r[4:0]}
        //                          0x6~0xE: Not allowed
        //                          0xF: {g[1:0],1'b0,b[4:0]}, {r[4:0],g[4:2]}
        //           0x8: RGB555 format 2
        //                Bit[3:0]: Output sequence
        //                          0x0: {1'b0,b[4:0],g[4:3]}, {g[2:0],r[4:0]}
        //                          0x1: {1'b0,r[4:0],g[4:2]}, {g[2:0],b[4:0]}
        //                          0x2: {1'b0,g[4:0],r[4:2]}, {r[2:0],b[4:0]}
        //                          0x3: {1'b0,b[4:0],r[4:2]}, {r[2:0],g[4:0]}
        //                          0x4: {1'b0,r[4:0],b[4:2]}, {b[2:0],g[4:0]}
        //                          0x5: {1'b0,g[4:0],b[4:2]}, {b[2:0],r[4:0]}
        //                          0x6: {b[4:0],1'b0,g[4:3]}, {g[2:0],r[4:0]}
        //                          0x7: {r[4:0],1'b0,g[4:2]}, {g[2:0],b[4:0]}
        //                          0x8: {g[4:0],1'b0,r[4:2]}, {r[2:0],b[4:0]}
        //                          0x9: {b[4:0],1'b0,r[4:2]}, {r[2:0],g[4:0]}
        //                          0xA: {r[4:0],1'b0,b[4:2]}, {b[2:0],g[4:0]}
        //                          0xB: {g[4:0],1'b0,b[4:2]}, {b[2:0],r[4:0]}
        //                          0xC~0xF: Not allowed
        //           0x9: RGB444 format 1
        //                Bit[3:0]: Output sequence
        //                          0x0: {1'b0,b[3:0],2'h0,g[3]}, {g[2:0],1'b0,r[3:0]}
        //                          0x1: {1'b0,r[3:0],2'h0,g[3]}, {g[2:0],1'b0,b[3:0]}
        //                          0x2: {1'b0,g[3:0],2'h0,r[3]}, {r[2:0],1'b0,b[3:0]}
        //                          0x3: {1'b0,b[3:0],2'h0,r[3]}, {r[2:0],1'b0,g[3:0]}
        //                          0x4: {1'b0,r[3:0],2'h0,b[3]}, {b[2:0],1'b0,g[3:0]}
        //                          0x5: {1'b0,g[3:0],2'h0,b[3]}, {b[2:0],1'h0,r[3:0]}
        //                          0x6: {b[3:0],1'b0,g[3:1]}, {g[0],2'h0,r[3:0],1'b0}
        //                          0x7: {r[3:0],1'b0,g[3:1]}, {g[0],2'h0,b[3:0],1'b0}
        //                          0x8: {g[3:0],1'b0,r[3:1]}, {r[0],2'h0,b[3:0],1'b0}
        //                          0x9: {b[3:0],1'b0,r[3:1]}, {r[0],2'h0,g[3:0],1'b0}
        //                          0xA: {r[3:0],1'b0,b[3:1]}, {b[0],2'h0,g[3:0],1'b0}
        //                          0xB: {g[3:0],1'b0,b[3:1]}, {b[0],2'h0,r[3:0],1'b0}
        //                          0xC~0xE: Not allowed
        //                          0xF: {g[0],2'h2,b[3:0],1'b1}, {r[3:0],1'b1,g[3:1]}
        //           0xA: RGB444 format 2
        //                Bit[3:0]: Output sequence
        //                          0x0: {4'b0,b[3:0]}, {g[3:0],r[3:0]}
        //                          0x1: {4'b0,r[3:0]}, {g[3:0],b[3:0]}
        //                          0x2: {4'b0,b[3:0]}, {r[3:0],g[3:0]}
        //                          0x3: {4'b0,r[3:0]}, {b[3:0],g[3:0]}
        //                          0x4: {4'b0,g[3:0]}, {b[3:0],r[3:0]}
        //                          0x5: {4'b0,g[3:0]}, {r[3:0],b[3:0]}
        //                          0x6: {b[3:0],g[3:0],2'h0}, {r[3:0],b[3:0],2'h0,g[3:0],r[3:0],2'h0}
        //                          0x7: {r[3:0],g[3:0],2'h0}, {b[3:0],r[3:0],2'h0,g[3:0],b[3:0],2'h0}
        //                          0x8: {b[3:0],r[3:0],2'h0}, {g[3:0],b[3:0],2'h0,r[3:0],g[3:0],2'h0}
        //                          0x9: {r[3:0],b[3:0],2'h0}, {g[3:0],r[3:0],2'h0,b[3:0],g[3:0],2'h0}
        //                          0xA: {g[3:0],b[3:0],2'h0}, {r[3:0],g[3:0],2'h0,b[3:0],r[3:0],2'h0}
        //                          0xB: {g[3:0],r[3:0],2'h0}, {b[3:0],g[3:0],2'h0,r[3:0],b[3:0],2'h0}
        //                          0xC~0xF: Not allowed
        //           0xB~0xE: Not allowed
        //           0xF: Bypass formatter module (not recommended)
        //                Bit[3:0]: Output format
        //                          0x8: Raw
        //                          0x9: YUV422
        //                          0xA: YUV444
        //                          0xE: VYYVYY.../UYYUYY
        //                          0xF: UYYUYY.../VYYVYY

#if (TEST_CSI_COLOR_MODE == MIPI_CSI_RGB565_MODE)
        {0x4300, 0x6F},
        //[2:0]=0x0 Format select YUV422
        {0x501f, 0x01},
        {0x5001 ,0x23},
#elif (TEST_CSI_COLOR_MODE == MIPI_CSI_RGB888_MODE)
        {0x4300, 0x25},
        //[2:0]=0x0 Format select YUV422
        {0x501f, 0x00},
        {0x5001 ,0x25},
#elif (TEST_CSI_COLOR_MODE == MIPI_CSI_YUV422_MODE)
        {0x4300, 0x30},
        //[2:0]=0x0 Format select YUV422
        {0x501f, 0x00},
        {0x5001 ,0x23},
#elif (TEST_CSI_COLOR_MODE == MIPI_CSI_YUV420_MODE)
        {0x4300, 0x5F},
        //[2:0]=0x0 Format select YUV422
        {0x501f, 0x00},
        {0x5001 ,0x23},
#elif (TEST_CSI_COLOR_MODE >= MIPI_CSI_RAW8_MODE)
        //[7:4]=0x0 Formatter RAW, [3:0]=0x0 BGBG/GRGR
        {0x4300, 0x00},
        //[2:0]=0x3 Format select ISP RAW (DPC)
        {0x501f, 0x03},
        {0x5001 ,0x03},
#endif
#if (TEST_CSI_PATTERN)
        {0x4741, 0x05},
        {0x503D, 0x80},
#endif
        {SEQUENCE_END, 0x00}
};

struct reginfo cfg_advanced_awb[] =
{
        // Enable Advanced AWB
        {0x3406 ,0x00},
        {0x5192 ,0x04},
        {0x5191 ,0xf8},
        {0x518d ,0x26},
        {0x518f ,0x42},
        {0x518e ,0x2b},
        {0x5190 ,0x42},
        {0x518b ,0xd0},
        {0x518c ,0xbd},
        {0x5187 ,0x18},
        {0x5188 ,0x18},
        {0x5189 ,0x56},
        {0x518a ,0x5c},
        {0x5186 ,0x1c},
        {0x5181 ,0x50},
        {0x5184 ,0x20},
        {0x5182 ,0x11},
        {0x5183 ,0x00},

        {SEQUENCE_END, 0x00}
};

static void delay_us(uint32_t t)
{
#if TEST_CSI_FPGA
    for (uint32_t tu = 0; tu < t; tu++);
#else
    ets_delay_us(t);
#endif
}

int ov5640_read(uint16_t addr,uint8_t *read_buf)
{
    return sccb_read_reg16(OV5640_ADDR, addr, 1, read_buf);;
}

int ov5640_write(uint16_t addr,uint8_t data)
{

    return sccb_write_reg16(OV5640_ADDR, addr, 1, data);
}

/* write a array of registers  */
static void sensor_write_array( struct reginfo *regarray)
{
    int i = 0;
    while (regarray[i].reg != SEQUENCE_END) {
        ov5640_write(regarray[i].reg,regarray[i].val);
        i++;
    }

}

void OV5640_BrightnessConfig(int8_t Brightness)
{ 
    ov5640_write(0x3212, 0x03); // start group 3
  
    ov5640_write(0x5587, (Brightness<<4)&0xF0);

    if(Brightness >= 0)
    {
        ov5640_write(0x5588, 0x01);
    }
    else
    {
        ov5640_write(0x5588, 0x09);
    }
      
    ov5640_write(0x3212, 0x13); // end group 3
    ov5640_write(0x3212, 0xa3); // launch group 3
}


const static uint8_t OV5640_Saturation_reg[][6]=
{
    {0X0C,0x30,0X3D,0X3E,0X3D,0X01},//-3
    {0X10,0x3D,0X4D,0X4E,0X4D,0X01},//-2
    {0X15,0x52,0X66,0X68,0X66,0X02},//-1
    {0X1A,0x66,0X80,0X82,0X80,0X02},//0
    {0X1F,0x7A,0X9A,0X9C,0X9A,0X02},//+1
    {0X24,0x8F,0XB3,0XB6,0XB3,0X03},//+2
    {0X2B,0xAB,0XD6,0XDA,0XD6,0X04},//+3
};
/**
  * @brief  设置饱和度
  * @param  sat:饱和度,参数范围[-3 ~ +3]             
  * @retval 无
  */
void OV5640_Color_Saturation(int8_t sat)
{
    uint8_t i;
  
    ov5640_write(0x3212, 0x03); // start group 3
    ov5640_write(0x5381, 0x1c);
    ov5640_write(0x5382, 0x5a);
    ov5640_write(0x5383, 0x06);
  
    for(i=0;i<6;i++)
    {
        ov5640_write(0x5384+i, OV5640_Saturation_reg[sat+3][i]);
    }
  
    ov5640_write(0x538b, 0x98);
    ov5640_write(0x538a, 0x01);
    ov5640_write(0x3212, 0x13); // end group 3
    ov5640_write(0x3212, 0xa3); // launch group 3
  
}         

const static uint8_t OV5640_Contrast_reg[][2]=
{
 {0x2C,0x1C},
 {0x28,0x18},
 {0x24,0x10},
 {0x20,0x00},
 {0x1C,0x1C},
 {0x18,0x18},
 {0x14,0x14},
};

/**
  * @brief  设置对比度
    * @param  cnst:对比度，参数范围[-3~+3]
  * @retval 无
  */
void OV5640_ContrastConfig(int8_t cnst)
{ 
    ov5640_write(0x3212, 0x03); // start group 3
    ov5640_write(0x5586, OV5640_Contrast_reg[cnst+3][0]);
    ov5640_write(0x5585, OV5640_Contrast_reg[cnst+3][1]);
    ov5640_write(0x3212, 0x13); // end group 3
    ov5640_write(0x3212, 0xa3); // launch group 3
}

//曝光补偿设置参数表
const static uint8_t OV5640_Exposure_reg[][6]=
{
    {0x10,0x08,0x10,0x08,0x20,0x10},//-3
    {0x20,0x18,0x41,0x20,0x18,0x10},//-2
    {0x30,0x28,0x61,0x30,0x28,0x10},//-1
    {0x38,0x30,0x61,0x38,0x30,0x10},//0
    {0x40,0x38,0x71,0x40,0x38,0x10},//+1
    {0x50,0x48,0x90,0x50,0x48,0x20},//+2 
    {0x60,0x58,0xa0,0x60,0x58,0x20},//+3  
};

/**
  * @brief  设置曝光补偿
  * @param  ev:曝光补偿等级，参数范围[-3 ~ +3]               
  * @retval 无
  */
void OV5640_Exposure(int8_t ev)
{
    ov5640_write(0x3212, 0x03); // start group 3

    ov5640_write(0x3a0f, OV5640_Exposure_reg[ev+3][0]);
    ov5640_write(0x3a10, OV5640_Exposure_reg[ev+3][1]);
    ov5640_write(0x3a11, OV5640_Exposure_reg[ev+3][2]);
    ov5640_write(0x3a1b, OV5640_Exposure_reg[ev+3][3]);
    ov5640_write(0x3a1e, OV5640_Exposure_reg[ev+3][4]);
    ov5640_write(0x3a1f, OV5640_Exposure_reg[ev+3][5]);
  
    ov5640_write(0x3212, 0x13); // end group 3
    ov5640_write(0x3212, 0xa3); // launch group 3

}

//环境光模式参数表
const static uint8_t OV5640_LightMode_reg[][7]=
{
    {0x04,0X00,0X04,0X00,0X04,0X00,0X00},//Auto,自动
    {0x06,0X1C,0X04,0X00,0X04,0XF3,0X01},//Sunny,日光
    {0x05,0X48,0X04,0X00,0X07,0XCF,0X01},//Office,办公室
    {0x06,0X48,0X04,0X00,0X04,0XD3,0X01},//Cloudy,阴天
    {0x04,0X10,0X04,0X00,0X08,0X40,0X01},//Home,室内
};
/**
  * @brief  配置光线模式
  * @param  参数用于选择光线模式
  *         0x00 Auto     自动
  *         0x01 Sunny    日光
  *         0x02 Office     办公室
  *         0x03 Cloudy     阴天
  *         0x04 Home           室内

  * @retval None
  */
void OV5640_LightMode(uint8_t mode)
{
  ov5640_write(0x3212, 0x03); // start group 3
  
    ov5640_write(0x3406, OV5640_LightMode_reg[mode][0]);
    ov5640_write(0x3400, OV5640_LightMode_reg[mode][1]);
    ov5640_write(0x3401, OV5640_LightMode_reg[mode][2]);
    ov5640_write(0x3402, OV5640_LightMode_reg[mode][3]);
    ov5640_write(0x3403, OV5640_LightMode_reg[mode][4]);
    ov5640_write(0x3404, OV5640_LightMode_reg[mode][5]);
    ov5640_write(0x3405, OV5640_LightMode_reg[mode][6]);
  
    ov5640_write(0x3212, 0x13); // end group 3
    ov5640_write(0x3212, 0xa3); // lanuch group 3
}

//特效设置参数表
const static uint8_t OV5640_Effect_reg[][4]=
{
    {0X06,0x40,0X10,0X08},//正常
    {0X1E,0xA0,0X40,0X08},//冷色
    {0X1E,0x80,0XC0,0X08},//暖色
    {0X1E,0x80,0X80,0X08},//黑白
    {0X1E,0x40,0XA0,0X08},//泛黄
    {0X40,0x40,0X10,0X08},//反色
    {0X1E,0x60,0X60,0X08},//偏绿
    {0X1E,0xF0,0XF0,0X08},//过曝
    {0X06,0x40,0X10,0X09},//正负片叠加
};
/**
  * @brief  特殊效果
  * @param  参数用于选择光线模式
    *   0x01  正常
    *   0x02  冷色
    *   0x03  暖色
    *   0x04  黑白
    *   0x05  泛黄
    *   0x06  反色
    *   0x07  偏绿
    *   0x08  过曝
    *   0x09  正负片叠加

  * @retval None
  */
void OV5640_SpecialEffects(uint8_t mode)
{
    ov5640_write(0x3212, 0x03); // start group 3
  
    ov5640_write(0x5580, OV5640_Effect_reg[mode][0]);
    ov5640_write(0x5583, OV5640_Effect_reg[mode][1]); // sat U
    ov5640_write(0x5584, OV5640_Effect_reg[mode][2]); // sat V
    ov5640_write(0x5003, OV5640_Effect_reg[mode][3]);
  
    ov5640_write(0x3212, 0x13); // end group 3
    ov5640_write(0x3212, 0xa3); // launch group 3
}

int ov5640_init()
{
    uint8_t sensor_id[2] ;

    // OV5640_ADDR = sccb_prob();
    OV5640_ADDR = 0x3C;

    printf("ADDR: 0x%x\n", OV5640_ADDR);
    ov5640_read( 0x300A, &sensor_id[0]);
    ov5640_read( 0x300B, &sensor_id[1]);

    printf("id, %x %x\r\n", sensor_id[0], sensor_id[1]);

    if (sensor_id[0] != 0x56 || sensor_id[1] != 0x40)
    {
        printf("Not ov5640 id, %x %x\r\n", sensor_id[0], sensor_id[1]);
        return 0 ;
    }
    else
    {
        printf("Got ov5640 id, %x %x\r\n", sensor_id[0], sensor_id[1]);
    }

    //[1]=0 System input clock from pad; Default read = 0x11
    ov5640_write(0x3103,0x11);
    //[7]=1 Software reset; [6]=0 Software power down; Default=0x02
    ov5640_write(0x3008, 0x82);
    // Ensure streaming off to make clock lane go into LP-11 state.
    ov5640_write(0x4800, 0x05);  
    delay_us(100000);

    sensor_write_array(sensor_init_data);
    delay_us(100000);
    //[7]=0 Software reset; [6]=1 Software power down; Default=0x02
    ov5640_write(0x3008, 0x42);

#if TEST_CSI_720P
    sensor_write_array(cfg_720p_preview);
#else
    sensor_write_array(cfg_1080p_preview);
#endif
    sensor_write_array(cfg_advanced_awb);

    // OV5640_BrightnessConfig(10);

    // OV5640_Exposure(3);

    // OV5640_LightMode(0);

    //[7]=0 Software reset; [6]=0 Software power down; Default=0x02
    ov5640_write(0x3008, 0x02);
    return 0;
}
