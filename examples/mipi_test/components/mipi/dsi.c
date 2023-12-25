#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "dsi.h"

uint32_t DPI_HSA               = MIPI_DSI_IMAGE_HSYNC ;
uint32_t DPI_HBP               = MIPI_DSI_IMAGE_HBP   ;
uint32_t DPI_HFP               = MIPI_DSI_IMAGE_HFP   ;
uint32_t DPI_HACT              = MIPI_DSI_IMAGE_HSIZE ;

uint32_t DPI_VSA               = MIPI_DSI_IMAGE_VSYNC ;
uint32_t DPI_VBP               = MIPI_DSI_IMAGE_VBP   ;
uint32_t DPI_VFP               = MIPI_DSI_IMAGE_VFP   ;
uint32_t DPI_VACT              = MIPI_DSI_IMAGE_VSIZE ;

static void delay_us(uint32_t t)
{
#if TEST_DSI_FPGA
    for (uint32_t tu = 0; tu < t; tu++);
#else
    ets_delay_us(t);
#endif
}

static void delay_ms(uint32_t t)
{
    delay_us(1000 * t);
}

/*
 * @brief  : dphy_write_control
 * @params : <testcode>
 *           <testwrite>
 * @ret    : int
 **/
static void mipi_dsi_dphy_write_control (uint32_t testcode, uint32_t testwrite)
{
    MIPI_DSI_HOST.phy_tst_ctrl1.val  = 0x00010000| testcode ;
    MIPI_DSI_HOST.phy_tst_ctrl0.val  = 0x00000002           ;
    MIPI_DSI_HOST.phy_tst_ctrl0.val  = 0x00000000           ;
    MIPI_DSI_HOST.phy_tst_ctrl1.val  = 0x00000000| testwrite;
    MIPI_DSI_HOST.phy_tst_ctrl0.val  = 0x00000002           ;
    MIPI_DSI_HOST.phy_tst_ctrl0.val  = 0x00000000           ;
}

/**
 * Write command header in the generic interface
 * (which also sends DCS commands) as a subset
 * @param vc of destination
 * @param packet_type (or type of DCS command)
 * @param ls_byte (if DCS, it is the DCS command)
 * @param ms_byte (only parameter of short DCS packet)
 * @return error code
 */
int mipi_dsih_hal_gen_packet_header(uint8_t vc, uint8_t packet_type, uint8_t ms_byte, uint8_t ls_byte)
{
    if (vc < 4)
    {
        MIPI_DSI_HOST.gen_hdr.val = (ms_byte << 16) | (ls_byte << 8) | ((vc << 6) | packet_type);
        return true;
    }
    return false;
}

/**
 * Get the FULL status of generic command fifo
 * @return 1 if fifo full
 */
int mipi_dsih_hal_gen_cmd_fifo_full()
{
    return MIPI_DSI_HOST.cmd_pkt_status.gen_cmd_full;
}

/**
 * Get the FULL status of generic write payload fifo
 * @return 1 if fifo full
 */
int mipi_dsih_hal_gen_write_fifo_full()
{
    return MIPI_DSI_HOST.cmd_pkt_status.gen_pld_w_full;
}

/**
 * Write the payload of the long packet commands
 * @param payload array of bytes of payload
 * @return error code
 */
int mipi_dsih_hal_gen_packet_payload(uint32_t payload)
{
    if (mipi_dsih_hal_gen_write_fifo_full())
        return false;

    MIPI_DSI_HOST.gen_pld_data.val = payload;
    return true;
}

/**
 * Enable command mode (Generic interface)
 * @param  pointer to structure holding the DSI Host core information
 * @param enable
 */
void mipi_dsih_hal_gen_cmd_mode_en(int enable)
{
    MIPI_DSI_HOST.mode_cfg.cmd_video_mode = enable;
}

/**
 * Enable Bus Turn-around request
 * @param dev pointer to structure holding the DSI Host core information
 * @param enable
 */
void mipi_dsih_hal_bta_en(int enable)
{
    MIPI_DSI_HOST.pckhdl_cfg.bta_en = enable;
}

/**
 * Configure the read back virtual channel for the generic interface
 * @param dev pointer to structure holding the DSI Host core information
 * @param vc to listen to on the line
 */
void mipi_dsih_hal_gen_rd_vc(uint8_t vc)
{
    MIPI_DSI_HOST.gen_vcid.gen_vcid_rx = vc;
}

/**
 * Get status of read command
 * @param dev pointer to structure holding the DSI Host core information
 * @return 1 if busy
 */
int mipi_dsih_hal_gen_rd_cmd_busy()
{
    return MIPI_DSI_HOST.cmd_pkt_status.gen_rd_cmd_busy;
}

/**
 * Get the EMPTY status of generic read payload fifo
 * @param dev pointer to structure holding the DSI Host core information
 * @return 1 if fifo empty
 */
int mipi_dsih_hal_gen_read_fifo_empty()
{
    return MIPI_DSI_HOST.cmd_pkt_status.gen_pld_r_empty;
}

/**
 * Write the payload of the long packet commands
 * @param dev pointer to structure holding the DSI Host core information
 * @param payload pointer to 32-bit array to hold read information
 * @return error code
 */
int  mipi_dsih_hal_gen_read_payload(uint32_t* payload)
{
    *payload = MIPI_DSI_HOST.gen_pld_data.val;
    return true;
}

#define DSIH_FIFO_ACTIVE_WAIT (200)
/**
 * Send a packet on the generic interface
 * @param vc destination virtual channel
 * @param data_type type of command, inserted in first byte of header
 * @param params byte array of command parameters
 * @param param_length length of the above array
 * @return error code
 * @note this function has an active delay to wait for the buffer to clear.
 * The delay is limited to:
 * (param_length / 4) x DSIH_FIFO_ACTIVE_WAIT x register access time
 * @note the controller restricts the sending of .
 * This function will not be able to send Null and Blanking packets due to
 *  controller restriction
 */
int mipi_dsih_gen_wr_packet(uint8_t vc, uint8_t data_type, uint8_t *params, uint16_t param_length)
{
    /* active delay iterator */
    int timeout = 0;
    /* iterators */
    int i = 0;
    int j = 0;
    /* holds padding bytes needed */
    int compliment_counter = 0;
    uint8_t *payload = 0;
    /* temporary variable to arrange bytes into words */
    uint32_t temp = 0;
    uint16_t word_count = 0;

    // printf("param_length: %d\n", param_length);

    // clean buffer
    for (word_count = 0; (!mipi_dsih_hal_gen_read_fifo_empty()); word_count += 4)
    {
        mipi_dsih_hal_gen_read_payload(temp);
    }

    if ((params == 0) && (param_length != 0))
    { /* pointer NULL */
        printf("null params\n");
        return false;
    }

    if (param_length > 200)
    {
        printf("param length too large\n");
        return false;
    }

    if (param_length > 2)
    {

        /*
         * LONG PACKET - write word count to header and the rest to payload
         */
        payload = params + (2 * sizeof(params[0]));
        word_count = (params[1] << 8) | params[0];

        // printf("word count: 0x%X\n", word_count);

        if (word_count > 200)
        {
            printf("word count too large\n");
            return false;
        }

        if ((param_length - 2) < word_count)
        {
            printf("sent > input payload. complemented with zeroes\n");
            compliment_counter = (param_length - 2) - word_count;
        }
        else if ((param_length - 2) > word_count)
        {
            printf("overflow - input > sent. payload truncated\n");
        }

        for (i = 0; i < (param_length - 2); i += j)
        {
            temp = 0;
            for (j = 0; (j < 4) && ((j + i) < (param_length - 2)); j++)
            {
                /* temp = (payload[i + 3] << 24) | (payload[i + 2] << 16) | (payload[i + 1] << 8) | payload[i]; */
                temp |= payload[i + j] << (j * 8);
            }
            /* check if payload Tx fifo is not full */
            for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++)
            {
                if (mipi_dsih_hal_gen_packet_payload(temp))
                    break;
                delay_us(500);
            }
            if (!(timeout < DSIH_FIFO_ACTIVE_WAIT))
            {
                printf("timeout! %u\n", timeout);
                return false;
            }
        }
        /*
         * If word count entered by the user more than actual
         * parameters received fill with zeroes
         *
         * A fail safe mechanism, otherwise controller will
         * want to send data from an empty buffer
         */
        for (i = 0; i < compliment_counter; i++)
        {

            /* check if payload Tx fifo is not full */
            for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++)
            {
                if (!mipi_dsih_hal_gen_packet_payload(0x00))
                    break;
            }
            if (!(timeout < DSIH_FIFO_ACTIVE_WAIT))
            {
                printf("timeout!!\n");
                return false;
            }
        }
    }
    for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++)
    {
        /* check if payload Tx fifo is not full */
        if (!mipi_dsih_hal_gen_cmd_fifo_full())
        {
            if (param_length == 0)
                return mipi_dsih_hal_gen_packet_header(vc, data_type,
                                                       0x0, 0x0);
            else if (param_length == 1)
                return mipi_dsih_hal_gen_packet_header(vc, data_type,
                                                       0x0, params[0]);
            else
                return mipi_dsih_hal_gen_packet_header(vc, data_type,
                                                       params[1], params[0]);
            break;
        }
    }
    return true;
}

/**
 * Send READ packet to peripheral using the generic interface
 * This will force command mode and stop video mode (because of BTA)
 * @param  pointer to structure holding the DSI Host core information
 * @param vc destination virtual channel
 * @param data_type generic command type
 * @param lsb_byte first command parameter
 * @param msb_byte second command parameter
 * @param bytes_to_read no of bytes to read (expected to arrive at buffer)
 * @param read_buffer pointer to 8-bit array to hold the read buffer words
 * return read_buffer_length
 * @note this function has an active delay to wait for the buffer to clear.
 * The delay is limited to 2 x DSIH_FIFO_ACTIVE_WAIT
 * (waiting for command buffer, and waiting for receiving)
 * @note this function will enable BTA
 */
int mipi_dsih_gen_rd_packet(uint8_t vc,
                                 uint8_t data_type, uint8_t msb_byte,
                                 uint8_t lsb_byte, uint8_t bytes_to_read,
                                 uint8_t *read_buffer)
{
    int timeout = 0;
    int counter = 0;
    int i = 0;
    int last_count = 0;
    uint32_t temp[1] = {0};

    if (bytes_to_read < 1)
        return false;

    if (read_buffer == NULL)
        return false;

    /* make sure command mode is on */
    mipi_dsih_hal_gen_cmd_mode_en(1);
    /* make sure receiving is enabled */
    mipi_dsih_hal_bta_en(1);
    /* listen to the same virtual channel as the one sent to */
    mipi_dsih_hal_gen_rd_vc(vc);

    // printf("data_type: 0x%X\n", data_type);
    // printf("msb_byte: 0x%X\n", msb_byte);
    // printf("lsb_byte: 0x%X\n", lsb_byte);
    // printf("bytes_to_read: %d\n", bytes_to_read);

    // clean buffer
    for (counter = 0; (!mipi_dsih_hal_gen_read_fifo_empty()); counter += 4)
    {
        mipi_dsih_hal_gen_read_payload(temp);
    }

    for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++)
    {

        /* check if payload Tx fifo is not full */
        if (!mipi_dsih_hal_gen_cmd_fifo_full())
        {

            mipi_dsih_hal_gen_packet_header(vc, data_type,
                                            msb_byte, lsb_byte);
            break;
        }
        delay_us(500);
    }
    if (!(timeout < DSIH_FIFO_ACTIVE_WAIT))
    {

        printf("TX READ command timed out\n");
        return false;
    }

    /* loop for the number of words to be read */
    for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++)
    {

        /* check if command transaction is done */
        if (!mipi_dsih_hal_gen_rd_cmd_busy())
        {
            delay_us(500);
            if (!mipi_dsih_hal_gen_read_fifo_empty())
            {

                for (counter = 0; (!mipi_dsih_hal_gen_read_fifo_empty()); counter += 4)
                {
                    mipi_dsih_hal_gen_read_payload(temp);
                    if (counter < bytes_to_read)
                    {

                        for (i = 0; i < 4; i++)
                        {
                            if ((counter + i) < bytes_to_read)
                            {
                                /* put 32 bit temp in 4 bytes of buffer passed by user*/
                                read_buffer[counter + i] = (uint8_t)(temp[0] >> (i * 8));
                                last_count = i + counter;
                            }
                            else
                            {
                                if ((uint8_t)(temp[0] >> (i * 8)) != 0x00)
                                    last_count = i + counter;
                            }
                        }
                    }
                    else
                    {
                        last_count = counter;
                        for (i = 0; i < 4; i++)
                        {
                            if ((uint8_t)(temp[0] >> (i * 8)) != 0x00)
                                last_count = i + counter;
                        }
                    }
                }
                return last_count + 1;
            }
            else
            {
                printf("RX buffer empty\n");
                return true;
            }
        }
        delay_us(500);
    }
    printf("RX command timed out\n");
    return false;
}

/*
 * @brief  : Initialization of MIPI-DSI Bridge.
 * @params : void
 * @ret    : int
 **/
