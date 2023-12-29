#include "imx219.h"
#include "csi.h"
#include "sccb.h"

static uint8_t IMX_ADDR;

// 0 = 3280x2464
// 1 = 1920x1080
// 2 = 1640x1232
// 3 = 640x480
#define RESOLUTION (2)
// 0 = RAW8
// 1 = RAW10
#define FORMAT     ((TEST_CSI_COLOR_MODE==MIPI_CSI_RAW10_MODE) ? 0x1 : 0x0)

#define FRAME_LENGTH (RESOLUTION == 0x0 ? 3526 : (3526 / 2))

#if (RESOLUTION == 0)
#define XADDR_START (0)
#define XADDR_END   (3279)
#define YADDR_START (0)
#define YADDR_END   (2463)
#define VBIN_MODE   (0)
#define HBIN_MODE   (0)
#elif (RESOLUTION == 1)
#define XADDR_START (680)
#define XADDR_END   (2087)
#define YADDR_START (692)
#define YADDR_END   (1771)
#define VBIN_MODE   (0)
#define HBIN_MODE   (0)
#elif (RESOLUTION == 2)
#define XADDR_START (0)
#define XADDR_END   (3279)
#define YADDR_START (0)
#define YADDR_END   (2463)
#define VBIN_MODE   (1)
#define HBIN_MODE   (1)
#else
#define XADDR_START (1000)
#define XADDR_END   (2279)
#define YADDR_START (752)
#define YADDR_END   (1711)
#define VBIN_MODE   (3)
#define HBIN_MODE   (3)
#endif

typedef struct
{
    uint16_t reg;
    uint8_t val;
} reginfo_t;

#define SEQUENCE_END	     0xFFFF