static int mipi_dsi_host_phy_initialization(void)
{
    uint32_t stat;

    // When the null packets are enabled:
    // lanebyteclkperiod * vid_num_chunks (vid_pkt_size * bytes_per_pixel + 12 + vid_null_size) / number_of_lanes = pixels_per_line * dpipclkperiod
    // When the null packets are disabled:
    // lanebyteclkperiod * vid_num_chunks (vid_pkt_size * bytes_per_pixel + 6) / number_of_lanes = pixels_per_line * dpipclkperiod

    // uint32_t DPI_NUMBER_OF_CHUNKS  = 30; // Set 0 to transmit video line in one packet.
    // uint32_t DPI_NULL_PKT_SIZE     = 0; // Set 0 to disable null packet.
    // uint32_t DPI_PIXELS_PER_PACKET = 120 ;
    uint32_t DPI_NUMBER_OF_CHUNKS = 0; // Set 0 to transmit video line in one packet.
    uint32_t DPI_NULL_PKT_SIZE = 0;    // Set 0 to disable null packet.
    uint32_t DPI_PIXELS_PER_PACKET = DPI_HACT;
    uint32_t DPI_HSA_TIME = (int)DPI_HSA * MIPI_DPI_TIME_FACTOR;
    uint32_t DPI_HBP_TIME = (int)DPI_HBP * MIPI_DPI_TIME_FACTOR;
    uint32_t DPI_HLINE_TIME = (int)((DPI_HSA + DPI_HBP + DPI_HFP + DPI_HACT) * MIPI_DPI_TIME_FACTOR);
    uint32_t DPI_FPS         = (MIPI_DPI_CLOCK_RATE / ((MIPI_DSI_IMAGE_HSIZE + MIPI_DSI_IMAGE_HSYNC + MIPI_DSI_IMAGE_HBP + MIPI_DSI_IMAGE_HFP) * (MIPI_DSI_IMAGE_VSIZE + MIPI_DSI_IMAGE_VSYNC + MIPI_DSI_IMAGE_VBP + MIPI_DSI_IMAGE_VFP))); // fps

    printf("MIPI_DPI_TIME_FACTOR: %f, DPI_HSA_TIME: %d, DPI_HBP_TIME: %d, DPI_HLINE_TIME: %d, DPI_FPS: %d\n", MIPI_DPI_TIME_FACTOR, DPI_HSA_TIME, DPI_HBP_TIME, DPI_HLINE_TIME, DPI_FPS);

    MIPI_DSI_HOST.pwr_up.val = 0x0;
    // ---------------------------------------
    // Configure Display Pixel Interface
    // ---------------------------------------
    printf("[MIPI-HAL] Configure Display Pixel Interface.\n");
    MIPI_DSI_HOST.dpi_vcid.val = 0x00000000 |
                                 ((0 & 0x00000003) << 0); // bit[ 1: 0] VCID                  = 0 (DPI Virtual Channel ID)

    // ■ 0x0 (CC00): 16-bit configuration 1
    // ■ 0x1 (CC01): 16-bit configuration 2
    // ■ 0x2 (CC02): 16-bit configuration 3
    // ■ 0x3 (CC03): 18-bit configuration 1
    // ■ 0x4 (CC04): 18-bit configuration 2
    // ■ 0x5 (CC05): 24-bit
    // ■ 0x6 (CC06): 20-bit YCbCr 4:2:2 loosely packed /
    // Reserved for eDPI Command Mode
    // ■ 0x7 (CC07): 24-bit YCbCr 4:2:2 / Reserved for eDPI
    // Command Mode
    // ■ 0x8 (CC08): 16-bit YCbCr 4:2:2 / Reserved for eDPI
    // Command Mode
    // ■ 0x9 (CC09): 30-bit - DSC_ENC 10bit / Reserved for eDPI
    // Command Mode
    // ■ 0xa (CC10): 36-bit / Reserved for eDPI Command Mode
    // ■ 0xb (CC11): 12-bit YCbCr 4:2:0 / Reserved for eDPI
    // Command Mode
    // ■ 0xc (CC12): DSC24 compressed Data
    // ■ 0xd (CC13): Reserved
    // ■ 0xe (CC14): Reserved
    // ■ 0xf (CC15): Reserved
    uint8_t color_mode = 0;
    if (TEST_DSI_COLOR_MODE == MIPI_DSI_RGB565_MODE) {
        color_mode = 0;
    } else if (TEST_DSI_COLOR_MODE == MIPI_DSI_RGB666_MODE) {
        color_mode = 3;
    } else {
        color_mode = 5;
    }
    MIPI_DSI_HOST.dpi_color_coding.val = 0x00000000 |
                                         ((0 & 0x00000001) << 8) | // bit[    8] ENABLE18BPPLOOSELY    = 0 (0: Disable Loosely packed variant to 18-bit configurations)
                                         ((color_mode & 0x0000000f) << 0);  // bit[ 3: 0] COLORMODE             = 5 (CC05 24-bit)
    MIPI_DSI_HOST.dpi_cfg_pol.val = 0x00000000 |
                                    ((0 & 0x00000001) << 4) | // bit[    4] COLRMACTLOW           = 0 (Configures the color mode pin (dpicolorm) as active low.
                                    ((0 & 0x00000001) << 3) | // bit[    3] SHTDMACTLOW           = 0 (Configures the shutdown pin (dpishutdn) as active low.
                                    ((0 & 0x00000001) << 2) | // bit[    2] HSYNCACTLOW           = 0 (Configures the horizontal synchronism pin (dpihsync) as active low.
                                    ((0 & 0x00000001) << 1) | // bit[    1] VSYNCACTLOW           = 0 (Configures the vertical synchronism pin (dpivsync) as active low.
                                    ((0 & 0x00000001) << 0);  // bit[    0] DATENACTLOW           = 0 (Configures the data enable pin (dpidataen) as active low.

    // !!! ILI9881C escape mode 频率敏感, 需要精确时钟
    MIPI_DSI_HOST.clkmgr_cfg.val = 0x00000000 |
                                   (((2 - 1) & 0x000000ff) << 8) | // bit[15: 8] TO_CLK_DIVISION       = 1 (Division factor for the Time Out clock.
                                   (((MIPI_DSI_BYTE_RATE / 10000000 - 1) & 0x000000ff) << 0);  // bit[ 7: 0] TX_ESC_CLK_DIVISION   = 7 (Division factor for the TX Escape clock source (lanebyteclk).
    // ---------------------------------------
    // Configure Display Bus Interface
    // ---------------------------------------
    printf("[MIPI-HAL] Configure Display Bus Interface.\n");
#if TEST_DSI_FPGA
    MIPI_DSI_HOST.cmd_mode_cfg.val = 0x00000000 |
                                     ((0 & 0x00000001) << 24) | // bit[   24] MAX_READ_PACKET_SIZE  = 0 (Configures the maximum read packet size.
                                     ((0 & 0x00000001) << 19) | // bit[   19] DCS_LW_LP_ENA         = 0 (Configures the DCS long write packet.
                                     ((0 & 0x00000001) << 18) | // bit[   18] DSC_SR_0P_LP_ENA      = 0 (Configures the DCS short read packet with zero parameter.
                                     ((0 & 0x00000001) << 17) | // bit[   17] DSC_SW_1P_LP_ENA      = 0 (Configures the DCS short write packet with one parameter.
                                     ((0 & 0x00000001) << 16) | // bit[   16] DSC_SW_0P_LP_ENA      = 0 (Configures the DCS short write packet with zero parameter.
                                     ((0 & 0x00000001) << 14) | // bit[   14] GEN_LW_LP_ENA         = 0 (Configures the Generic long write packet.
                                     ((0 & 0x00000001) << 13) | // bit[   13] GEN_SR_2P_LP_ENA      = 0 (Configures the Generic short read packet with two parameters.
                                     ((0 & 0x00000001) << 12) | // bit[   12] GEN_SR_1P_LP_ENA      = 0 (Configures the Generic short read packet with one parameter.
                                     ((0 & 0x00000001) << 11) | // bit[   11] GEN_SR_0P_LP_ENA      = 0 (Configures the Generic short read packet with zero parameter.
                                     ((0 & 0x00000001) << 10) | // bit[   10] GEN_SW_2P_LP_ENA      = 0 (Configures the Generic short write packet with two parameters.
                                     ((0 & 0x00000001) << 9) |  // bit[    9] GEN_SW_1P_LP_ENA      = 0 (Configures the Generic short write packet with one parameter.
                                     ((0 & 0x00000001) << 8) |  // bit[    8] GEN_SW_0P_LP_ENA      = 0 (Configures the Generic short write packet with zero parameter.
                                     ((0 & 0x00000001) << 1) |  // bit[    1] ACK_REQUEST_ENABLE    = 0 (Tcknowledge request after each packet transmission.
                                     ((0 & 0x00000001) << 0);   // bit[    0] TEAR_FX_ENABLE        = 0 (Tearing effect acknowledge request.
#else
    // LPDT escape传输command
    MIPI_DSI_HOST.cmd_mode_cfg.val = 0x00000000 |
                                     ((15 & 0x00000001) << 24) | // bit[   24] MAX_READ_PACKET_SIZE  = 0 (Configures the maximum read packet size.
                                     ((1 & 0x00000001) << 19) | // bit[   19] DCS_LW_LP_ENA         = 0 (Configures the DCS long write packet.
                                     ((1 & 0x00000001) << 18) | // bit[   18] DSC_SR_0P_LP_ENA      = 0 (Configures the DCS short read packet with zero parameter.
                                     ((1 & 0x00000001) << 17) | // bit[   17] DSC_SW_1P_LP_ENA      = 0 (Configures the DCS short write packet with one parameter.
                                     ((1 & 0x00000001) << 16) | // bit[   16] DSC_SW_0P_LP_ENA      = 0 (Configures the DCS short write packet with zero parameter.
                                     ((1 & 0x00000001) << 14) | // bit[   14] GEN_LW_LP_ENA         = 0 (Configures the Generic long write packet.
                                     ((1 & 0x00000001) << 13) | // bit[   13] GEN_SR_2P_LP_ENA      = 0 (Configures the Generic short read packet with two parameters.
                                     ((1 & 0x00000001) << 12) | // bit[   12] GEN_SR_1P_LP_ENA      = 0 (Configures the Generic short read packet with one parameter.
                                     ((1 & 0x00000001) << 11) | // bit[   11] GEN_SR_0P_LP_ENA      = 0 (Configures the Generic short read packet with zero parameter.
                                     ((1 & 0x00000001) << 10) | // bit[   10] GEN_SW_2P_LP_ENA      = 0 (Configures the Generic short write packet with two parameters.
                                     ((1 & 0x00000001) << 9) |  // bit[    9] GEN_SW_1P_LP_ENA      = 0 (Configures the Generic short write packet with one parameter.
                                     ((1 & 0x00000001) << 8) |  // bit[    8] GEN_SW_0P_LP_ENA      = 0 (Configures the Generic short write packet with zero parameter.
                                     ((0 & 0x00000001) << 1) |  // bit[    1] ACK_REQUEST_ENABLE    = 0 (Tcknowledge request after each packet transmission.
                                     ((0 & 0x00000001) << 0);   // bit[    0] TEAR_FX_ENABLE        = 0 (Tearing effect acknowledge request.
#endif

    // ---------------------------------------
    // Configure Packet Handler
    // ---------------------------------------
    printf("[MIPI-HAL] Configure Packet Handler.\n");
    MIPI_DSI_HOST.pckhdl_cfg.val = 0x00000000 |
                                   ((1 & 0x00000001) << 4) | // bit[    4] CRC_ENABLE            = 1 (Enables the CRC reception and error reporting.
                                   ((1 & 0x00000001) << 3) | // bit[    3] ECC_ENABLE            = 1 (Enables the ECC reception, error correction, and reporting.
                                   ((0 & 0x00000001) << 2) | // bit[    2] BTA_ENABLE            = 0 (Enables the Bus Turn-Around (BTA) request.
                                   ((0 & 0x00000001) << 1) | // bit[    1] EOTPRECEPTION         = 0 (Enables the EOTp reception.
                                   ((1 & 0x00000001) << 0);  // bit[    0] EOTPGENERATION        = 1 (Enables the EOTp transmission.
    MIPI_DSI_HOST.gen_vcid.val = 0x00000000 |
                                 ((0 & 0x00000003) << 8) | // bit[ 9: 8] GENERIC_TEAR_VC_ID    = 3 (Generic virtual channel ID for tear Request by HW.
                                 ((0 & 0x00000003) << 0);  // bit[ 1: 0] GENERIC_VC_ID         = 3 (Generic virtual channel ID.
    MIPI_DSI_HOST.mode_cfg.val = 0x00000000 |
                                 ((1 & 0x00000001) << 0); // bit[    0] CMD_VIDEO_ENABLE      = 0 (Video command mode selection.
    MIPI_DSI_HOST.vid_mode_cfg.val = 0x00000000 |
                                     ((0 & 0x00000001) << 24) | // bit[   24] VPG_ORIENTATION       = 1 (Video Pattern Generator Orientation - 0=Vertical / 1=Horizontal
                                     ((0 & 0x00000001) << 20) | // bit[   20] VPG_MODE              = 0 (Video Pattern Generator Mode        - 0=ColorBAr / 1=BER Testing
                                     ((0 & 0x00000001) << 16) | // bit[   16] VPG_EN                = 0 (Video Pattern Generator Activation
                                     ((1 & 0x00000001) << 15) | // bit[   15] LPCMDEN               = 0 (Enables Low Power command transmission.
                                     ((1 & 0x00000001) << 13) | // bit[   13] LP_HFP_EN             = 1 (Enables Low Power HFP.
                                     ((1 & 0x00000001) << 12) | // bit[   12] LP_HBP_EN             = 1 (Enables Low Power HBP.
                                     ((1 & 0x00000001) << 11) | // bit[   11] LP_VACT_EN            = 1 (Enables Low Power VACT.
                                     ((1 & 0x00000001) << 10) | // bit[   10] LP_VFP_EN             = 1 (Enables Low Power VFP.
                                     ((1 & 0x00000001) << 9) |  // bit[    9] LP_VBP_EN             = 1 (Enables Low Power VBP.
                                     ((1 & 0x00000001) << 8) |  // bit[    8] LP_VSA_EN             = 1 (Enables Low Power VSA.
                                     ((2 & 0x00000003) << 0);   // bit[ 1: 0] VIDEO_MODE            = 2 (Video transmission mode. - 2=Burst Mode
    MIPI_DSI_HOST.vid_pkt_size.val = DPI_PIXELS_PER_PACKET;     // bit[13: 0] PIXELS_PER_PACKET     =  (Number of pixels per packet.
    MIPI_DSI_HOST.vid_num_chunks.val = DPI_NUMBER_OF_CHUNKS;    // bit[12: 0] NUMBER_OF_CHUNKS      =  (Number of chunks.
    MIPI_DSI_HOST.vid_null_size.val = DPI_NULL_PKT_SIZE;        // bit[12: 0] NULL_PKT_SIZE         =  (NUll packet payload size.
    MIPI_DSI_HOST.vid_hsa_time.val = DPI_HSA_TIME;              // bit[11: 0] THSA                  =  (HSA period configuration
    MIPI_DSI_HOST.vid_hbp_time.val = DPI_HBP_TIME;              // bit[11: 0] THBP                  =  (HBP period configuration
    MIPI_DSI_HOST.vid_hline_time.val = DPI_HLINE_TIME;          // bit[14: 0] THLINE                =  (HLINE period configuration
    MIPI_DSI_HOST.vid_vsa_lines.val = DPI_VSA;                  // bit[ 9: 0] DPIVSALINES           =  (VSA line configuration
    MIPI_DSI_HOST.vid_vbp_lines.val = DPI_VBP;                  // bit[ 9: 0] DPIVBPLINES           =  (VBP line configuration
    MIPI_DSI_HOST.vid_vfp_lines.val = DPI_VFP;                  // bit[ 9: 0] DPIVFPLINES           =  (VFP line configuration
    MIPI_DSI_HOST.vid_vactive_lines.val = DPI_VACT; // bit[13: 0] DPIVACTLINES          =  (VACT line configuration

    // ---------------------------------------
    // Define SFR2GENERIC ADDRESS REGISTERS
    // ---------------------------------------
    printf("[MIPI-HAL] Define SFR2GENERIC ADDRESS REGISTERS.\n");
    MIPI_DSI_HOST.phy_tmr_cfg.val = 0x00000000 |
                                    ((50 & 0x000003ff) << 16) | // bit[25:16] PHY_HS_TO_LP          = 38 (Maximum time that the D-PHY takes to go from HS to LP transmission measured in lane byte clock cycles.
                                    ((104 & 0x000003ff) << 0);  // bit[ 9: 0] PHY_LP_TO_HS          = 96 (Maximum time required to perform a read command in lane byte clock cycles. This register can only be modified when no read command is in progress.
    MIPI_DSI_HOST.lpclk_ctrl.val = 0x00000000 |
                                   ((0 & 0x00000001) << 1) | // bit[    1] AUTO_CLKLANE_CTRL     = 0 (Automatic mechanism to stop providing clock in the clock lane when time allows.
                                   ((0 & 0x00000001) << 0);  // bit[    0] TXREQUESTCLKHS        = 0 (Requesting High Speed Clock Transmission.

    // ---------------------------------------
    // Configure core's timeouts
    // ---------------------------------------
    printf("[MIPI-HAL] Configure core's timeouts.\n");
    MIPI_DSI_HOST.to_cnt_cfg.val = 0x00000000 |
                                   ((0 & 0x0000ffff) << 16) | // bit[31:16] HSTX_TO_CNT           = 0 (Timeout counter that triggers a low-power reception timeout contention detection.
                                   ((0 & 0x0000ffff) << 0);   // bit[15: 0] LPRX_TO_CNT           = 0 (Timeout counter that triggers a high-speed transmission timeout contention detection.
    MIPI_DSI_HOST.hs_rd_to_cnt.val = 0;                       // bit[15: 0] VAL_HS_RD_TO_CNT      = 0 (Timeout definition after High-Speed Read operations.
    MIPI_DSI_HOST.lp_rd_to_cnt.val = 0;                       // bit[15: 0] VAL_LP_RD_TO_CNT      = 0 (Timeout definition after Low-Power Read operations.
    MIPI_DSI_HOST.hs_wr_to_cnt.val = 0;                       // bit[15: 0] VAL_HS_WR_TO_CNT      = 0 (Peripheral Response timeout definition after High-Speed Write operations.
    MIPI_DSI_HOST.lp_wr_to_cnt.val = 0;                       // bit[15: 0] VAL_LP_WR_TO_CNT      = 0 (Peripheral Response timeout definition after Low-Power Write operations.
    MIPI_DSI_HOST.bta_to_cnt.val = 0;                         // bit[15: 0] VAL_BTA_TO_CNT        = 0 (Peripheral Response timeout definition after Bus Turnaround.

    // ---------------------------------------
    // Configure core's phy parameters
    // ---------------------------------------
    printf("[MIPI-HAL] Configure core's phy parameters.\n");
    MIPI_DSI_HOST.phy_tmr_lpclk_cfg.val = 0x00000000 |
                                          ((46 & 0x000003ff) << 16) | // bit[25:16] PHY_CLKHS2LP_TIME     = 56  (Maximum time that the D-PHY takes to go from HS to LP transmission measured in lane byte clock cycles.
                                          ((128 & 0x000003ff) << 0);  // bit[ 9: 0] PHY_CLKLP2HS_TIME     = 125 (Maximum time that the D-PHY takes to go from LP to HS transmission measured in lane byte clock cycles.
    MIPI_DSI_HOST.phy_tmr_rd_cfg.val = 0x00000000 |
                                       ((0 & 0x00007fff) << 0); // bit[14: 0] BTA_MAX_TIME          = 0   (Maximum time required to perform a read command in lane byte clock cycles. This register can only be modified when no read command is in progress.
    MIPI_DSI_HOST.phy_if_cfg.val = 0x00000000 |
                                   ((40 & 0x000000ff) << 8) | // bit[15: 8] PHY_WAIT_TIME_TO_HS   = 40  (PHY wait time to HSTX.
                                   (((MIPI_DPI_LINE_NUM - 1) & 0x00000003) << 0);   // bit[ 1: 0] NUMBEROFLANES-1       = 2-1 (Number of active data lanes.
    MIPI_DSI_HOST.phy_ulps_ctrl.val = 0x00000000 |
                                      ((0 & 0x00000001) << 3) | // bit[    3] PHY_TXEXITULPSLAN     = 0 (ULPS mode Exit on data lanes.
                                      ((0 & 0x00000001) << 2) | // bit[    2] PHY_TXREQULPSLAN      = 0 (ULPS mode Request on data lanes.
                                      ((0 & 0x00000001) << 1) | // bit[    1] PHY_TXEXITULPSCLK     = 0 (ULPS mode Exit on clock lane.
                                      ((0 & 0x00000001) << 0);  // bit[    0] PHY_TXREQULPSCLK      = 0 (ULPS mode Request on clock lane.
    MIPI_DSI_HOST.phy_tx_triggers.val = 0x00000000 |
                                        ((0 & 0x0000000f) << 0); // bit[ 3: 0] TX_TRIGGERS           = 0 (trigger transmissions.
    MIPI_DSI_HOST.vid_shadow_ctrl.val = 0x00000000 |
                                        ((0 & 0x00000001) << 16) | // bit[   16] VID_SHADOW_PIN_REQ    = 0 (When set to 1, the video request is done by external pin. In this mode vid_shadow_req is ignored.
                                        ((0 & 0x00000001) << 8) |  // bit[    8] VID_SHADOW_REQ        = 0 (When set to 1, this bit request that the dpi registers from regbank are copied to the auxiliary registers. When the request is completed this bit is auto clear.
                                        ((0 & 0x00000001) << 0);   // bit[    0] VID_SHADOW_EN         = 0 (When set to 1, DPI receives the active configuration from the auxiliary registers. When the feature is set at the same time than vid_shadow_req the auxiliary registers are automatically updated.

    // ---------------------------------------
    // Enabling all interrupts
    // ---------------------------------------
    printf("[MIPI-HAL] Enabling all interrupts.\n");
    MIPI_DSI_HOST.int_msk0.val = 0xFFFFFFFF;
    MIPI_DSI_HOST.int_msk1.val = 0xFFFFFFFF;

#if TEST_DSI_FPGA
    // ---------------------------------------
    // Waking up Core
    // ---------------------------------------
    printf("[MIPI-HAL] Waking up Core.\n");
    MIPI_DSI_HOST.pwr_up.val                = 0x0;
    MIPI_DSI_HOST.pwr_up.val                = 0xF;

    MIPI_DSI_HOST.phy_rstz.val              = 0x00000000 |
                                                            ((1   & 0x00000001)<< 3) |  // bit[    3] PHY_FORCEPLL          = 1 (D-PHY forcepll signal.
                                                            ((1   & 0x00000001)<< 2) |  // bit[    2] PHY_ENABLECLK         = 1 (Enables the D-PHY Clock Lane Module.
                                                            ((1   & 0x00000001)<< 1) |  // bit[    1] PHY_RSTZ              = 1 (Places the digital section of the D-PHY in the reset state.
                                                            ((1   & 0x00000001)<< 0); // bit[    0] PHY_SHUTDOWNZ         = 1 (Places the complete D-PHY macro in power-down state.
#else
    // ---------------------------------------
    // Configuring new PLL parameters
    // ---------------------------------------
    uint32_t pll_freq = MIPI_DSI_LINE_RATE;
    uint32_t pll_N = 2;
    uint32_t pll_M = (pll_freq / 20000000) * pll_N; // fix 20MHz input clock
    uint8_t hs_freq = 0x1A;
    struct
    {
        uint32_t freq;       /* upper margin of frequency range */
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
    printf("[MIPI-HAL] Configuring new PLL parameters.\n");
    // dwc_mipi_dphy_bd2_tsmc40ulp25_databook_1g5.pdf page84: REFCLK = 24MHz(not 27MHz parameters)
    // mipi_dsi_dphy_write_control(0x44, 0x34); // 1000MHz

    for (int x = 0; ; x++) {
        if (pll_ranges[x].freq == 1500) {
            hs_freq = pll_ranges[x].hs_freq;
            break;
        }
        if ((MIPI_DSI_LINE_RATE >= pll_ranges[x].freq * 1000000) && (MIPI_DSI_LINE_RATE < pll_ranges[x+1].freq * 1000000)) {
            hs_freq = pll_ranges[x].hs_freq;
            break;
        }
    }
    mipi_dsi_dphy_write_control(0x44, hs_freq << 1);
    printf("[MIPI-HAL] DPHY lane_rate: %d Hz, hs_freq: 0x%x, N: %d, M: %d\n", pll_freq, hs_freq, pll_N, pll_M);

    // ---------------------------------------
    // Waking up Core
    // ---------------------------------------
    printf("[MIPI-HAL] Waking up Core.\n");
    MIPI_DSI_HOST.pwr_up.val = 0x0;
    MIPI_DSI_HOST.pwr_up.val = 0xF;

    MIPI_DSI_HOST.phy_rstz.val = 0x00000000 |
                                 ((0 & 0x00000001) << 3) | // bit[    3] PHY_FORCEPLL          = 1 (D-PHY forcepll signal.
                                 ((0 & 0x00000001) << 2) | // bit[    2] PHY_ENABLECLK         = 1 (Enables the D-PHY Clock Lane Module.
                                 ((0 & 0x00000001) << 1) | // bit[    1] PHY_RSTZ              = 1 (Places the digital section of the D-PHY in the reset state.
                                 ((0 & 0x00000001) << 0);  // bit[    0] PHY_SHUTDOWNZ         = 1 (Places the complete D-PHY macro in power-down state.

    delay_ms(1000);

    MIPI_DSI_HOST.phy_rstz.val = 0x00000000 |
                                 ((1 & 0x00000001) << 3) | // bit[    3] PHY_FORCEPLL          = 1 (D-PHY forcepll signal.
                                 ((1 & 0x00000001) << 2) | // bit[    2] PHY_ENABLECLK         = 1 (Enables the D-PHY Clock Lane Module.
                                 ((1 & 0x00000001) << 1) | // bit[    1] PHY_RSTZ              = 1 (Places the digital section of the D-PHY in the reset state.
                                 ((1 & 0x00000001) << 0);  // bit[    0] PHY_SHUTDOWNZ         = 1 (Places the complete D-PHY macro in power-down state.

    delay_us(10);
    mipi_dsi_dphy_write_control(0x10, 0x03); //mpll_pwron

    delay_us(10);
    mipi_dsi_dphy_write_control(0x10, 0x0B); //mpll_reset

    // 20MHz -> 1000MHz
    // N = 2
    // M = 99
    mipi_dsi_dphy_write_control(0x19, 0x30); // 0x00_11_0000
    mipi_dsi_dphy_write_control(0x17, pll_N - 1); // 0x0_0000001
    mipi_dsi_dphy_write_control(0x18, 0x00 | ((pll_M - 1) & 0x1F)); // 0x0_00_00011
    mipi_dsi_dphy_write_control(0x18, 0x80 | (((pll_M - 1) >> 5) & 0x0F)); // 0x1_000_0011

    // ---------------------------------------
    // wait_for_PHY_PWRUP
    // ---------------------------------------
    printf("[MIPI-HAL] wait_for_PHY_PWRUP.\n");
    // Wait till PHY_STATUS[0] == 1.
    printf("[MIPI-HAL] Wait till PHY_STATUS[0] == 1.\n");
    do
    {
        delay_us(50);
        stat = (MIPI_DSI_HOST.phy_status.val);
        printf("stat: 0x%x\n", stat);
    } while (0 == (stat & 0x1));

    // Wait till PHY_STATUS[2] == 1.
    printf("[MIPI-HAL] Wait till PHY_STATUS[2] == 1.\n");
    do
    {
        delay_us(50);
        stat = (MIPI_DSI_HOST.phy_status.val);
    } while (0 == (stat & 0x4));
#endif

    printf("[MIPI-HAL] MIPI DSI Host Controller & D-PHY initialization done.\n");
    return 0;
}

/*
 * @brief  : Initialization of MIPI-DSI Bridge.
 * @params : void
 * @ret    : int
 **/
static int mipi_dsi_bridge_initialization(void)
{
    uint32_t rtn;

    uint32_t HACT = MIPI_DSI_IMAGE_HSIZE;

    uint32_t DPI_HTOTAL        = DPI_HSA + DPI_HBP + DPI_HFP + HACT ;
    uint32_t DPI_VTOTAL        = DPI_VSA + DPI_VBP + DPI_VFP + DPI_VACT ;
    uint32_t DPI_IMG_SIZE = (HACT * DPI_VACT * TEST_DSI_COLOR_WIDTH) / TEST_DSI_TR_WIDTH;
    //...
    MIPI_DSI_BRIDGE.clk_en.clk_en = 1;
    MIPI_DSI_BRIDGE.dpi_v_cfg1.vbank = DPI_VBP;
    MIPI_DSI_BRIDGE.dpi_v_cfg1.vsync = DPI_VSA;
    MIPI_DSI_BRIDGE.dpi_v_cfg0.vtotal = DPI_VTOTAL;
    MIPI_DSI_BRIDGE.dpi_v_cfg0.vdisp = DPI_VACT;
    MIPI_DSI_BRIDGE.dpi_h_cfg1.hsync = DPI_HSA;
    MIPI_DSI_BRIDGE.dpi_h_cfg1.hbank = DPI_HBP;
    MIPI_DSI_BRIDGE.dpi_h_cfg0.htotal = DPI_HTOTAL;
    MIPI_DSI_BRIDGE.dpi_h_cfg0.hdisp = HACT;
    MIPI_DSI_BRIDGE.raw_num_cfg.unalign_64bit_en = 0;
    MIPI_DSI_BRIDGE.raw_num_cfg.raw_num_total = DPI_IMG_SIZE;
    MIPI_DSI_BRIDGE.raw_num_cfg.raw_num_total_set = 1;
    MIPI_DSI_BRIDGE.dpi_misc_config.fifo_underrun_discard_vcnt = HACT;
    MIPI_DSI_BRIDGE.dpi_misc_config.dpi_en = 0;

    // reg_raw_type ：0 RGB888
    //                            1 RGB666，reg_dpi_config: 0 对应dsi手册上的config1 ，1 对于dsi手册上config2
    //                            2 RGB565，reg_dpi_config: 0/1/2 ,分别对应dsi 手册的config1／2／3
    MIPI_DSI_BRIDGE.pixel_type.raw_type = TEST_DSI_COLOR_MODE;
    MIPI_DSI_BRIDGE.pixel_type.dpi_config = 0;
    MIPI_DSI_BRIDGE.pixel_type.data_in_type = 0;

    MIPI_DSI_BRIDGE.yuv_cfg.yuv_protocal = 0;
    MIPI_DSI_BRIDGE.yuv_cfg.yuv_pix_endian = 0;
    MIPI_DSI_BRIDGE.yuv_cfg.yuv422_format = 0;

    printf("credit_thrd: %d\n", MIPI_DSI_BRIDGE.raw_buf_credit_ctl.credit_thrd);
    MIPI_DSI_BRIDGE.raw_buf_credit_ctl.credit_thrd = 1024;

    MIPI_DSI_BRIDGE.dpi_config_update.val  = 0x1;

    printf("[MIPI-HAL] MIPI DSI Bridge initialization done.\n");
    return 0;
}

void mipi_gen_write_cmd(uint8_t cmd, uint32_t len, ...)
{
    va_list arg_ptr;
    int data = 0;
    uint8_t params[200] = {0};
    params[2] = cmd;
    va_start(arg_ptr, len);
    for (int x = 0; x < len; x++)
    {
        data = va_arg(arg_ptr, int);
        params[x + 3] = data;
    }
    va_end(arg_ptr);
    if (len >= 2)
    {
        params[0] = len + 1;
        params[1] = 0x0;
        mipi_dsih_gen_wr_packet(0, 0x29, params, params[0] + 2);
    }
    else if (len == 1)
    {
        params[0] = params[2];
        params[1] = params[3];
        mipi_dsih_gen_wr_packet(0, 0x23, params, 2);
    }
    else
    {
        params[0] = params[2];
        params[1] = 0x0;
        mipi_dsih_gen_wr_packet(0, 0x13, params, 2);
    }
}

void mipi_gen_read_cmd(uint8_t cmd, uint32_t len, ...)
{
    va_list arg_ptr;
    uint8_t *data = 0;
    uint8_t params[200] = {0};

    params[0] = len & 0xFF;
    params[1] = len >> 8;
    mipi_dsih_gen_wr_packet(0, 0x37, params, 2); // Set Maximum Return Packet Size
    // mipi_dsih_gen_rd_packet(0, 0x14, 0, cmd, len, params);
    mipi_dsih_gen_rd_packet(0, 0x24, len, cmd, len, params); // ICN6211 test ok

    va_start(arg_ptr, len);
    for (int x = 0; x < len; x++)
    {
        data = (uint8_t *)va_arg(arg_ptr, int);
        *data = params[x];
    }
    va_end(arg_ptr);
}

void mipi_gen_write_data(uint8_t *data, uint32_t len)
{
    int x = 0, y = 0;
    uint8_t params[200] = {0};

    params[0] = 129;
    params[1] = 0x0;
    params[2] = 0x3C;
    for (y = 0; y < len / 128; y++)
    {
        for (x = 0; x < 128; x++)
        {
            params[x + 3] = data[y * 128 + x];
        }
        mipi_dsih_gen_wr_packet(0, 0x29, params, params[0] + 2);
    }
    if (len % 128)
    {
        params[0] = (len % 128) + 1;
        params[1] = 0x0;
        params[2] = 0x3C;
        for (x = 0; x < len % 128; x++)
        {
            params[x + 3] = data[y * 128 + x];
        }
        mipi_dsih_gen_wr_packet(0, 0x29, params, params[0] + 2);
    }
}

void mipi_dcs_write_cmd(uint8_t cmd, uint32_t len, ...)
{
    va_list arg_ptr;
    int data = 0;
    uint8_t params[200] = {0};
    params[2] = cmd;
    va_start(arg_ptr, len);
    for (int x = 0; x < len; x++)
    {
        data = va_arg(arg_ptr, int);
        params[x + 3] = data;
    }
    va_end(arg_ptr);
    if (len >= 2)
    {
        params[0] = len + 1;
        params[1] = 0x0;
        mipi_dsih_gen_wr_packet(0, 0x39, params, params[0] + 2);
    }
    else if (len == 1)
    {
        params[0] = params[2];
        params[1] = params[3];
        mipi_dsih_gen_wr_packet(0, 0x15, params, 2);
    }
    else
    {
        params[0] = params[2];
        params[1] = 0x0;
        mipi_dsih_gen_wr_packet(0, 0x05, params, 2);
    }
}

void mipi_dcs_read_cmd(uint8_t cmd, uint32_t len, ...)
{
    va_list arg_ptr;
    uint8_t *data = 0;
    uint8_t params[200] = {0};

    params[0] = len & 0xFF;
    params[1] = len >> 8;
    mipi_dsih_gen_wr_packet(0, 0x37, params, 2); // Set Maximum Return Packet Size
    mipi_dsih_gen_rd_packet(0, 0x06, 0x0, cmd, len, params);

    va_start(arg_ptr, len);
    for (int x = 0; x < len; x++)
    {
        data = (uint8_t *)va_arg(arg_ptr, int);
        *data = params[x];
    }
    va_end(arg_ptr);
}

void mipi_dcs_write_data(uint8_t *data, uint32_t len)
{
    int x = 0, y = 0;
    uint8_t params[200] = {0};

    params[0] = 129;
    params[1] = 0x0;
    params[2] = 0x3C;
    for (y = 0; y < len / 128; y++)
    {
        for (x = 0; x < 128; x++)
        {
            params[x + 3] = data[y * 128 + x];
        }
        mipi_dsih_gen_wr_packet(0, 0x39, params, params[0] + 2);
    }
    if (len % 128)
    {
        params[0] = (len % 128) + 1;
        params[1] = 0x0;
        params[2] = 0x3C;
        for (x = 0; x < len % 128; x++)
        {
            params[x + 3] = data[y * 128 + x];
        }
        mipi_dsih_gen_wr_packet(0, 0x39, params, params[0] + 2);
    }
}

/*
 * @brief  : Initialization of MIPI-DSI, including Bridge,
 *           Host-Controller and D-PHY.
 * @params : void
 * @ret    : int
 **/
int mipi_dsi_initialization(void)
{
    int stat;
    printf("[MIPI-HAL] MIPI DSI initialization.\n");
    stat = mipi_dsi_host_phy_initialization();
    if (stat)
    {
        printf("[MIPI-HAL] Initialize MIPI DSI Host&PHY failed.\n");
        return 1;
    }

    mipi_dsih_hal_gen_cmd_mode_en(1);

#if !TEST_DSI_LOOPBACK
#if TEST_DSI_FPGA
    MIPI_DSI_HOST.lpclk_ctrl.val = 0x00000000 |
                                   ((1 & 0x00000001) << 1) | // bit[    1] AUTO_CLKLANE_CTRL     = 0 (Automatic mechanism to stop providing clock in the clock lane when time allows.
                                   ((1 & 0x00000001) << 0);  // bit[    0] TXREQUESTCLKHS        = 1 (Requesting High Speed Clock Transmission.
#else
    MIPI_DSI_HOST.lpclk_ctrl.val = 0x00000000 |
                                   ((0 & 0x00000001) << 1) | // bit[    1] AUTO_CLKLANE_CTRL     = 0 (Automatic mechanism to stop providing clock in the clock lane when time allows.
                                   ((0 & 0x00000001) << 0);  // bit[    0] TXREQUESTCLKHS        = 1 (Requesting High Speed Clock Transmission.
    // MIPI_DSI_HOST.phy_ulps_ctrl.phy_txrequlpsclk = 1;

    // RDDID: Read Display ID(DAH)
    // This read byte returns 24-bit display identification information.
    // The 1 parameter (ID1): the module’s manufacture ID.
    // The 2 parameter (ID2): the module/driver version ID.
    // The 3 parameter (ID3): the module/driver ID
    uint8_t ID1, ID2, ID3;
    mipi_dcs_read_cmd(0xDA, 3, &ID1, &ID2, &ID3);
    printf("ID1: 0x%x, ID2: 0x%x, ID3: 0x%x\n", ID1, ID2, ID3);
#endif
#if TEST_DSI_ST7796
    mipi_dcs_write_cmd(0x11, 0);         //Sleep Out
    delay_ms(200);
    mipi_dcs_write_cmd(0xf0, 1, 0xc3); //enable command 2 part 1
    mipi_dcs_write_cmd(0xf0, 1, 0x96); //enable command 2 part 2
    mipi_dcs_write_cmd(0x36, 1, 0x48);

    mipi_dcs_write_cmd(0x3a, 1, 0x75);         //16bit pixel

    mipi_dcs_write_cmd(0xb4, 1, 0x01);
    mipi_dcs_write_cmd(0xb7, 1, 0xc6);
    mipi_dcs_write_cmd(0xe8, 8, 0x40, 0x8a, 0x00, 0x00, 0x29, 0x19, 0xa5, 0x33);
    mipi_dcs_write_cmd(0xc1, 5, 0x06, 0xc2, 0xa7, 0xc5, 0x18);
    mipi_dcs_write_cmd(0xe0, 14, 0xf0, 0x09, 0x0b, 0x06, 0x04, 0x15, 0x2f, 0x54, 0x42, 0x3c, 0x17, 0x14, 0x18, 0x1b);

    //Negative Voltage Gamma Coltrol
    mipi_dcs_write_cmd(0xe1, 14, 0xf0, 0x09, 0x0b, 0x06, 0x04, 0x03, 0x2d, 0x43, 0x42, 0x3b, 0x16, 0x14, 0x17, 0x1b);

    mipi_dcs_write_cmd(0xf0, 1, 0x3c);
    mipi_dcs_write_cmd(0xf0, 1, 0x69);

    mipi_dcs_write_cmd(0x13, 0);
    mipi_dcs_write_cmd(0x38, 0); //Normal mode
    delay_ms(120);
    mipi_dcs_write_cmd(0x29, 0); //Display ON

#endif

#if TEST_DSI_NT35516
    mipi_dcs_write_cmd(0xFF,4,0xAA,0x55,0x25,0x01);
    mipi_dcs_write_cmd(0xF0,5,0x55,0xAA,0x52,0x08,0x00);
    mipi_dcs_write_cmd(0xB1,1,0xFC);
    mipi_dcs_write_cmd(0xB8,4,0x01,0x02,0x02,0x02);
    mipi_dcs_write_cmd(0xBC,3,0x05,0x05,0x05);
    mipi_dcs_write_cmd(0xB7,2,0x00,0x00);
    mipi_dcs_write_cmd(0xC8,18,0x01,0x00,0x46,0x1E,0x46,0x1E,0x46,0x1E,0x46,0x1E,0x64,0x3C,0x3C,0x64,0x64,0x3C,0x3C,0x64);
    mipi_dcs_write_cmd(0xF0,5,0xAA,0x55,0x52,0x08,0x01);
    mipi_dcs_write_cmd(0xB0,3,0x05,0x05,0x05);
    mipi_dcs_write_cmd(0xB6,3,0x44,0x44,0x44);
    mipi_dcs_write_cmd(0xB1,3,0x05,0x05,0x05);
    mipi_dcs_write_cmd(0xB7,3,0x34,0x34,0x34);
    mipi_dcs_write_cmd(0xB3,3,0x16,0x16,0x16);
    mipi_dcs_write_cmd(0xB4,3,0x0A,0x0A,0x0A);
    mipi_dcs_write_cmd(0xBC,3,0x00,0x90,0x11);
    mipi_dcs_write_cmd(0xBD,3,0x00,0x90,0x11);
    mipi_dcs_write_cmd(0xBE,1,0x51);
    mipi_dcs_write_cmd(0xD1,16,0x00,0x17,0x00,0x24,0x00,0x3D,0x00,0x52,0x00,0x66,0x00,0x86,0x00,0xA0,0x00,0xCC);
    mipi_dcs_write_cmd(0xD2,16,0x00,0xF1,0x01,0x26,0x01,0x4E,0x01,0x8C,0x01,0xBC,0x01,0xBE,0x01,0xE7,0x02,0x0E);
    mipi_dcs_write_cmd(0xD3,16,0x02,0x22,0x02,0x3C,0x02,0x4F,0x02,0x71,0x02,0x90,0x02,0xC6,0x02,0xF1,0x03,0x3A);
    mipi_dcs_write_cmd(0xD4,4,0x03,0xB15,0x03,0xC1);
    mipi_dcs_write_cmd(0xD5,16,0x00,0x17,0x00,0x24,0x00,0x3D,0x00,0x52,0x00,0x66,0x00,0x86,0x00,0xA0,0x00,0xCC);
    mipi_dcs_write_cmd(0xD6,16,0x00,0xF1,0x01,0x26,0x01,0x4E,0x01,0x8C,0x01,0xBC,0x01,0xBE,0x01,0xE7,0x02,0x0E);
    mipi_dcs_write_cmd(0xD7,16,0x02,0x22,0x02,0x3C,0x02,0x4F,0x02,0x71,0x02,0x90,0x02,0xC6,0x02,0xF1,0x03,0x3A);
    mipi_dcs_write_cmd(0xD8,4,0x03,0xB5,0x03,0xC1);
    mipi_dcs_write_cmd(0xD9,16,0x00,0x17,0x00,0x24,0x00,0x3D,0x00,0x52,0x00,0x66,0x00,0x86,0x00,0xA0,0x00,0xCC);
    mipi_dcs_write_cmd(0xDD,16,0x00,0xF1,0x01,0x26,0x01,0x4E,0x01,0x8C,0x01,0xBC,0x01,0xBE,0x01,0xE7,0x02,0x0E);
    mipi_dcs_write_cmd(0xDE,16,0x02,0x22,0x02,0x3C,0x02,0x4F,0x02,0x71,0x02,0x90,0x02,0xC6,0x02,0xF1,0x03,0x3A);
    mipi_dcs_write_cmd(0xDF,4,0x03,0xB5,0x03,0xC1);
    mipi_dcs_write_cmd(0xE0,16,0x00,0x17,0x00,0x24,0x00,0x3D,0x00,0x52,0x00,0x66,0x00,0x86,0x00,0xA0,0x00,0xCC);
    mipi_dcs_write_cmd(0xE1,16,0x00,0xF1,0x01,0x26,0x01,0x4E,0x01,0x8C,0x01,0xBC,0x01,0xBE,0x01,0xE7,0x02,0x0E);
    mipi_dcs_write_cmd(0xE2,16,0x02,0x22,0x02,0x3C,0x02,0x4F,0x02,0x71,0x02,0x90,0x02,0xC6,0x02,0xF1,0x03,0x3A);
    mipi_dcs_write_cmd(0xE3,4,0x03,0xB5,0x03,0xC1);
    mipi_dcs_write_cmd(0xE4,16,0x00,0x17,0x00,0x24,0x00,0x3D,0x00,0x52,0x00,0x66,0x00,0x86,0x00,0xA0,0x00,0xCC);
    mipi_dcs_write_cmd(0xE5,16,0x00,0xF1,0x01,0x26,0x01,0x4E,0x01,0x8C,0x01,0xBC,0x01,0xBE,0x01,0xE7,0x02,0x0E);
    mipi_dcs_write_cmd(0xE6,16,0x02,0x22,0x02,0x3C,0x02,0x4F,0x02,0x71,0x02,0x90,0x02,0xC6,0x02,0xF1,0x03,0x3A);
    mipi_dcs_write_cmd(0xE7,4,0x03,0xB5,0x03,0xC1);
    mipi_dcs_write_cmd(0xE8,16,0x00,0x17,0x00,0x24,0x00,0x3D,0x00,0x52,0x00,0x66,0x00,0x86,0x00,0xA0,0x00,0xCC);
    mipi_dcs_write_cmd(0xE9,16,0x00,0xF1,0x01,0x26,0x01,0x4E,0x01,0x8C,0x01,0xBC,0x01,0xBE,0x01,0xE7,0x02,0x0E);
    mipi_dcs_write_cmd(0xEA,16,0x02,0x22,0x02,0x3C,0x02,0x4F,0x02,0x71,0x02,0x90,0x02,0xC6,0x02,0xF1,0x03,0x3A);
    mipi_dcs_write_cmd(0xEB,4,0x03,0xB5,0x03,0xC1);
    mipi_dcs_write_cmd(0x35,1,0x0);
    mipi_dcs_write_cmd(0x11, 0);
    delay_ms(200);
    mipi_dcs_write_cmd(0x29, 0);
    mipi_dcs_write_cmd(0x30, 4, 0x0, 0x0, (MIPI_DSI_IMAGE_VSIZE - 1) >> 8, (MIPI_DSI_IMAGE_VSIZE - 1) & 0xFF);
    mipi_dcs_write_cmd(0x36, 1, 0x00);
    mipi_dcs_write_cmd(0x38, 0);
    mipi_dcs_write_cmd(0x3A, 1, 0x75);
    mipi_dcs_write_cmd(0x12, 0);
    // mipi_dcs_write_cmd(0x13, 0);

#endif

#if TEST_DSI_ILI9806E
    mipi_dcs_write_cmd(0xFF, 0x5, 0xFF, 0x98, 0x06, 0x04, 0x00); // Change to Page 0
    mipi_dcs_write_cmd(0x20, 0x1, 0x00);

    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x01); // Change to Page 1
    mipi_dcs_write_cmd(0x08, 1, 0x10);                         // output SDA
    mipi_dcs_write_cmd(0x21, 1, 0x01);                         // DE = 1 Active
    mipi_dcs_write_cmd(0x30, 1, 0x02);                         // 480 X 800
    mipi_dcs_write_cmd(0x31, 1, 0x02);                         // 2-Dot Inversion
    mipi_dcs_write_cmd(0x60, 1, 0x07);                         // SDTI
    mipi_dcs_write_cmd(0x61, 1, 0x06);                         // CRTI
    mipi_dcs_write_cmd(0x62, 1, 0x06);                         // EQTI
    mipi_dcs_write_cmd(0x63, 1, 0x04);                         // PCTI
    mipi_dcs_write_cmd(0x40, 1, 0x14);                         // BT  +2.5/-2.5 pump for DDVDH-L
    mipi_dcs_write_cmd(0x41, 1, 0x55);                         // DVDDH DVDDL clamp
    mipi_dcs_write_cmd(0x42, 1, 0x11);                         // VGH/VGL
    mipi_dcs_write_cmd(0x43, 1, 0x09);                         // VGH clamp
    mipi_dcs_write_cmd(0x44, 1, 0x0C);                         // VGL clamp
    mipi_dcs_write_cmd(0x45, 1, 0x14);                         // VGL_REG  -10V
    mipi_dcs_write_cmd(0x50, 1, 0x50);                         // VGMP
    mipi_dcs_write_cmd(0x51, 1, 0x50);                         // VGMN
    mipi_dcs_write_cmd(0x52, 1, 0x00);                         //Flicker
    mipi_dcs_write_cmd(0x53, 1, 0x47);                         //Flicker4F

    //++++++++++++++++++ Gamma Setting ++++++++++++++++++//

    mipi_dcs_write_cmd(0xA0, 1, 0x00);  // Gamma 0 /255
    mipi_dcs_write_cmd(0xA1, 1, 0x09);  // Gamma 4 /251
    mipi_dcs_write_cmd(0xA2, 1, 0x0C);  // Gamma 8 /247
    mipi_dcs_write_cmd(0xA3, 1, 0x0F);  // Gamma 16    /239
    mipi_dcs_write_cmd(0xA4, 1, 0x06);  // Gamma 24 /231
    mipi_dcs_write_cmd(0xA5, 1, 0x09);  // Gamma 52 / 203
    mipi_dcs_write_cmd(0xA6, 1, 0x07);  // Gamma 80 / 175
    mipi_dcs_write_cmd(0xA7, 1, 0x016); // Gamma 108 /147
    mipi_dcs_write_cmd(0xA8, 1, 0x06);  // Gamma 147 /108
    mipi_dcs_write_cmd(0xA9, 1, 0x09);  // Gamma 175 / 80
    mipi_dcs_write_cmd(0xAA, 1, 0x11);  // Gamma 203 / 52
    mipi_dcs_write_cmd(0xAB, 1, 0x06);  // Gamma 231 / 24
    mipi_dcs_write_cmd(0xAC, 1, 0x0E);  // Gamma 239 / 16
    mipi_dcs_write_cmd(0xAD, 1, 0x19);  // Gamma 247 / 8
    mipi_dcs_write_cmd(0xAE, 1, 0x0C);  // Gamma 251 / 4
    mipi_dcs_write_cmd(0xAF, 1, 0x00);  // Gamma 255 / 0

    ///==============Nagitive
    mipi_dcs_write_cmd(0xC0, 1, 0x00); // Gamma 0 /255
    mipi_dcs_write_cmd(0xC1, 1, 0x09); // Gamma 4  /251
    mipi_dcs_write_cmd(0xC2, 1, 0x0C); // Gamma 8  /247
    mipi_dcs_write_cmd(0xC3, 1, 0x0F); // Gamma 16  /239
    mipi_dcs_write_cmd(0xC4, 1, 0x06); // Gamma 24  /231
    mipi_dcs_write_cmd(0xC5, 1, 0x09); // Gamma 52  /203
    mipi_dcs_write_cmd(0xC6, 1, 0x07); // Gamma 80  /175
    mipi_dcs_write_cmd(0xC7, 1, 0x16); // Gamma 108  /147
    mipi_dcs_write_cmd(0xC8, 1, 0x06); // Gamma 147  /108
    mipi_dcs_write_cmd(0xC9, 1, 0x09); // Gamma 175  /80
    mipi_dcs_write_cmd(0xCA, 1, 0x11); // Gamma 203  /52
    mipi_dcs_write_cmd(0xCB, 1, 0x06); // Gamma 231  /24
    mipi_dcs_write_cmd(0xCC, 1, 0x0E); // Gamma 239  /16
    mipi_dcs_write_cmd(0xCD, 1, 0x19); // Gamma 247  /8
    mipi_dcs_write_cmd(0xCE, 1, 0x0C); // Gamma 251  /4
    mipi_dcs_write_cmd(0xCF, 1, 0x00); // Gamma 255

    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x07); // Change to Page 7

    mipi_dcs_write_cmd(0x17, 1, 0x32);
    delay_ms(10);
    mipi_dcs_write_cmd(0x06, 1, 0x13);
    delay_ms(10);

    delay_ms(10);
    mipi_dcs_write_cmd(0x02, 1, 0x77);
    delay_ms(10);

    mipi_dcs_write_cmd(0x18, 1, 0x1D);
    delay_ms(10);
    mipi_dcs_write_cmd(0xE1, 1, 0x79);
    delay_ms(10);

    //+++++++++++++++++++++++++++++++++++++++++++++++++++//

    //****************************************************************************//
    //****************************** Page 6 Command ******************************//
    //****************************************************************************//
    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x06); // Change to Page 6
    mipi_dcs_write_cmd(0x00, 1, 0xA0);
    mipi_dcs_write_cmd(0x01, 1, 0x05);
    mipi_dcs_write_cmd(0x02, 1, 0x00);
    mipi_dcs_write_cmd(0x03, 1, 0x00);
    mipi_dcs_write_cmd(0x04, 1, 0x01);
    mipi_dcs_write_cmd(0x05, 1, 0x01);
    mipi_dcs_write_cmd(0x06, 1, 0x88);
    mipi_dcs_write_cmd(0x07, 1, 0x04);
    mipi_dcs_write_cmd(0x08, 1, 0x01);
    mipi_dcs_write_cmd(0x09, 1, 0x90);
    mipi_dcs_write_cmd(0x0A, 1, 0x04);
    mipi_dcs_write_cmd(0x0B, 1, 0x01);
    mipi_dcs_write_cmd(0x0C, 1, 0x01);
    mipi_dcs_write_cmd(0x0D, 1, 0x01);
    mipi_dcs_write_cmd(0x0E, 1, 0x00);
    mipi_dcs_write_cmd(0x0F, 1, 0x00);
    mipi_dcs_write_cmd(0x10, 1, 0x55);
    mipi_dcs_write_cmd(0x11, 1, 0x50);
    mipi_dcs_write_cmd(0x12, 1, 0x01);
    mipi_dcs_write_cmd(0x13, 1, 0x85);
    mipi_dcs_write_cmd(0x14, 1, 0x85);
    mipi_dcs_write_cmd(0x15, 1, 0xC0);
    mipi_dcs_write_cmd(0x16, 1, 0x0B);
    mipi_dcs_write_cmd(0x17, 1, 0x00);
    mipi_dcs_write_cmd(0x18, 1, 0x00);
    mipi_dcs_write_cmd(0x19, 1, 0x00);
    mipi_dcs_write_cmd(0x1A, 1, 0x00);
    mipi_dcs_write_cmd(0x1B, 1, 0x00);
    mipi_dcs_write_cmd(0x1C, 1, 0x00);
    mipi_dcs_write_cmd(0x1D, 1, 0x00);

    mipi_dcs_write_cmd(0x20, 1, 0x01);
    mipi_dcs_write_cmd(0x21, 1, 0x23);
    mipi_dcs_write_cmd(0x22, 1, 0x45);
    mipi_dcs_write_cmd(0x23, 1, 0x67);
    mipi_dcs_write_cmd(0x24, 1, 0x01);
    mipi_dcs_write_cmd(0x25, 1, 0x23);
    mipi_dcs_write_cmd(0x26, 1, 0x45);
    mipi_dcs_write_cmd(0x27, 1, 0x67);

    mipi_dcs_write_cmd(0x30, 1, 0x02);
    mipi_dcs_write_cmd(0x31, 1, 0x22);
    mipi_dcs_write_cmd(0x32, 1, 0x11);
    mipi_dcs_write_cmd(0x33, 1, 0xAA);
    mipi_dcs_write_cmd(0x34, 1, 0xBB);
    mipi_dcs_write_cmd(0x35, 1, 0x66);
    mipi_dcs_write_cmd(0x36, 1, 0x00);
    mipi_dcs_write_cmd(0x37, 1, 0x22);
    mipi_dcs_write_cmd(0x38, 1, 0x22);
    mipi_dcs_write_cmd(0x39, 1, 0x22);
    mipi_dcs_write_cmd(0x3A, 1, 0x22);
    mipi_dcs_write_cmd(0x3B, 1, 0x22);
    mipi_dcs_write_cmd(0x3C, 1, 0x22);
    mipi_dcs_write_cmd(0x3D, 1, 0x22);
    mipi_dcs_write_cmd(0x3E, 1, 0x22);
    mipi_dcs_write_cmd(0x3F, 1, 0x22);
    mipi_dcs_write_cmd(0x40, 1, 0x22);
    mipi_dcs_write_cmd(0x52, 1, 0x12);
    mipi_dcs_write_cmd(0x53, 1, 0x12); //VGLO refer VGL_REG   1A

    // //****************************************************************************//
    // //****************************** Page 0 Command ******************************//
    // //****************************************************************************//

    mipi_dcs_write_cmd(0xFF, 5, 0xFF, 0x98, 0x06, 0x04, 0x00); // Change to Page 0

    mipi_dcs_write_cmd(0x11, 1, 0x00); // Sleep-Out
    delay_ms(120);
    mipi_dcs_write_cmd(0x29, 1, 0x00); // Display On

#endif

#if TEST_DSI_ILI9881C
    mipi_dcs_write_cmd(0xFF, 0x03, 0x98, 0x81, 0x01);

    mipi_dcs_read_cmd(0x00, 1, &ID1);
    mipi_dcs_read_cmd(0x01, 1, &ID2);
    mipi_dcs_read_cmd(0x02, 1, &ID3);
    printf("ID1: 0x%x, ID2: 0x%x, ID3: 0x%x\n", ID1, ID2, ID3);

    //800x1280
    //VS_VBP_VFP=2/16/16
    //HS_HBP_HFP=4/138/24

    mipi_dcs_write_cmd(0xFF, 0x03, 0x98, 0x81, 0x03);

    //GIP_1
    mipi_dcs_write_cmd(0x01, 0x01, 0x00);
    mipi_dcs_write_cmd(0x02, 0x01, 0x00);
    mipi_dcs_write_cmd(0x03, 0x01, 0x5D);        //STVA
    mipi_dcs_write_cmd(0x04, 0x01, 0x17);        //STVB
    mipi_dcs_write_cmd(0x05, 0x01, 0x00);        //STVC
    mipi_dcs_write_cmd(0x06, 0x01, 0x0E);        //STVA_Rise
    mipi_dcs_write_cmd(0x07, 0x01, 0x00);        //STVB_Rise
    mipi_dcs_write_cmd(0x08, 0x01, 0x00);        //STVC_Rise
    mipi_dcs_write_cmd(0x09, 0x01, 0x21);        //FTI1R(A) 21
    mipi_dcs_write_cmd(0x0a, 0x01, 0x21);        //FTI2R(B)
    mipi_dcs_write_cmd(0x0b, 0x01, 0x00);        //FTI3R(C)
    mipi_dcs_write_cmd(0x0c, 0x01, 0x06);        //FTI1F(A) 06
    mipi_dcs_write_cmd(0x0d, 0x01, 0x06);        //FTI2F(B)
    mipi_dcs_write_cmd(0x0e, 0x01, 0x00);        //FTI2F(C)
    mipi_dcs_write_cmd(0x0f, 0x01, 0x22);        //CLW1(ALR)  «e«d
    mipi_dcs_write_cmd(0x10, 0x01, 0x22);        //CLW2(ARR)  «e«d
    mipi_dcs_write_cmd(0x11, 0x01, 0x00);
    mipi_dcs_write_cmd(0x12, 0x01, 0x00);
    mipi_dcs_write_cmd(0x13, 0x01, 0x05);        //CLWX(ATF)  ¦Z«d
    mipi_dcs_write_cmd(0x14, 0x01, 0x00);
    mipi_dcs_write_cmd(0x15, 0x01, 0x00);        //GPMRi(ALR)
    mipi_dcs_write_cmd(0x16, 0x01, 0x00);        //GPMRii(ARR)
    mipi_dcs_write_cmd(0x17, 0x01, 0x00);        //GPMFi(ALF)
    mipi_dcs_write_cmd(0x18, 0x01, 0x00);        //GPMFii(AFF)
    mipi_dcs_write_cmd(0x19, 0x01, 0x00);
    mipi_dcs_write_cmd(0x1a, 0x01, 0x00);
    mipi_dcs_write_cmd(0x1b, 0x01, 0x00);
    mipi_dcs_write_cmd(0x1c, 0x01, 0x00);
    mipi_dcs_write_cmd(0x1d, 0x01, 0x00);
    mipi_dcs_write_cmd(0x1e, 0x01, 0x40);        //CLKA 40¦Û°Ê¤Ï C0¤â°Ê¤Ï(X8°Ñ¦ÒCLKB)
    mipi_dcs_write_cmd(0x1f, 0x01, 0xC0);        //C0
    mipi_dcs_write_cmd(0x20, 0x01, 0x0E);        //CLKA_Rise
    mipi_dcs_write_cmd(0x21, 0x01, 0x09);        //CLKA_Fall
    mipi_dcs_write_cmd(0x22, 0x01, 0x0F);        //CLKB_Rise(keep toggle»Ý³]CLK A«á¤@®æ)
    mipi_dcs_write_cmd(0x23, 0x01, 0x00);        //CLKB_Fall
    mipi_dcs_write_cmd(0x24, 0x01, 0x8A);        //CLK keep toggle(AL) 8X©¹¥ª¬Ý
    mipi_dcs_write_cmd(0x25, 0x01, 0x8A);        //CLK keep toggle(AR) 8X©¹¥ª¬Ý
    mipi_dcs_write_cmd(0x26, 0x01, 0x00);
    mipi_dcs_write_cmd(0x27, 0x01, 0x00);
    mipi_dcs_write_cmd(0x28, 0x01, 0x77);       //CLK Phase  KEEP Toggle 7F
    mipi_dcs_write_cmd(0x29, 0x01, 0x77);       //CLK overlap
    mipi_dcs_write_cmd(0x2a, 0x01, 0x00);
    mipi_dcs_write_cmd(0x2b, 0x01, 0x00);
    mipi_dcs_write_cmd(0x2c, 0x01, 0x02);      //GCH R
    mipi_dcs_write_cmd(0x2d, 0x01, 0x02);      //GCL R
    mipi_dcs_write_cmd(0x2e, 0x01, 0x07);      //GCH F
    mipi_dcs_write_cmd(0x2f, 0x01, 0x07);      //GCL F
    mipi_dcs_write_cmd(0x30, 0x01, 0x00);
    mipi_dcs_write_cmd(0x31, 0x01, 0x00);
    mipi_dcs_write_cmd(0x32, 0x01, 0x22);       //GCH/L ext2/1¦æ¬°  5E 01:31   5E 00:42
    mipi_dcs_write_cmd(0x33, 0x01, 0x00);
    mipi_dcs_write_cmd(0x34, 0x01, 0x00);       //VDD1&2 non-overlap 04:2.62us
    mipi_dcs_write_cmd(0x35, 0x01, 0x0A);       //GCH/L °Ï¶¡ 00:VS«e 01:VS«á 10:¸óVS 11:frame¤¤
    mipi_dcs_write_cmd(0x36, 0x01, 0x00);
    mipi_dcs_write_cmd(0x37, 0x01, 0x08);       //GCH/L
    mipi_dcs_write_cmd(0x38, 0x01, 0x3C);	   //VDD1&2 toggle 1sec
    mipi_dcs_write_cmd(0x39, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3a, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3b, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3c, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3d, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3e, 0x01, 0x00);
    mipi_dcs_write_cmd(0x3f, 0x01, 0x00);
    mipi_dcs_write_cmd(0x40, 0x01, 0x00);
    mipi_dcs_write_cmd(0x41, 0x01, 0x00);
    mipi_dcs_write_cmd(0x42, 0x01, 0x00);
    mipi_dcs_write_cmd(0x43, 0x01, 0x08);       //GCH/L
    mipi_dcs_write_cmd(0x44, 0x01, 0x00);

    //GIP_2
    mipi_dcs_write_cmd(0x50, 0x01, 0x01);
    mipi_dcs_write_cmd(0x51, 0x01, 0x23);
    mipi_dcs_write_cmd(0x52, 0x01, 0x45);
    mipi_dcs_write_cmd(0x53, 0x01, 0x67);
    mipi_dcs_write_cmd(0x54, 0x01, 0x89);
    mipi_dcs_write_cmd(0x55, 0x01, 0xAB);
    mipi_dcs_write_cmd(0x56, 0x01, 0x01);
    mipi_dcs_write_cmd(0x57, 0x01, 0x23);
    mipi_dcs_write_cmd(0x58, 0x01, 0x45);
    mipi_dcs_write_cmd(0x59, 0x01, 0x67);
    mipi_dcs_write_cmd(0x5a, 0x01, 0x89);
    mipi_dcs_write_cmd(0x5b, 0x01, 0xAB);
    mipi_dcs_write_cmd(0x5c, 0x01, 0xCD);
    mipi_dcs_write_cmd(0x5d, 0x01, 0xEF);

    //GIP_3
    mipi_dcs_write_cmd(0x5e, 0x01, 0x00);
    mipi_dcs_write_cmd(0x5f, 0x01, 0x02);     //FW_CGOUT_L[1]    VGL
    mipi_dcs_write_cmd(0x60, 0x01, 0x02);     //FW_CGOUT_L[2]    VGL
    mipi_dcs_write_cmd(0x61, 0x01, 0x06);     //FW_CGOUT_L[3]    VST_R
    mipi_dcs_write_cmd(0x62, 0x01, 0x0F);     //FW_CGOUT_L[4]    XCLK_R4
    mipi_dcs_write_cmd(0x63, 0x01, 0x0F);     //FW_CGOUT_L[5]    XCLK_R4
    mipi_dcs_write_cmd(0x64, 0x01, 0x13);     //FW_CGOUT_L[6]    CLK_R4
    mipi_dcs_write_cmd(0x65, 0x01, 0x13);     //FW_CGOUT_L[7]    CLK_R4
    mipi_dcs_write_cmd(0x66, 0x01, 0x0E);     //FW_CGOUT_L[8]    XCLK_R3
    mipi_dcs_write_cmd(0x67, 0x01, 0x0E);     //FW_CGOUT_L[9]    XCLK_R3
    mipi_dcs_write_cmd(0x68, 0x01, 0x12);     //FW_CGOUT_L[10]   CLK_R3
    mipi_dcs_write_cmd(0x69, 0x01, 0x12);     //FW_CGOUT_L[11]   CLK_R3
    mipi_dcs_write_cmd(0x6a, 0x01, 0x0D);     //FW_CGOUT_L[12]   XCLK_R2
    mipi_dcs_write_cmd(0x6b, 0x01, 0x0D);     //FW_CGOUT_L[13]   XCLK_R2
    mipi_dcs_write_cmd(0x6c, 0x01, 0x11);     //FW_CGOUT_L[14]   CLK_R2
    mipi_dcs_write_cmd(0x6d, 0x01, 0x11);     //FW_CGOUT_L[15]   CLK_R2
    mipi_dcs_write_cmd(0x6e, 0x01, 0x0C);     //FW_CGOUT_L[16]   XCLK_R1
    mipi_dcs_write_cmd(0x6f, 0x01, 0x0C);     //FW_CGOUT_L[17]   XCLK_R1
    mipi_dcs_write_cmd(0x70, 0x01, 0x10);     //FW_CGOUT_L[18]   CLK_R1
    mipi_dcs_write_cmd(0x71, 0x01, 0x10);     //FW_CGOUT_L[19]   CLK_R1
    mipi_dcs_write_cmd(0x72, 0x01, 0x00);     //FW_CGOUT_L[20]   D2U  00
    mipi_dcs_write_cmd(0x73, 0x01, 0x16);     //FW_CGOUT_L[21]   U2D  01
    mipi_dcs_write_cmd(0x74, 0x01, 0x08);     //FW_CGOUT_L[22]   VEND

    mipi_dcs_write_cmd(0x75, 0x01, 0x02);     //BW_CGOUT_L[1]
    mipi_dcs_write_cmd(0x76, 0x01, 0x02);     //BW_CGOUT_L[2]
    mipi_dcs_write_cmd(0x77, 0x01, 0x08);     //BW_CGOUT_L[3]
    mipi_dcs_write_cmd(0x78, 0x01, 0x0f);     //BW_CGOUT_L[4]
    mipi_dcs_write_cmd(0x79, 0x01, 0x0f);     //BW_CGOUT_L[5]
    mipi_dcs_write_cmd(0x7a, 0x01, 0x13);     //BW_CGOUT_L[6]
    mipi_dcs_write_cmd(0x7b, 0x01, 0x13);     //BW_CGOUT_L[7]
    mipi_dcs_write_cmd(0x7c, 0x01, 0x0e);     //BW_CGOUT_L[8]
    mipi_dcs_write_cmd(0x7d, 0x01, 0x0e);     //BW_CGOUT_L[9]
    mipi_dcs_write_cmd(0x7e, 0x01, 0x12);     //BW_CGOUT_L[10]
    mipi_dcs_write_cmd(0x7f, 0x01, 0x12);     //BW_CGOUT_L[11]
    mipi_dcs_write_cmd(0x80, 0x01, 0x0d);     //BW_CGOUT_L[12]
    mipi_dcs_write_cmd(0x81, 0x01, 0x0d);     //BW_CGOUT_L[13]
    mipi_dcs_write_cmd(0x82, 0x01, 0x11);     //BW_CGOUT_L[14]
    mipi_dcs_write_cmd(0x83, 0x01, 0x11);     //BW_CGOUT_L[15]
    mipi_dcs_write_cmd(0x84, 0x01, 0x0c);     //BW_CGOUT_L[16]
    mipi_dcs_write_cmd(0x85, 0x01, 0x0c);     //BW_CGOUT_L[17]
    mipi_dcs_write_cmd(0x86, 0x01, 0x10);     //BW_CGOUT_L[18]
    mipi_dcs_write_cmd(0x87, 0x01, 0x10);     //BW_CGOUT_L[19]
    mipi_dcs_write_cmd(0x88, 0x01, 0x17);     //BW_CGOUT_L[20]
    mipi_dcs_write_cmd(0x89, 0x01, 0x01);     //BW_CGOUT_L[21]
    mipi_dcs_write_cmd(0x8a, 0x01, 0x06);     //BW_CGOUT_L[22]

    //CMD_Page 4
    mipi_dcs_write_cmd(0xFF, 0x03, 0x98, 0x81, 0x04);

    mipi_dcs_write_cmd(0x6E, 0x01, 0x2A);   //2B        /VGH 15V
    mipi_dcs_write_cmd(0x6F, 0x01, 0x37);       // reg vcl + pumping ratio VGH=3x VGL=-3x
    mipi_dcs_write_cmd(0x3A, 0x01, 0xA4);       //POWER SAVING
    mipi_dcs_write_cmd(0x8D, 0x01, 0x25);       //VGL -13V
    mipi_dcs_write_cmd(0x87, 0x01, 0xBA);       //ESD
    mipi_dcs_write_cmd(0xB2, 0x01, 0xD1);
    mipi_dcs_write_cmd(0x88, 0x01, 0x0B);
    mipi_dcs_write_cmd(0x38, 0x01, 0x01);
    mipi_dcs_write_cmd(0x39, 0x01, 0x00);
    mipi_dcs_write_cmd(0xB5, 0x01, 0x07);       //gamma bias
    mipi_dcs_write_cmd(0x31, 0x01, 0x75);       //source bias
    mipi_dcs_write_cmd(0x3B, 0x01, 0x98);

    //CMD_Page 1
    mipi_dcs_write_cmd(0xFF, 0x03, 0x98, 0x81, 0x01);
    mipi_dcs_write_cmd(0x22, 0x01, 0x0A);    //BGR, 0x SS
    mipi_dcs_write_cmd(0x31, 0x01, 0x0A);    //Z inversion
    mipi_dcs_write_cmd(0x35, 0x01, 0x07);  	//chopper
    mipi_dcs_write_cmd(0x52, 0x01, 0x00);    //VCOM1[8]
    mipi_dcs_write_cmd(0x53, 0x01, 0x73);    //VCOM1[7:0]
    mipi_dcs_write_cmd(0x54, 0x01, 0x00);    //VCOM2[8]
    mipi_dcs_write_cmd(0x55, 0x01, 0x3D);    //VCOM2[7:0]
    mipi_dcs_write_cmd(0x50, 0x01, 0xD0);    //VREG1OUT 5.208V
    mipi_dcs_write_cmd(0x51, 0x01, 0xCB);    //VREG2OUT -5.208V
    mipi_dcs_write_cmd(0x60, 0x01, 0x14);    //SDT=2.0
    mipi_dcs_write_cmd(0x62, 0x01, 0x01);    //EQ
    mipi_dcs_write_cmd(0x63, 0x01, 0x01);    //PC

    //============Gamma START=============

    //Pos Register
    mipi_dcs_write_cmd(0xA0, 0x01, 0x00);
    mipi_dcs_write_cmd(0xA1, 0x01, 0x18);
    mipi_dcs_write_cmd(0xA2, 0x01, 0x28);
    mipi_dcs_write_cmd(0xA3, 0x01, 0x17);
    mipi_dcs_write_cmd(0xA4, 0x01, 0x1C);
    mipi_dcs_write_cmd(0xA5, 0x01, 0x30);
    mipi_dcs_write_cmd(0xA6, 0x01, 0x24);
    mipi_dcs_write_cmd(0xA7, 0x01, 0x24);
    mipi_dcs_write_cmd(0xA8, 0x01, 0x85);
    mipi_dcs_write_cmd(0xA9, 0x01, 0x1C);
    mipi_dcs_write_cmd(0xAA, 0x01, 0x26);
    mipi_dcs_write_cmd(0xAB, 0x01, 0x66);
    mipi_dcs_write_cmd(0xAC, 0x01, 0x19);
    mipi_dcs_write_cmd(0xAD, 0x01, 0x18);
    mipi_dcs_write_cmd(0xAE, 0x01, 0x4F);
    mipi_dcs_write_cmd(0xAF, 0x01, 0x24);
    mipi_dcs_write_cmd(0xB0, 0x01, 0x2B);
    mipi_dcs_write_cmd(0xB1, 0x01, 0x4A);
    mipi_dcs_write_cmd(0xB2, 0x01, 0x59);
    mipi_dcs_write_cmd(0xB3, 0x01, 0x23);

    mipi_dcs_write_cmd(0xB7, 0x01, 0x03);// SET MIPI DSI 2 Lane mode

    //Neg Register
    mipi_dcs_write_cmd(0xC0, 0x01, 0x00);
    mipi_dcs_write_cmd(0xC1, 0x01, 0x18);
    mipi_dcs_write_cmd(0xC2, 0x01, 0x28);
    mipi_dcs_write_cmd(0xC3, 0x01, 0x17);
    mipi_dcs_write_cmd(0xC4, 0x01, 0x1B);
    mipi_dcs_write_cmd(0xC5, 0x01, 0x2F);
    mipi_dcs_write_cmd(0xC6, 0x01, 0x22);
    mipi_dcs_write_cmd(0xC7, 0x01, 0x22);
    mipi_dcs_write_cmd(0xC8, 0x01, 0x87);
    mipi_dcs_write_cmd(0xC9, 0x01, 0x1C);
    mipi_dcs_write_cmd(0xCA, 0x01, 0x27);
    mipi_dcs_write_cmd(0xCB, 0x01, 0x66);
    mipi_dcs_write_cmd(0xCC, 0x01, 0x19);
    mipi_dcs_write_cmd(0xCD, 0x01, 0x1A);
    mipi_dcs_write_cmd(0xCE, 0x01, 0x4E);
    mipi_dcs_write_cmd(0xCF, 0x01, 0x24);
    mipi_dcs_write_cmd(0xD0, 0x01, 0x2A);
    mipi_dcs_write_cmd(0xD1, 0x01, 0x4C);
    mipi_dcs_write_cmd(0xD2, 0x01, 0x5A);
    mipi_dcs_write_cmd(0xD3, 0x01, 0x23);

    //============ Gamma END===========

    //CMD_Page 0
    mipi_dcs_write_cmd(0xFF, 0x03, 0x98, 0x81, 0x00);
    // mipi_dcs_write_cmd(0x36, 0x01, (1ULL << 1));
    // mipi_dcs_write_cmd(0x36, 0x01, (1ULL << 1) || (1ULL << 0));
    mipi_dcs_write_cmd(0x11, 0x01, 0x00);
    delay_ms(120);
    mipi_dcs_write_cmd(0x29, 0x01, 0x00);
    mipi_dcs_write_cmd(0x35, 0x01, 0x00);
    switch(TEST_DSI_COLOR_WIDTH){
        case 16: mipi_dcs_write_cmd(0x3A, 0x01, 0x05); break;
        case 18: mipi_dcs_write_cmd(0x3A, 0x01, 0x06); break;
        case 24: mipi_dcs_write_cmd(0x3A, 0x01, 0x07); break;
    }
    // mipi_dcs_write_cmd(0x0B, 0x01, 0x08);