static reginfo_t imx219_init_data[] =
{    
	{0x30eb, 0x05}, // Manufacturer access command sequence (See Section 3-4)
	{0x30eb, 0x0c},
	{0x300a, 0xff},
	{0x300b, 0xff},
	{0x30eb, 0x05},
	{0x30eb, 0x09},
	{0x0114, 0x01}, // CSI Lane Count (2)
	{0x0128, 0x00}, // MIPI Global timing (auto)
	{0x012a, 0x18}, // External Clock Frequency MSB (24MHz)
	{0x012b, 0x00}, // External Clock Frequency LSB
	{0x0160, (FRAME_LENGTH >> 8) & 0xFF}, // Frame length MSB (15 FPS for full-frame, 30 FPS for other resolutions)
	{0x0161, FRAME_LENGTH & 0xFF}, // Frame IMX219_VTS_15FPSlength LSB
	{0x0162, (3448 >> 8) & 0xFF}, // Pixel clocks per line MSB (3448)
	{0x0163, (3448) & 0xFF}, // Pixel clocks per line LSB
	{0x0164, (XADDR_START >> 8) & 0xFF}, // X-address start MSB
	{0x0165, (XADDR_START) & 0xFF}, // X-address start LSB
	{0x0166, (XADDR_END >> 8) & 0xFF}, // X-address end MSB
	{0x0167, (XADDR_END) & 0xFF}, // X-address end LSB
	{0x0168, (YADDR_START >> 8) & 0xFF}, // Y-address start MSB
	{0x0169, (YADDR_START) & 0xFF}, // Y-address start LSB
	{0x016a, (YADDR_END >> 8) & 0xFF}, // Y-address end MSB
	{0x016b, (YADDR_END) & 0xFF}, // Y-address end LSB

	{0x016c, (MIPI_CSI_IMAGE_HSIZE >> 8) & 0x0F}, // X-output size MSB
	{0x016d, MIPI_CSI_IMAGE_HSIZE & 0xFF}, // X-output size LSB
	{0x016e, (MIPI_CSI_IMAGE_VSIZE >> 8) & 0x7F}, // Y-output size MSB
	{0x016f, MIPI_CSI_IMAGE_VSIZE & 0xFF}, // Y-output size LSB
	{0x0170, 0x01}, // X odd increment
	{0x0171, 0x01}, // Y odd increment
	{0x0172, 0x03},
	{0x0174, VBIN_MODE}, // Vertical binning mode
	{0x0175, HBIN_MODE}, // Horizontal binning mode
	{0x018c, FORMAT ? 0x0a : 0x08}, // CSI data format MSB
	{0x018d, FORMAT ? 0x0a : 0x08}, // CSI data format LSB
	{0x0301, FORMAT ? 0x05 : 0x04}, // Video timing pixel clock divider (/5 for 10-bit, /4 for 8-bit)
	{0x0303, 0x01}, // Video timing system clock divider (always /1)
	{0x0304, 0x03}, // External (pre-PLL) clock divider for video timing (3 for 24MHz to 27MHz)
	{0x0305, 0x03}, // External (pre-PLL) clock divider for output (3 for 24MHz to 27MHz)
	{0x0306, 0x00}, // PLL video timing system multiplier MSB
	{0x0307, (MIPI_IDI_CLOCK_RATE / 2000000)}, // PLL video timing system multiplier LSB
	{0x0309, FORMAT ? 0x0a : 0x08}, // Output pixel clock divider (/10 for 10-bit, /8 for 8-bit)
	{0x030b, 0x01}, // Output sytem clock divider (always /2)
	{0x030c, 0x00}, // PLL output system clock multiplier MSB
	{0x030d, (MIPI_IDI_CLOCK_RATE / 1000000)}, // PLL output system clock multiplier LSB (DDR clock, as compared to 0x0307)
	{0x0601, 0x00}, // Test pattern mode
	{0x0624, (MIPI_CSI_IMAGE_HSIZE >> 8) & 0x0F}, // Test pattern window width MSB
	{0x0625, MIPI_CSI_IMAGE_HSIZE & 0xFF}, // Test pattern window width LSB
	{0x0626, (MIPI_CSI_IMAGE_VSIZE >> 8) & 0x7F}, // Test pattern window height MSB
	{0x0627, MIPI_CSI_IMAGE_VSIZE & 0xFF}, // Test pattern window height LSB
	{0x455e, 0x00}, // CMOS Image Sensor Tuning for all below
	{0x471e, 0x4b},
	{0x4767, 0x0f},
	{0x4750, 0x14},
	{0x4540, 0x00},
	{0x47b4, 0x14},
	{0x4713, 0x30},
	{0x478b, 0x10},
	{0x478f, 0x10},
	{0x4793, 0x10},
	{0x4797, 0x0e},
	{0x479b, 0x0e},
	{0x0100, 0x01}, // Start streaming
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

static int imx_read_8bit(uint16_t addr, uint8_t *data)
{
    return sccb_read_reg16(IMX_ADDR, addr, 1, data);
}

static int imx_write_8bit(uint16_t addr, uint8_t data)
{

    return sccb_write_reg16(IMX_ADDR, addr, 1, data);
}

static int imx_read_16bit(uint16_t addr, uint16_t *data)
{
	int ret = -1;
	uint8_t buf[2];
	ret = sccb_read_reg16(IMX_ADDR, addr, 2, &buf[0], &buf[1]);
	data[0] = (buf[0] << 8) | (buf[1]);
    return ret;
}

static int imx_write_16bit(uint16_t addr, uint16_t data)
{
	
    return sccb_write_reg16(IMX_ADDR, addr, 2, (data >> 8) & 0xFF, (data) & 0xFF);
}

/* write a array of registers  */
static void imx_write_array( reginfo_t *regarray)
{
    int i = 0;
    while (regarray[i].reg != SEQUENCE_END) {
        imx_write_8bit(regarray[i].reg, regarray[i].val);
        i++;
    }
}

int imx219_init()
{
    uint8_t sensor_id[2] ;

    IMX_ADDR = 0x10;

    printf("ADDR: 0x%x\n", IMX_ADDR);
    imx_read_8bit( 0x0000, &sensor_id[0]);
    imx_read_8bit( 0x0001, &sensor_id[1]);

    printf("id, %x %x\r\n", sensor_id[0], sensor_id[1]);

	imx_write_8bit(0x0103, 0x1);
	delay_us(10000);
	imx_write_8bit(0x0103, 0x0);
	delay_us(10000);

    imx_write_8bit(0x0100, 0x0); // standby
    delay_us(10000);

    imx_write_array(imx219_init_data);
    delay_us(10000);

	imx_write_8bit(IMX219_REG_ANALOG_GAIN, 0);
	imx_write_16bit(IMX219_REG_EXPOSURE, 1600);
	imx_write_16bit(IMX219_REG_DIGITAL_GAIN, 4000);
    return 0;
}