#endif

#if TEST_DSI_NT35532

    mipi_dcs_write_cmd(0xFF, 1, 0x01);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0x02);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0x03);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0x04);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0x05);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xD7, 1, 0x31);
    mipi_dcs_write_cmd(0xD8, 1, 0x7E);
    delay_ms(100);
    mipi_dcs_write_cmd(0xFF, 1, 0x00);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xBA, 1, 0x01);
    mipi_dcs_write_cmd(0x36, 1, 0x00);
    mipi_dcs_write_cmd(0xB0, 1, 0x00);
    mipi_dcs_write_cmd(0xD3, 1, 0x08);
    mipi_dcs_write_cmd(0xD4, 1, 0x0E);
    mipi_dcs_write_cmd(0xD5, 1, 0x0F);
    mipi_dcs_write_cmd(0xD6, 1, 0x48);
    mipi_dcs_write_cmd(0xD7, 1, 0x00);
    mipi_dcs_write_cmd(0xD9, 1, 0x00);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0xEE);
    mipi_dcs_write_cmd(0x40, 1, 0x00);
    mipi_dcs_write_cmd(0x41, 1, 0x00);
    mipi_dcs_write_cmd(0x42, 1, 0x00);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0xFF, 1, 0x01);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0x01, 1, 0x55);
    mipi_dcs_write_cmd(0x04, 1, 0x0C);
    mipi_dcs_write_cmd(0x05, 1, 0x3A);
    mipi_dcs_write_cmd(0x06, 1, 0x50);
    mipi_dcs_write_cmd(0x07, 1, 0xD0);
    mipi_dcs_write_cmd(0x0A, 1, 0x0F);
    mipi_dcs_write_cmd(0x0C, 1, 0x06);
    mipi_dcs_write_cmd(0x0D, 1, 0x7F);
    mipi_dcs_write_cmd(0x0E, 1, 0x7F);
    mipi_dcs_write_cmd(0x0F, 1, 0x70);
    mipi_dcs_write_cmd(0x10, 1, 0x63);
    mipi_dcs_write_cmd(0x11, 1, 0x3C);
    mipi_dcs_write_cmd(0x12, 1, 0x5C);
    mipi_dcs_write_cmd(0x13, 1, 0x5A);
    mipi_dcs_write_cmd(0x14, 1, 0x5A);
    mipi_dcs_write_cmd(0x15, 1, 0x60);
    mipi_dcs_write_cmd(0x16, 1, 0x15);
    mipi_dcs_write_cmd(0x17, 1, 0x15);
    mipi_dcs_write_cmd(0x23, 1, 0x00);
    mipi_dcs_write_cmd(0x24, 1, 0x00);
    mipi_dcs_write_cmd(0x25, 1, 0x00);
    mipi_dcs_write_cmd(0x26, 1, 0x00);
    mipi_dcs_write_cmd(0x27, 1, 0x00);
    mipi_dcs_write_cmd(0x28, 1, 0x00);
    mipi_dcs_write_cmd(0x44, 1, 0x00);
    mipi_dcs_write_cmd(0x45, 1, 0x00);
    mipi_dcs_write_cmd(0x46, 1, 0x00);
    mipi_dcs_write_cmd(0x5B, 1, 0xCA);
    mipi_dcs_write_cmd(0x5C, 1, 0x00);
    mipi_dcs_write_cmd(0x5D, 1, 0x00);
    mipi_dcs_write_cmd(0x5E, 1, 0x2D);
    mipi_dcs_write_cmd(0x5F, 1, 0x1B);
    mipi_dcs_write_cmd(0x60, 1, 0xD5);
    mipi_dcs_write_cmd(0x61, 1, 0xF7);
    mipi_dcs_write_cmd(0x6C, 1, 0xAB);
    mipi_dcs_write_cmd(0x6D, 1, 0x44);
    mipi_dcs_write_cmd(0xFF, 1, 0x05);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    mipi_dcs_write_cmd(0x00, 1, 0x3F);
    mipi_dcs_write_cmd(0x01, 1, 0x3F);
    mipi_dcs_write_cmd(0x02, 1, 0x3F);
    mipi_dcs_write_cmd(0x03, 1, 0x3F);
    mipi_dcs_write_cmd(0x04, 1, 0x38);
    mipi_dcs_write_cmd(0x05, 1, 0x3F);
    mipi_dcs_write_cmd(0x06, 1, 0x3F);
    mipi_dcs_write_cmd(0x07, 1, 0x19);
    mipi_dcs_write_cmd(0x08, 1, 0x1D);
    mipi_dcs_write_cmd(0x09, 1, 0x3F);
    mipi_dcs_write_cmd(0x0A, 1, 0x3F);
    mipi_dcs_write_cmd(0x0B, 1, 0x1B);
    mipi_dcs_write_cmd(0x0C, 1, 0x17);
    mipi_dcs_write_cmd(0x0D, 1, 0x3F);
    mipi_dcs_write_cmd(0x0E, 1, 0x3F);
    mipi_dcs_write_cmd(0x0F, 1, 0x08);
    mipi_dcs_write_cmd(0x10, 1, 0x3F);
    mipi_dcs_write_cmd(0x11, 1, 0x10);
    mipi_dcs_write_cmd(0x12, 1, 0x3F);
    mipi_dcs_write_cmd(0x13, 1, 0x3F);
    mipi_dcs_write_cmd(0x14, 1, 0x3F);
    mipi_dcs_write_cmd(0x15, 1, 0x3F);
    mipi_dcs_write_cmd(0x16, 1, 0x3F);
    mipi_dcs_write_cmd(0x17, 1, 0x3F);
    mipi_dcs_write_cmd(0x18, 1, 0x38);
    mipi_dcs_write_cmd(0x19, 1, 0x18);
    mipi_dcs_write_cmd(0x1A, 1, 0x1C);
    mipi_dcs_write_cmd(0x1B, 1, 0x3F);
    mipi_dcs_write_cmd(0x1C, 1, 0x3F);
    mipi_dcs_write_cmd(0x1D, 1, 0x1A);
    mipi_dcs_write_cmd(0x1E, 1, 0x16);
    mipi_dcs_write_cmd(0x1F, 1, 0x3F);
    mipi_dcs_write_cmd(0x20, 1, 0x3F);
    mipi_dcs_write_cmd(0x21, 1, 0x3F);
    mipi_dcs_write_cmd(0x22, 1, 0x3F);
    mipi_dcs_write_cmd(0x23, 1, 0x06);
    mipi_dcs_write_cmd(0x24, 1, 0x3F);
    mipi_dcs_write_cmd(0x25, 1, 0x0E);
    mipi_dcs_write_cmd(0x26, 1, 0x3F);
    mipi_dcs_write_cmd(0x27, 1, 0x3F);
    mipi_dcs_write_cmd(0x54, 1, 0x06);
    mipi_dcs_write_cmd(0x55, 1, 0x05);
    mipi_dcs_write_cmd(0x56, 1, 0x04);
    mipi_dcs_write_cmd(0x58, 1, 0x03);
    mipi_dcs_write_cmd(0x59, 1, 0x1B);
    mipi_dcs_write_cmd(0x5A, 1, 0x1B);
    mipi_dcs_write_cmd(0x5B, 1, 0x01);
    mipi_dcs_write_cmd(0x5C, 1, 0x32);
    mipi_dcs_write_cmd(0x5E, 1, 0x18);
    mipi_dcs_write_cmd(0x5F, 1, 0x20);
    mipi_dcs_write_cmd(0x60, 1, 0x2B);
    mipi_dcs_write_cmd(0x61, 1, 0x2C);
    mipi_dcs_write_cmd(0x62, 1, 0x18);
    mipi_dcs_write_cmd(0x63, 1, 0x01);
    mipi_dcs_write_cmd(0x64, 1, 0x32);
    mipi_dcs_write_cmd(0x65, 1, 0x00);
    mipi_dcs_write_cmd(0x66, 1, 0x44);
    mipi_dcs_write_cmd(0x67, 1, 0x11);
    mipi_dcs_write_cmd(0x68, 1, 0x01);
    mipi_dcs_write_cmd(0x69, 1, 0x01);
    mipi_dcs_write_cmd(0x6A, 1, 0x04);
    mipi_dcs_write_cmd(0x6B, 1, 0x2C);
    mipi_dcs_write_cmd(0x6C, 1, 0x08);
    mipi_dcs_write_cmd(0x6D, 1, 0x08);
    mipi_dcs_write_cmd(0x78, 1, 0x00);
    mipi_dcs_write_cmd(0x79, 1, 0x00);
    mipi_dcs_write_cmd(0x7E, 1, 0x00);
    mipi_dcs_write_cmd(0x7F, 1, 0x00);
    mipi_dcs_write_cmd(0x80, 1, 0x00);
    mipi_dcs_write_cmd(0x81, 1, 0x00);
    mipi_dcs_write_cmd(0x8D, 1, 0x00);
    mipi_dcs_write_cmd(0x8E, 1, 0x00);
    mipi_dcs_write_cmd(0x8F, 1, 0xC0);
    mipi_dcs_write_cmd(0x90, 1, 0x73);
    mipi_dcs_write_cmd(0x91, 1, 0x10);
    mipi_dcs_write_cmd(0x92, 1, 0x07);
    mipi_dcs_write_cmd(0x96, 1, 0x11);
    mipi_dcs_write_cmd(0x97, 1, 0x14);
    mipi_dcs_write_cmd(0x98, 1, 0x00);
    mipi_dcs_write_cmd(0x99, 1, 0x00);
    mipi_dcs_write_cmd(0x9A, 1, 0x00);
    mipi_dcs_write_cmd(0x9B, 1, 0x61);
    mipi_dcs_write_cmd(0x9C, 1, 0x15);
    mipi_dcs_write_cmd(0x9D, 1, 0x30);
    mipi_dcs_write_cmd(0x9F, 1, 0x0F);
    mipi_dcs_write_cmd(0xA2, 1, 0xB0);
    mipi_dcs_write_cmd(0xA7, 1, 0x0A);
    mipi_dcs_write_cmd(0xA9, 1, 0x00);
    mipi_dcs_write_cmd(0xAA, 1, 0x70);
    mipi_dcs_write_cmd(0xAB, 1, 0xDA);
    mipi_dcs_write_cmd(0xAC, 1, 0xFF);
    mipi_dcs_write_cmd(0xAE, 1, 0xF4);
    mipi_dcs_write_cmd(0xAF, 1, 0x40);
    mipi_dcs_write_cmd(0xB0, 1, 0x7F);
    mipi_dcs_write_cmd(0xB1, 1, 0x16);
    mipi_dcs_write_cmd(0xB2, 1, 0x53);
    mipi_dcs_write_cmd(0xB3, 1, 0x00);
    mipi_dcs_write_cmd(0xB4, 1, 0x2A);
    mipi_dcs_write_cmd(0xB5, 1, 0x3A);
    mipi_dcs_write_cmd(0xB6, 1, 0xF0);
    mipi_dcs_write_cmd(0xBC, 1, 0x85);
    mipi_dcs_write_cmd(0xBD, 1, 0xF4);
    mipi_dcs_write_cmd(0xBE, 1, 0x33);
    mipi_dcs_write_cmd(0xBF, 1, 0x13);
    mipi_dcs_write_cmd(0xC0, 1, 0x77);
    mipi_dcs_write_cmd(0xC1, 1, 0x77);
    mipi_dcs_write_cmd(0xC2, 1, 0x77);
    mipi_dcs_write_cmd(0xC3, 1, 0x77);
    mipi_dcs_write_cmd(0xC4, 1, 0x77);
    mipi_dcs_write_cmd(0xC5, 1, 0x77);
    mipi_dcs_write_cmd(0xC6, 1, 0x77);
    mipi_dcs_write_cmd(0xC7, 1, 0x77);
    mipi_dcs_write_cmd(0xC8, 1, 0xAA);
    mipi_dcs_write_cmd(0xC9, 1, 0x2A);
    mipi_dcs_write_cmd(0xCA, 1, 0x00);
    mipi_dcs_write_cmd(0xCB, 1, 0xAA);
    mipi_dcs_write_cmd(0xCC, 1, 0x92);
    mipi_dcs_write_cmd(0xCD, 1, 0x00);
    mipi_dcs_write_cmd(0xCE, 1, 0x18);
    mipi_dcs_write_cmd(0xCF, 1, 0x88);
    mipi_dcs_write_cmd(0xD0, 1, 0xAA);
    mipi_dcs_write_cmd(0xD1, 1, 0x00);
    mipi_dcs_write_cmd(0xD2, 1, 0x00);
    mipi_dcs_write_cmd(0xD3, 1, 0x00);
    mipi_dcs_write_cmd(0xD6, 1, 0x02);
    mipi_dcs_write_cmd(0xED, 1, 0x00);
    mipi_dcs_write_cmd(0xEE, 1, 0x00);
    mipi_dcs_write_cmd(0xEF, 1, 0x70);
    mipi_dcs_write_cmd(0xFA, 1, 0x03);
    mipi_dcs_write_cmd(0xFF, 1, 0x00);

    //page selection cmd start
    mipi_dcs_write_cmd(0xFF, 1, 0x01);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    //page selection cmd end
    //R(+) MCR cmd
    mipi_dcs_write_cmd(0x75, 1, 0x00);
    mipi_dcs_write_cmd(0x76, 1, 0x00);
    mipi_dcs_write_cmd(0x77, 1, 0x00);
    mipi_dcs_write_cmd(0x78, 1, 0x2C);
    mipi_dcs_write_cmd(0x79, 1, 0x00);
    mipi_dcs_write_cmd(0x7A, 1, 0x4F);
    mipi_dcs_write_cmd(0x7B, 1, 0x00);
    mipi_dcs_write_cmd(0x7C, 1, 0x69);
    mipi_dcs_write_cmd(0x7D, 1, 0x00);
    mipi_dcs_write_cmd(0x7E, 1, 0x7F);
    mipi_dcs_write_cmd(0x7F, 1, 0x00);
    mipi_dcs_write_cmd(0x80, 1, 0x92);
    mipi_dcs_write_cmd(0x81, 1, 0x00);
    mipi_dcs_write_cmd(0x82, 1, 0xA3);
    mipi_dcs_write_cmd(0x83, 1, 0x00);
    mipi_dcs_write_cmd(0x84, 1, 0xB3);
    mipi_dcs_write_cmd(0x85, 1, 0x00);
    mipi_dcs_write_cmd(0x86, 1, 0xC1);
    mipi_dcs_write_cmd(0x87, 1, 0x00);
    mipi_dcs_write_cmd(0x88, 1, 0xF3);
    mipi_dcs_write_cmd(0x89, 1, 0x01);
    mipi_dcs_write_cmd(0x8A, 1, 0x1B);
    mipi_dcs_write_cmd(0x8B, 1, 0x01);
    mipi_dcs_write_cmd(0x8C, 1, 0x5A);
    mipi_dcs_write_cmd(0x8D, 1, 0x01);
    mipi_dcs_write_cmd(0x8E, 1, 0x8B);
    mipi_dcs_write_cmd(0x8F, 1, 0x01);
    mipi_dcs_write_cmd(0x90, 1, 0xD9);
    mipi_dcs_write_cmd(0x91, 1, 0x02);
    mipi_dcs_write_cmd(0x92, 1, 0x16);
    mipi_dcs_write_cmd(0x93, 1, 0x02);
    mipi_dcs_write_cmd(0x94, 1, 0x18);
    mipi_dcs_write_cmd(0x95, 1, 0x02);
    mipi_dcs_write_cmd(0x96, 1, 0x4E);
    mipi_dcs_write_cmd(0x97, 1, 0x02);
    mipi_dcs_write_cmd(0x98, 1, 0x88);
    mipi_dcs_write_cmd(0x99, 1, 0x02);
    mipi_dcs_write_cmd(0x9A, 1, 0xAC);
    mipi_dcs_write_cmd(0x9B, 1, 0x02);
    mipi_dcs_write_cmd(0x9C, 1, 0xDD);
    mipi_dcs_write_cmd(0x9D, 1, 0x03);
    mipi_dcs_write_cmd(0x9E, 1, 0x01);
    mipi_dcs_write_cmd(0x9F, 1, 0x03);
    mipi_dcs_write_cmd(0xA0, 1, 0x2E);
    mipi_dcs_write_cmd(0xA2, 1, 0x03);
    mipi_dcs_write_cmd(0xA3, 1, 0x3C);
    mipi_dcs_write_cmd(0xA4, 1, 0x03);
    mipi_dcs_write_cmd(0xA5, 1, 0x4C);
    mipi_dcs_write_cmd(0xA6, 1, 0x03);
    mipi_dcs_write_cmd(0xA7, 1, 0x5D);
    mipi_dcs_write_cmd(0xA9, 1, 0x03);
    mipi_dcs_write_cmd(0xAA, 1, 0x70);
    mipi_dcs_write_cmd(0xAB, 1, 0x03);
    mipi_dcs_write_cmd(0xAC, 1, 0x88);
    mipi_dcs_write_cmd(0xAD, 1, 0x03);
    mipi_dcs_write_cmd(0xAE, 1, 0xA8);
    mipi_dcs_write_cmd(0xAF, 1, 0x03);
    mipi_dcs_write_cmd(0xB0, 1, 0xC8);
    mipi_dcs_write_cmd(0xB1, 1, 0x03);
    mipi_dcs_write_cmd(0xB2, 1, 0xFF);
    //R(-) MCR cmd
    mipi_dcs_write_cmd(0xB3, 1, 0x00);
    mipi_dcs_write_cmd(0xB4, 1, 0x00);
    mipi_dcs_write_cmd(0xB5, 1, 0x00);
    mipi_dcs_write_cmd(0xB6, 1, 0x2C);
    mipi_dcs_write_cmd(0xB7, 1, 0x00);
    mipi_dcs_write_cmd(0xB8, 1, 0x4F);
    mipi_dcs_write_cmd(0xB9, 1, 0x00);
    mipi_dcs_write_cmd(0xBA, 1, 0x69);
    mipi_dcs_write_cmd(0xBB, 1, 0x00);
    mipi_dcs_write_cmd(0xBC, 1, 0x7F);
    mipi_dcs_write_cmd(0xBD, 1, 0x00);
    mipi_dcs_write_cmd(0xBE, 1, 0x92);
    mipi_dcs_write_cmd(0xBF, 1, 0x00);
    mipi_dcs_write_cmd(0xC0, 1, 0xA3);
    mipi_dcs_write_cmd(0xC1, 1, 0x00);
    mipi_dcs_write_cmd(0xC2, 1, 0xB3);
    mipi_dcs_write_cmd(0xC3, 1, 0x00);
    mipi_dcs_write_cmd(0xC4, 1, 0xC1);
    mipi_dcs_write_cmd(0xC5, 1, 0x00);
    mipi_dcs_write_cmd(0xC6, 1, 0xF3);
    mipi_dcs_write_cmd(0xC7, 1, 0x01);
    mipi_dcs_write_cmd(0xC8, 1, 0x1B);
    mipi_dcs_write_cmd(0xC9, 1, 0x01);
    mipi_dcs_write_cmd(0xCA, 1, 0x5A);
    mipi_dcs_write_cmd(0xCB, 1, 0x01);
    mipi_dcs_write_cmd(0xCC, 1, 0x8B);
    mipi_dcs_write_cmd(0xCD, 1, 0x01);
    mipi_dcs_write_cmd(0xCE, 1, 0xD9);
    mipi_dcs_write_cmd(0xCF, 1, 0x02);
    mipi_dcs_write_cmd(0xD0, 1, 0x16);
    mipi_dcs_write_cmd(0xD1, 1, 0x02);
    mipi_dcs_write_cmd(0xD2, 1, 0x18);
    mipi_dcs_write_cmd(0xD3, 1, 0x02);
    mipi_dcs_write_cmd(0xD4, 1, 0x4E);
    mipi_dcs_write_cmd(0xD5, 1, 0x02);
    mipi_dcs_write_cmd(0xD6, 1, 0x88);
    mipi_dcs_write_cmd(0xD7, 1, 0x02);
    mipi_dcs_write_cmd(0xD8, 1, 0xAC);
    mipi_dcs_write_cmd(0xD9, 1, 0x02);
    mipi_dcs_write_cmd(0xDA, 1, 0xDD);
    mipi_dcs_write_cmd(0xDB, 1, 0x03);
    mipi_dcs_write_cmd(0xDC, 1, 0x01);
    mipi_dcs_write_cmd(0xDD, 1, 0x03);
    mipi_dcs_write_cmd(0xDE, 1, 0x2E);
    mipi_dcs_write_cmd(0xDF, 1, 0x03);
    mipi_dcs_write_cmd(0xE0, 1, 0x3C);
    mipi_dcs_write_cmd(0xE1, 1, 0x03);
    mipi_dcs_write_cmd(0xE2, 1, 0x4C);
    mipi_dcs_write_cmd(0xE3, 1, 0x03);
    mipi_dcs_write_cmd(0xE4, 1, 0x5D);
    mipi_dcs_write_cmd(0xE5, 1, 0x03);
    mipi_dcs_write_cmd(0xE6, 1, 0x70);
    mipi_dcs_write_cmd(0xE7, 1, 0x03);
    mipi_dcs_write_cmd(0xE8, 1, 0x88);
    mipi_dcs_write_cmd(0xE9, 1, 0x03);
    mipi_dcs_write_cmd(0xEA, 1, 0xA8);
    mipi_dcs_write_cmd(0xEB, 1, 0x03);
    mipi_dcs_write_cmd(0xEC, 1, 0xC8);
    mipi_dcs_write_cmd(0xED, 1, 0x03);
    mipi_dcs_write_cmd(0xEE, 1, 0xFF);
    //G(+) MCR cmd
    mipi_dcs_write_cmd(0xEF, 1, 0x00);
    mipi_dcs_write_cmd(0xF0, 1, 0x00);
    mipi_dcs_write_cmd(0xF1, 1, 0x00);
    mipi_dcs_write_cmd(0xF2, 1, 0x2C);
    mipi_dcs_write_cmd(0xF3, 1, 0x00);
    mipi_dcs_write_cmd(0xF4, 1, 0x4F);
    mipi_dcs_write_cmd(0xF5, 1, 0x00);
    mipi_dcs_write_cmd(0xF6, 1, 0x69);
    mipi_dcs_write_cmd(0xF7, 1, 0x00);
    mipi_dcs_write_cmd(0xF8, 1, 0x7F);
    mipi_dcs_write_cmd(0xF9, 1, 0x00);
    mipi_dcs_write_cmd(0xFA, 1, 0x92);
    //page selection cmd start
    mipi_dcs_write_cmd(0xFF, 1, 0x02);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);
    //page selection cmd end
    mipi_dcs_write_cmd(0x00, 1, 0x00);
    mipi_dcs_write_cmd(0x01, 1, 0xA3);
    mipi_dcs_write_cmd(0x02, 1, 0x00);
    mipi_dcs_write_cmd(0x03, 1, 0xB3);
    mipi_dcs_write_cmd(0x04, 1, 0x00);
    mipi_dcs_write_cmd(0x05, 1, 0xC1);
    mipi_dcs_write_cmd(0x06, 1, 0x00);
    mipi_dcs_write_cmd(0x07, 1, 0xF3);
    mipi_dcs_write_cmd(0x08, 1, 0x01);
    mipi_dcs_write_cmd(0x09, 1, 0x1B);
    mipi_dcs_write_cmd(0x0A, 1, 0x01);
    mipi_dcs_write_cmd(0x0B, 1, 0x5A);
    mipi_dcs_write_cmd(0x0C, 1, 0x01);
    mipi_dcs_write_cmd(0x0D, 1, 0x8B);
    mipi_dcs_write_cmd(0x0E, 1, 0x01);
    mipi_dcs_write_cmd(0x0F, 1, 0xD9);
    mipi_dcs_write_cmd(0x10, 1, 0x02);
    mipi_dcs_write_cmd(0x11, 1, 0x16);
    mipi_dcs_write_cmd(0x12, 1, 0x02);
    mipi_dcs_write_cmd(0x13, 1, 0x18);
    mipi_dcs_write_cmd(0x14, 1, 0x02);
    mipi_dcs_write_cmd(0x15, 1, 0x4E);
    mipi_dcs_write_cmd(0x16, 1, 0x02);
    mipi_dcs_write_cmd(0x17, 1, 0x88);
    mipi_dcs_write_cmd(0x18, 1, 0x02);
    mipi_dcs_write_cmd(0x19, 1, 0xAC);
    mipi_dcs_write_cmd(0x1A, 1, 0x02);
    mipi_dcs_write_cmd(0x1B, 1, 0xDD);
    mipi_dcs_write_cmd(0x1C, 1, 0x03);
    mipi_dcs_write_cmd(0x1D, 1, 0x01);
    mipi_dcs_write_cmd(0x1E, 1, 0x03);
    mipi_dcs_write_cmd(0x1F, 1, 0x2E);
    mipi_dcs_write_cmd(0x20, 1, 0x03);
    mipi_dcs_write_cmd(0x21, 1, 0x3C);
    mipi_dcs_write_cmd(0x22, 1, 0x03);
    mipi_dcs_write_cmd(0x23, 1, 0x4C);
    mipi_dcs_write_cmd(0x24, 1, 0x03);
    mipi_dcs_write_cmd(0x25, 1, 0x5D);
    mipi_dcs_write_cmd(0x26, 1, 0x03);
    mipi_dcs_write_cmd(0x27, 1, 0x70);
    mipi_dcs_write_cmd(0x28, 1, 0x03);
    mipi_dcs_write_cmd(0x29, 1, 0x88);
    mipi_dcs_write_cmd(0x2A, 1, 0x03);
    mipi_dcs_write_cmd(0x2B, 1, 0xA8);
    mipi_dcs_write_cmd(0x2D, 1, 0x03);
    mipi_dcs_write_cmd(0x2F, 1, 0xC8);
    mipi_dcs_write_cmd(0x30, 1, 0x03);
    mipi_dcs_write_cmd(0x31, 1, 0xFF);
    //G(-) MCR cmd
    mipi_dcs_write_cmd(0x32, 1, 0x00);
    mipi_dcs_write_cmd(0x33, 1, 0x00);
    mipi_dcs_write_cmd(0x34, 1, 0x00);
    mipi_dcs_write_cmd(0x35, 1, 0x2C);
    mipi_dcs_write_cmd(0x36, 1, 0x00);
    mipi_dcs_write_cmd(0x37, 1, 0x4F);
    mipi_dcs_write_cmd(0x38, 1, 0x00);
    mipi_dcs_write_cmd(0x39, 1, 0x69);
    mipi_dcs_write_cmd(0x3A, 1, 0x00);
    mipi_dcs_write_cmd(0x3B, 1, 0x7F);
    mipi_dcs_write_cmd(0x3D, 1, 0x00);
    mipi_dcs_write_cmd(0x3F, 1, 0x92);
    mipi_dcs_write_cmd(0x40, 1, 0x00);
    mipi_dcs_write_cmd(0x41, 1, 0xA3);
    mipi_dcs_write_cmd(0x42, 1, 0x00);
    mipi_dcs_write_cmd(0x43, 1, 0xB3);
    mipi_dcs_write_cmd(0x44, 1, 0x00);
    mipi_dcs_write_cmd(0x45, 1, 0xC1);
    mipi_dcs_write_cmd(0x46, 1, 0x00);
    mipi_dcs_write_cmd(0x47, 1, 0xF3);
    mipi_dcs_write_cmd(0x48, 1, 0x01);
    mipi_dcs_write_cmd(0x49, 1, 0x1B);
    mipi_dcs_write_cmd(0x4A, 1, 0x01);
    mipi_dcs_write_cmd(0x4B, 1, 0x5A);
    mipi_dcs_write_cmd(0x4C, 1, 0x01);
    mipi_dcs_write_cmd(0x4D, 1, 0x8B);
    mipi_dcs_write_cmd(0x4E, 1, 0x01);
    mipi_dcs_write_cmd(0x4F, 1, 0xD9);
    mipi_dcs_write_cmd(0x50, 1, 0x02);
    mipi_dcs_write_cmd(0x51, 1, 0x16);
    mipi_dcs_write_cmd(0x52, 1, 0x02);
    mipi_dcs_write_cmd(0x53, 1, 0x18);
    mipi_dcs_write_cmd(0x54, 1, 0x02);
    mipi_dcs_write_cmd(0x55, 1, 0x4E);
    mipi_dcs_write_cmd(0x56, 1, 0x02);
    mipi_dcs_write_cmd(0x58, 1, 0x88);
    mipi_dcs_write_cmd(0x59, 1, 0x02);
    mipi_dcs_write_cmd(0x5A, 1, 0xAC);
    mipi_dcs_write_cmd(0x5B, 1, 0x02);
    mipi_dcs_write_cmd(0x5C, 1, 0xDD);
    mipi_dcs_write_cmd(0x5D, 1, 0x03);
    mipi_dcs_write_cmd(0x5E, 1, 0x01);
    mipi_dcs_write_cmd(0x5F, 1, 0x03);
    mipi_dcs_write_cmd(0x60, 1, 0x2E);
    mipi_dcs_write_cmd(0x61, 1, 0x03);
    mipi_dcs_write_cmd(0x62, 1, 0x3C);
    mipi_dcs_write_cmd(0x63, 1, 0x03);
    mipi_dcs_write_cmd(0x64, 1, 0x4C);
    mipi_dcs_write_cmd(0x65, 1, 0x03);
    mipi_dcs_write_cmd(0x66, 1, 0x5D);
    mipi_dcs_write_cmd(0x67, 1, 0x03);
    mipi_dcs_write_cmd(0x68, 1, 0x70);
    mipi_dcs_write_cmd(0x69, 1, 0x03);
    mipi_dcs_write_cmd(0x6A, 1, 0x88);
    mipi_dcs_write_cmd(0x6B, 1, 0x03);
    mipi_dcs_write_cmd(0x6C, 1, 0xA8);
    mipi_dcs_write_cmd(0x6D, 1, 0x03);
    mipi_dcs_write_cmd(0x6E, 1, 0xC8);
    mipi_dcs_write_cmd(0x6F, 1, 0x03);
    mipi_dcs_write_cmd(0x70, 1, 0xFF);
    //B(+) MCR cmd
    mipi_dcs_write_cmd(0x71, 1, 0x00);
    mipi_dcs_write_cmd(0x72, 1, 0x00);
    mipi_dcs_write_cmd(0x73, 1, 0x00);
    mipi_dcs_write_cmd(0x74, 1, 0x2C);
    mipi_dcs_write_cmd(0x75, 1, 0x00);
    mipi_dcs_write_cmd(0x76, 1, 0x4F);
    mipi_dcs_write_cmd(0x77, 1, 0x00);
    mipi_dcs_write_cmd(0x78, 1, 0x69);
    mipi_dcs_write_cmd(0x79, 1, 0x00);
    mipi_dcs_write_cmd(0x7A, 1, 0x7F);
    mipi_dcs_write_cmd(0x7B, 1, 0x00);
    mipi_dcs_write_cmd(0x7C, 1, 0x92);
    mipi_dcs_write_cmd(0x7D, 1, 0x00);
    mipi_dcs_write_cmd(0x7E, 1, 0xA3);
    mipi_dcs_write_cmd(0x7F, 1, 0x00);
    mipi_dcs_write_cmd(0x80, 1, 0xB3);
    mipi_dcs_write_cmd(0x81, 1, 0x00);
    mipi_dcs_write_cmd(0x82, 1, 0xC1);
    mipi_dcs_write_cmd(0x83, 1, 0x00);
    mipi_dcs_write_cmd(0x84, 1, 0xF3);
    mipi_dcs_write_cmd(0x85, 1, 0x01);
    mipi_dcs_write_cmd(0x86, 1, 0x1B);
    mipi_dcs_write_cmd(0x87, 1, 0x01);
    mipi_dcs_write_cmd(0x88, 1, 0x5A);
    mipi_dcs_write_cmd(0x89, 1, 0x01);
    mipi_dcs_write_cmd(0x8A, 1, 0x8B);
    mipi_dcs_write_cmd(0x8B, 1, 0x01);
    mipi_dcs_write_cmd(0x8C, 1, 0xD9);
    mipi_dcs_write_cmd(0x8D, 1, 0x02);
    mipi_dcs_write_cmd(0x8E, 1, 0x16);
    mipi_dcs_write_cmd(0x8F, 1, 0x02);
    mipi_dcs_write_cmd(0x90, 1, 0x18);
    mipi_dcs_write_cmd(0x91, 1, 0x02);
    mipi_dcs_write_cmd(0x92, 1, 0x4E);
    mipi_dcs_write_cmd(0x93, 1, 0x02);
    mipi_dcs_write_cmd(0x94, 1, 0x88);
    mipi_dcs_write_cmd(0x95, 1, 0x02);
    mipi_dcs_write_cmd(0x96, 1, 0xAC);
    mipi_dcs_write_cmd(0x97, 1, 0x02);
    mipi_dcs_write_cmd(0x98, 1, 0xDD);
    mipi_dcs_write_cmd(0x99, 1, 0x03);
    mipi_dcs_write_cmd(0x9A, 1, 0x01);
    mipi_dcs_write_cmd(0x9B, 1, 0x03);
    mipi_dcs_write_cmd(0x9C, 1, 0x2E);
    mipi_dcs_write_cmd(0x9D, 1, 0x03);
    mipi_dcs_write_cmd(0x9E, 1, 0x3C);
    mipi_dcs_write_cmd(0x9F, 1, 0x03);
    mipi_dcs_write_cmd(0xA0, 1, 0x4C);
    mipi_dcs_write_cmd(0xA2, 1, 0x03);
    mipi_dcs_write_cmd(0xA3, 1, 0x5D);
    mipi_dcs_write_cmd(0xA4, 1, 0x03);
    mipi_dcs_write_cmd(0xA5, 1, 0x70);
    mipi_dcs_write_cmd(0xA6, 1, 0x03);
    mipi_dcs_write_cmd(0xA7, 1, 0x88);
    mipi_dcs_write_cmd(0xA9, 1, 0x03);
    mipi_dcs_write_cmd(0xAA, 1, 0xA8);
    mipi_dcs_write_cmd(0xAB, 1, 0x03);
    mipi_dcs_write_cmd(0xAC, 1, 0xC8);
    mipi_dcs_write_cmd(0xAD, 1, 0x03);
    mipi_dcs_write_cmd(0xAE, 1, 0xFF);
    //B(-) MCR cmd
    mipi_dcs_write_cmd(0xAF, 1, 0x00);
    mipi_dcs_write_cmd(0xB0, 1, 0x00);
    mipi_dcs_write_cmd(0xB1, 1, 0x00);
    mipi_dcs_write_cmd(0xB2, 1, 0x2C);
    mipi_dcs_write_cmd(0xB3, 1, 0x00);
    mipi_dcs_write_cmd(0xB4, 1, 0x4F);
    mipi_dcs_write_cmd(0xB5, 1, 0x00);
    mipi_dcs_write_cmd(0xB6, 1, 0x69);
    mipi_dcs_write_cmd(0xB7, 1, 0x00);
    mipi_dcs_write_cmd(0xB8, 1, 0x7F);
    mipi_dcs_write_cmd(0xB9, 1, 0x00);
    mipi_dcs_write_cmd(0xBA, 1, 0x92);
    mipi_dcs_write_cmd(0xBB, 1, 0x00);
    mipi_dcs_write_cmd(0xBC, 1, 0xA3);
    mipi_dcs_write_cmd(0xBD, 1, 0x00);
    mipi_dcs_write_cmd(0xBE, 1, 0xB3);
    mipi_dcs_write_cmd(0xBF, 1, 0x00);
    mipi_dcs_write_cmd(0xC0, 1, 0xC1);
    mipi_dcs_write_cmd(0xC1, 1, 0x00);
    mipi_dcs_write_cmd(0xC2, 1, 0xF3);
    mipi_dcs_write_cmd(0xC3, 1, 0x01);
    mipi_dcs_write_cmd(0xC4, 1, 0x1B);
    mipi_dcs_write_cmd(0xC5, 1, 0x01);
    mipi_dcs_write_cmd(0xC6, 1, 0x5A);
    mipi_dcs_write_cmd(0xC7, 1, 0x01);
    mipi_dcs_write_cmd(0xC8, 1, 0x8B);
    mipi_dcs_write_cmd(0xC9, 1, 0x01);
    mipi_dcs_write_cmd(0xCA, 1, 0xD9);
    mipi_dcs_write_cmd(0xCB, 1, 0x02);
    mipi_dcs_write_cmd(0xCC, 1, 0x16);
    mipi_dcs_write_cmd(0xCD, 1, 0x02);
    mipi_dcs_write_cmd(0xCE, 1, 0x18);
    mipi_dcs_write_cmd(0xCF, 1, 0x02);
    mipi_dcs_write_cmd(0xD0, 1, 0x4E);
    mipi_dcs_write_cmd(0xD1, 1, 0x02);
    mipi_dcs_write_cmd(0xD2, 1, 0x88);
    mipi_dcs_write_cmd(0xD3, 1, 0x02);
    mipi_dcs_write_cmd(0xD4, 1, 0xAC);
    mipi_dcs_write_cmd(0xD5, 1, 0x02);
    mipi_dcs_write_cmd(0xD6, 1, 0xDD);
    mipi_dcs_write_cmd(0xD7, 1, 0x03);
    mipi_dcs_write_cmd(0xD8, 1, 0x01);
    mipi_dcs_write_cmd(0xD9, 1, 0x03);
    mipi_dcs_write_cmd(0xDA, 1, 0x2E);
    mipi_dcs_write_cmd(0xDB, 1, 0x03);
    mipi_dcs_write_cmd(0xDC, 1, 0x3C);
    mipi_dcs_write_cmd(0xDD, 1, 0x03);
    mipi_dcs_write_cmd(0xDE, 1, 0x4C);
    mipi_dcs_write_cmd(0xDF, 1, 0x03);
    mipi_dcs_write_cmd(0xE0, 1, 0x5D);
    mipi_dcs_write_cmd(0xE1, 1, 0x03);
    mipi_dcs_write_cmd(0xE2, 1, 0x70);
    mipi_dcs_write_cmd(0xE3, 1, 0x03);
    mipi_dcs_write_cmd(0xE4, 1, 0x88);
    mipi_dcs_write_cmd(0xE5, 1, 0x03);
    mipi_dcs_write_cmd(0xE6, 1, 0xA8);
    mipi_dcs_write_cmd(0xE7, 1, 0x03);
    mipi_dcs_write_cmd(0xE8, 1, 0xC8);
    mipi_dcs_write_cmd(0xE9, 1, 0x03);
    mipi_dcs_write_cmd(0xEA, 1, 0xFF);

    mipi_dcs_write_cmd(0xFF, 1, 0x00);
    mipi_dcs_write_cmd(0xFB, 1, 0x01);

    mipi_dcs_write_cmd(0x11, 0);
    delay_ms(120);
    mipi_dcs_write_cmd(0x29, 0);

    // mipi_dcs_write_cmd(0x01, 0);
    // delay_ms(200);

    // mipi_dcs_write_cmd(0x11, 0);
    // delay_ms(120);
    // mipi_dcs_write_cmd(0x29, 0);
    // mipi_dcs_write_cmd(0x30, 4, 0x0, 0x0, (MIPI_DSI_IMAGE_VSIZE - 1) >> 8, (MIPI_DSI_IMAGE_VSIZE - 1) & 0xFF);
    // // Refresh
    // mipi_dcs_write_cmd(0x36, 1, 0x00);
    // // Pixel Format
    // mipi_dcs_write_cmd(0x3A, 1, 0x75);

    // mipi_dcs_write_cmd(0xBA, 1, 0x01);
    // // Normal Display Mode On
    // // mipi_dcs_write_cmd(0x12, 0);
    // mipi_dcs_write_cmd(0x13, 0);

#endif

#if TEST_DSI_ICN6211_800
    printf("init ICN6211 800x480\n");
    delay_ms(200);

    mipi_gen_write_cmd(0x7A, 1, 0xC1);
    mipi_gen_write_cmd(0x20, 1, 0x20);
    mipi_gen_write_cmd(0x21, 1, 0xE0);
    mipi_gen_write_cmd(0x22, 1, 0x13);
    mipi_gen_write_cmd(0x23, 1, 0x64);
    mipi_gen_write_cmd(0x24, 1, 0x50);
    mipi_gen_write_cmd(0x25, 1, 0x64);
    mipi_gen_write_cmd(0x26, 1, 0x00);
    mipi_gen_write_cmd(0x27, 1, 0x08);
    mipi_gen_write_cmd(0x28, 1, 0x04);
    mipi_gen_write_cmd(0x29, 1, 0x04);
    mipi_gen_write_cmd(0x34, 1, 0x80);
    mipi_gen_write_cmd(0x36, 1, 0x64);
    mipi_gen_write_cmd(0x86, 1, 0x29);
    mipi_gen_write_cmd(0xB5, 1, 0xA0);
    mipi_gen_write_cmd(0x5C, 1, 0xFF);
    mipi_gen_write_cmd(0x2A, 1, 0x01);
    mipi_gen_write_cmd(0x56, 1, 0x90);
    mipi_gen_write_cmd(0x6B, 1, 0x71);
    mipi_gen_write_cmd(0x69, 1, 0x28);
    mipi_gen_write_cmd(0x10, 1, 0x40);
    mipi_gen_write_cmd(0x11, 1, 0x88);
    mipi_gen_write_cmd(0xB6, 1, 0x20);
    mipi_gen_write_cmd(0x51, 1, 0x20);
    // mipi_gen_write_cmd(0x14, 1, 0x43);
    // mipi_gen_write_cmd(0x2A, 1, 0x49);
    mipi_gen_write_cmd(0x09, 1, 0x10);

    uint8_t cmd_data[16];
    for (int x = 0; x < 16; x++) {
        delay_ms(200); // must delay
        mipi_gen_read_cmd(0x20 + x, 1, &cmd_data[x]);
        printf("cmd[0x%x]: 0x%x\n", 0x20 + x, cmd_data[x]);
    }
    delay_ms(200);

#endif

#if TEST_DSI_ICN6211_1280
    printf("init ICN6211 1280x800\n");
    delay_ms(200);

    mipi_gen_write_cmd(0x7A, 1, 0xC1);
    mipi_gen_write_cmd(0x20, 1, 0x00);
    mipi_gen_write_cmd(0x21, 1, 0x20);
    mipi_gen_write_cmd(0x22, 1, 0x35);
    mipi_gen_write_cmd(0x23, 1, 0x78);
    mipi_gen_write_cmd(0x24, 1, 0x72);
    mipi_gen_write_cmd(0x25, 1, 0x78);
    mipi_gen_write_cmd(0x26, 1, 0x00);
    mipi_gen_write_cmd(0x27, 1, 0x08);
    mipi_gen_write_cmd(0x28, 1, 0x04);
    mipi_gen_write_cmd(0x29, 1, 0x04);
    mipi_gen_write_cmd(0x34, 1, 0x80);
    mipi_gen_write_cmd(0x36, 1, 0x78);
    mipi_gen_write_cmd(0x86, 1, 0x29);
    mipi_gen_write_cmd(0xB5, 1, 0xA0);
    mipi_gen_write_cmd(0x5C, 1, 0xFF);
    mipi_gen_write_cmd(0x2A, 1, 0x01);
    mipi_gen_write_cmd(0x56, 1, 0x90);
    mipi_gen_write_cmd(0x6B, 1, 0x51);
    mipi_gen_write_cmd(0x69, 1, 0x32);
    mipi_gen_write_cmd(0x10, 1, 0x40);
    mipi_gen_write_cmd(0x11, 1, 0x88);
    mipi_gen_write_cmd(0xB6, 1, 0x20);
    mipi_gen_write_cmd(0x51, 1, 0x20);
    // mipi_gen_write_cmd(0x14, 1, 0x43);
    // mipi_gen_write_cmd(0x2A, 1, 0x49);
    mipi_gen_write_cmd(0x09, 1, 0x10);

    uint8_t cmd_data[16];
    for (int x = 0; x < 16; x++) {
        delay_ms(200); // must delay
        mipi_gen_read_cmd(0x20 + x, 1, &cmd_data[x]);
        printf("cmd[0x%x]: 0x%x\n", 0x20 + x, cmd_data[x]);
    }
    delay_ms(200);

#endif

#if TEST_DSI_ESCAPE_MODE
    // 可以断开 clk lane, 观察data lane0 是否进入了escape mode
    uint16_t data[128];

    printf("esacpe mode test\n");

    for (int x = 0; x < 128; x++) {
        data[x] = 0x001F;
    }

    for (int x = 0; x < 100; x++) {
        // Memory write
        mipi_dcs_write_data((uint8_t *)data, 128 * 2);
        delay_ms(100);
    }
    mipi_dcs_write_cmd(0x29, 0); //Display ON
#endif
#endif
    // MIPI_DSI_HOST.phy_ulps_ctrl.phy_txexitulpsclk = 1;
    mipi_dsih_hal_gen_cmd_mode_en(0);
    MIPI_DSI_HOST.lpclk_ctrl.val = 0x00000000 |
                                   ((1 & 0x00000001) << 1) | // bit[    1] AUTO_CLKLANE_CTRL     = 0 (Automatic mechanism to stop providing clock in the clock lane when time allows.
                                   ((1 & 0x00000001) << 0);  // bit[    0] TXREQUESTCLKHS        = 1 (Requesting High Speed Clock Transmission.
#if TEST_DSI_PATTERN
    MIPI_DSI_HOST.vid_mode_cfg.val = 0x00000000 |
                                     ((1 & 0x00000001) << 24) | // bit[   24] VPG_ORIENTATION       = 1 (Video Pattern Generator Orientation - 0=Vertical / 1=Horizontal
                                     ((0 & 0x00000001) << 20) | // bit[   20] VPG_MODE              = 0 (Video Pattern Generator Mode        - 0=ColorBAr / 1=BER Testing
                                     ((1 & 0x00000001) << 16) | // bit[   16] VPG_EN                = 0 (Video Pattern Generator Activation
                                     ((0 & 0x00000001) << 15) | // bit[   15] LPCMDEN               = 0 (Enables Low Power command transmission.
                                     ((1 & 0x00000001) << 13) | // bit[   13] LP_HFP_EN             = 1 (Enables Low Power HFP.
                                     ((1 & 0x00000001) << 12) | // bit[   12] LP_HBP_EN             = 1 (Enables Low Power HBP.
                                     ((1 & 0x00000001) << 11) | // bit[   11] LP_VACT_EN            = 1 (Enables Low Power VACT.
                                     ((1 & 0x00000001) << 10) | // bit[   10] LP_VFP_EN             = 1 (Enables Low Power VFP.
                                     ((1 & 0x00000001) << 9) |  // bit[    9] LP_VBP_EN             = 1 (Enables Low Power VBP.
                                     ((1 & 0x00000001) << 8) |  // bit[    8] LP_VSA_EN             = 1 (Enables Low Power VSA.
                                     ((2 & 0x00000003) << 0);   // bit[ 1: 0] VIDEO_MODE            = 2 (Video transmission mode. - 2=Burst Mode
    while (1) {
        delay_ms(10);
    }
#endif

    MIPI_DSI_HOST.vid_mode_cfg.val = 0x00000000 |
                                     ((0 & 0x00000001) << 24) | // bit[   24] VPG_ORIENTATION       = 1 (Video Pattern Generator Orientation - 0=Vertical / 1=Horizontal
                                     ((0 & 0x00000001) << 20) | // bit[   20] VPG_MODE              = 0 (Video Pattern Generator Mode        - 0=ColorBAr / 1=BER Testing
                                     ((0 & 0x00000001) << 16) | // bit[   16] VPG_EN                = 0 (Video Pattern Generator Activation
                                     ((1 & 0x00000001) << 15) | // bit[   15] LPCMDEN               = 0 (Enables Low Power command transmission.
                                     ((1 & 0x00000001) << 13) | // bit[   13] LP_HFP_EN             = 1 (Enables Low Power HFP.
                                     ((1 & 0x00000001) << 12) | // bit[   12] LP_HBP_EN             = 1 (Enables Low Power HBP.
                                     ((1 & 0x00000001) << 11) | // bit[   11] LP_VACT_EN            = 1 (Enables Low Power VACT.
                                     ((1 & 0x00000001) << 10) | // bit[   10] LP_VFP_EN             = 1 (Enables Low Power VFP.
                                     ((1 & 0x00000001) << 9) |  // bit[    9] LP_VBP_EN             = 1 (Enables Low Power VBP.
                                     ((1 & 0x00000001) << 8) |  // bit[    8] LP_VSA_EN             = 1 (Enables Low Power VSA.
                                     ((2 & 0x00000003) << 0);   // bit[ 1: 0] VIDEO_MODE            = 2 (Video transmission mode. - 2=Burst Mode

    stat = mipi_dsi_bridge_initialization();
    if (stat)
    {
        printf("[MIPI-HAL] Initialize MIPI DSI Bridge failed.\n");
        return 1;
    }

    printf("[MIPI-HAL] MIPI DSI initialization done.\n");
    return 0;
}
