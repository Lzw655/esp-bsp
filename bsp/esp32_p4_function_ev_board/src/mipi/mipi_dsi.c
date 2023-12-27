/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <stdlib.h>
#include <stdint.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "hp_sys_clkrst_reg.h"
#include "mipi_dsi_bridge_struct.h"
#include "mipi_dsi_host_struct.h"

#include "mipi_dsi.h"

const char *TAG = "mipi_dsi";

/*
 * @brief  : dphy_write_control
 * @params : <testcode>
 *           <testwrite>
 * @ret    : int
 **/
void mipi_dsi_dphy_write_control (uint32_t testcode, uint32_t testwrite)
{
    MIPI_DSI_HOST.phy_tst_ctrl1.val  = 0x00010000 | testcode ;
    MIPI_DSI_HOST.phy_tst_ctrl0.val  = 0x00000002           ;
    MIPI_DSI_HOST.phy_tst_ctrl0.val  = 0x00000000           ;
    MIPI_DSI_HOST.phy_tst_ctrl1.val  = 0x00000000 | testwrite;
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
bool mipi_dsih_hal_gen_packet_header(uint8_t vc, uint8_t packet_type, uint8_t ms_byte, uint8_t ls_byte)
{
    if (vc < 4) {
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
bool mipi_dsih_hal_gen_packet_payload(uint32_t payload)
{
    if (mipi_dsih_hal_gen_write_fifo_full()) {
        return false;
    }

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
bool mipi_dsih_hal_gen_read_payload(uint32_t *payload)
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
bool mipi_dsih_gen_wr_packet(uint8_t vc, uint8_t data_type, uint8_t *params, uint16_t param_length)
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
    for (word_count = 0; (!mipi_dsih_hal_gen_read_fifo_empty()); word_count += 4) {
        mipi_dsih_hal_gen_read_payload(temp);
    }

    if ((params == 0) && (param_length != 0)) {
        /* pointer NULL */
        printf("null params\n");
        return false;
    }

    if (param_length > 200) {
        printf("param length too large\n");
        return false;
    }

    if (param_length > 2) {

        /*
         * LONG PACKET - write word count to header and the rest to payload
         */
        payload = params + (2 * sizeof(params[0]));
        word_count = (params[1] << 8) | params[0];

        // printf("word count: 0x%X\n", word_count);

        if (word_count > 200) {
            printf("word count too large\n");
            return false;
        }

        if ((param_length - 2) < word_count) {
            printf("sent > input payload. complemented with zeroes\n");
            compliment_counter = (param_length - 2) - word_count;
        } else if ((param_length - 2) > word_count) {
            printf("overflow - input > sent. payload truncated\n");
        }

        for (i = 0; i < (param_length - 2); i += j) {
            temp = 0;
            for (j = 0; (j < 4) && ((j + i) < (param_length - 2)); j++) {
                /* temp = (payload[i + 3] << 24) | (payload[i + 2] << 16) | (payload[i + 1] << 8) | payload[i]; */
                temp |= payload[i + j] << (j * 8);
            }
            /* check if payload Tx fifo is not full */
            for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++) {
                if (mipi_dsih_hal_gen_packet_payload(temp)) {
                    break;
                }
                esp_rom_delay_us(500);
            }
            if (!(timeout < DSIH_FIFO_ACTIVE_WAIT)) {
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
        for (i = 0; i < compliment_counter; i++) {

            /* check if payload Tx fifo is not full */
            for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++) {
                if (!mipi_dsih_hal_gen_packet_payload(0x00)) {
                    break;
                }
            }
            if (!(timeout < DSIH_FIFO_ACTIVE_WAIT)) {
                printf("timeout!!\n");
                return false;
            }
        }
    }
    for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++) {
        /* check if payload Tx fifo is not full */
        if (!mipi_dsih_hal_gen_cmd_fifo_full()) {
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
bool mipi_dsih_gen_rd_packet(uint8_t vc,
                             uint8_t data_type, uint8_t msb_byte,
                             uint8_t lsb_byte, uint8_t bytes_to_read,
                             uint8_t *read_buffer)
{
    int timeout = 0;
    int counter = 0;
    int i = 0;
    int last_count = 0;
    uint32_t temp[1] = {0};

    if (bytes_to_read < 1) {
        return false;
    }

    if (read_buffer == NULL) {
        return false;
    }

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
    for (counter = 0; (!mipi_dsih_hal_gen_read_fifo_empty()); counter += 4) {
        mipi_dsih_hal_gen_read_payload(temp);
    }

    for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++) {

        /* check if payload Tx fifo is not full */
        if (!mipi_dsih_hal_gen_cmd_fifo_full()) {

            mipi_dsih_hal_gen_packet_header(vc, data_type,
                                            msb_byte, lsb_byte);
            break;
        }
        esp_rom_delay_us(500);
    }
    if (!(timeout < DSIH_FIFO_ACTIVE_WAIT)) {

        printf("TX READ command timed out\n");
        return false;
    }

    /* loop for the number of words to be read */
    for (timeout = 0; timeout < DSIH_FIFO_ACTIVE_WAIT; timeout++) {

        /* check if command transaction is done */
        if (!mipi_dsih_hal_gen_rd_cmd_busy()) {
            esp_rom_delay_us(500);
            if (!mipi_dsih_hal_gen_read_fifo_empty()) {

                for (counter = 0; (!mipi_dsih_hal_gen_read_fifo_empty()); counter += 4) {
                    mipi_dsih_hal_gen_read_payload(temp);
                    if (counter < bytes_to_read) {

                        for (i = 0; i < 4; i++) {
                            if ((counter + i) < bytes_to_read) {
                                /* put 32 bit temp in 4 bytes of buffer passed by user*/
                                read_buffer[counter + i] = (uint8_t)(temp[0] >> (i * 8));
                                last_count = i + counter;
                            } else {
                                if ((uint8_t)(temp[0] >> (i * 8)) != 0x00) {
                                    last_count = i + counter;
                                }
                            }
                        }
                    } else {
                        last_count = counter;
                        for (i = 0; i < 4; i++) {
                            if ((uint8_t)(temp[0] >> (i * 8)) != 0x00) {
                                last_count = i + counter;
                            }
                        }
                    }
                }
                return last_count + 1;
            } else {
                printf("RX buffer empty\n");
                return true;
            }
        }
        esp_rom_delay_us(500);
    }
    printf("RX command timed out\n");
    return false;
}

void mipi_gen_write_cmd(uint8_t cmd, uint32_t len, ...)
{
    va_list arg_ptr;
    int data = 0;
    uint8_t params[200] = {0};
    params[2] = cmd;
    va_start(arg_ptr, len);
    for (int x = 0; x < len; x++) {
        data = va_arg(arg_ptr, int);
        params[x + 3] = data;
    }
    va_end(arg_ptr);
    if (len >= 2) {
        params[0] = len + 1;
        params[1] = 0x0;
        mipi_dsih_gen_wr_packet(0, 0x29, params, params[0] + 2);
    } else if (len == 1) {
        params[0] = params[2];
        params[1] = params[3];
        mipi_dsih_gen_wr_packet(0, 0x23, params, 2);
    } else {
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
    for (int x = 0; x < len; x++) {
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
    for (y = 0; y < len / 128; y++) {
        for (x = 0; x < 128; x++) {
            params[x + 3] = data[y * 128 + x];
        }
        mipi_dsih_gen_wr_packet(0, 0x29, params, params[0] + 2);
    }
    if (len % 128) {
        params[0] = (len % 128) + 1;
        params[1] = 0x0;
        params[2] = 0x3C;
        for (x = 0; x < len % 128; x++) {
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
    for (int x = 0; x < len; x++) {
        data = va_arg(arg_ptr, int);
        params[x + 3] = data;
    }
    va_end(arg_ptr);
    if (len >= 2) {
        params[0] = len + 1;
        params[1] = 0x0;
        mipi_dsih_gen_wr_packet(0, 0x39, params, params[0] + 2);
    } else if (len == 1) {
        params[0] = params[2];
        params[1] = params[3];
        mipi_dsih_gen_wr_packet(0, 0x15, params, 2);
    } else {
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
    for (int x = 0; x < len; x++) {
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
    for (y = 0; y < len / 128; y++) {
        for (x = 0; x < 128; x++) {
            params[x + 3] = data[y * 128 + x];
        }
        mipi_dsih_gen_wr_packet(0, 0x39, params, params[0] + 2);
    }
    if (len % 128) {
        params[0] = (len % 128) + 1;
        params[1] = 0x0;
        params[2] = 0x3C;
        for (x = 0; x < len % 128; x++) {
            params[x + 3] = data[y * 128 + x];
        }
        mipi_dsih_gen_wr_packet(0, 0x39, params, params[0] + 2);
    }
}

esp_err_t mipi_dsi_clock_init(void)
{
    REG_SET_FIELD(HP_SYS_CLKRST_PERI_CLK_CTRL02_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPHY_CLK_SRC_SEL, 1);
    REG_CLR_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPHY_CFG_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPHY_CFG_CLK_EN);
    REG_CLR_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPHY_PLL_REFCLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPHY_PLL_REFCLK_EN);

    REG_CLR_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_DSI_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_SOC_CLK_CTRL1_REG, HP_SYS_CLKRST_REG_DSI_SYS_CLK_EN);
    REG_SET_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_DSI_BRG);
    REG_CLR_BIT(HP_SYS_CLKRST_HP_RST_EN0_REG, HP_SYS_CLKRST_REG_RST_EN_DSI_BRG);

    REG_SET_FIELD(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPICLK_DIV_NUM, (240000000 / MIPI_DPI_CLOCK_RATE) - 1);
    REG_SET_FIELD(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPICLK_SRC_SEL, 1);
    REG_SET_BIT(HP_SYS_CLKRST_PERI_CLK_CTRL03_REG, HP_SYS_CLKRST_REG_MIPI_DSI_DPICLK_EN);

    printf("[MIPI-HAL] MIPI DSI clock initialization done.\n");

    return ESP_OK;
}

esp_err_t mipi_dsi_host_phy_init(void)
{
    printf("Initialize MIPI-DSI host and phy\n");

    /**
     *  1. When the null packets are enabled:
     *
     *  lanebyteclkperiod * vid_num_chunks (vid_pkt_size * bytes_per_pixel + 12 + vid_null_size) / number_of_lanes =
     *  pixels_per_line * dpipclkperiod
     *
     *  2. When the null packets are disabled:
     *
     *  lanebyteclkperiod * vid_num_chunks (vid_pkt_size * bytes_per_pixel + 6) / number_of_lanes =
     *  pixels_per_line * dpipclkperiod
     */

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

    printf("MIPI_DPI_TIME_FACTOR: %f, DPI_HSA_TIME: %d, DPI_HBP_TIME: %d, DPI_HLINE_TIME: %d, DPI_FPS: %d\n",
           MIPI_DPI_TIME_FACTOR, DPI_HSA_TIME, DPI_HBP_TIME, DPI_HLINE_TIME, DPI_FPS);

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

    // ---------------------------------------
    // Configuring new PLL parameters
    // ---------------------------------------
    uint32_t pll_freq = MIPI_DSI_LINE_RATE;
    uint32_t pll_N = 2;
    uint32_t pll_M = (pll_freq / 20000000) * pll_N; // fix 20MHz input clock
    uint8_t hs_freq = 0x1A;
    struct {
        uint32_t freq;       /* upper margin of frequency range */
        uint8_t hs_freq;   /* hsfreqrange */
        uint8_t vco_range; /* vcorange */
    } pll_ranges[] = {
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
        if ((MIPI_DSI_LINE_RATE >= pll_ranges[x].freq * 1000000) && (MIPI_DSI_LINE_RATE < pll_ranges[x + 1].freq * 1000000)) {
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

    vTaskDelay(pdMS_TO_TICKS(1000));

    MIPI_DSI_HOST.phy_rstz.val = 0x00000000 |
                                 ((1 & 0x00000001) << 3) | // bit[    3] PHY_FORCEPLL          = 1 (D-PHY forcepll signal.
                                 ((1 & 0x00000001) << 2) | // bit[    2] PHY_ENABLECLK         = 1 (Enables the D-PHY Clock Lane Module.
                                 ((1 & 0x00000001) << 1) | // bit[    1] PHY_RSTZ              = 1 (Places the digital section of the D-PHY in the reset state.
                                 ((1 & 0x00000001) << 0);  // bit[    0] PHY_SHUTDOWNZ         = 1 (Places the complete D-PHY macro in power-down state.

    esp_rom_delay_us(10);
    mipi_dsi_dphy_write_control(0x10, 0x03); //mpll_pwron

    esp_rom_delay_us(10);
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

    uint32_t stat = 0;
    do {
        esp_rom_delay_us(50);
        stat = (MIPI_DSI_HOST.phy_status.val);
        printf("stat: 0x%x\n", stat);
    } while (0 == (stat & 0x1));

    // Wait till PHY_STATUS[2] == 1.
    printf("[MIPI-HAL] Wait till PHY_STATUS[2] == 1.\n");
    do {
        esp_rom_delay_us(50);
        stat = (MIPI_DSI_HOST.phy_status.val);
    } while (0 == (stat & 0x4));

    printf("[MIPI-HAL] MIPI DSI Host Controller & D-PHY initialization done.\n");

    return ESP_OK;
}

esp_err_t mipi_dsi_bridge_init(void)
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

    return ESP_OK;
}

esp_err_t mipi_dsi_bridge_start(void)
{
    MIPI_DSI_BRIDGE.dma_flow_ctrl.dsi_dma_flow_controller = 1;
    MIPI_DSI_BRIDGE.dma_req_cfg.dma_burst_len = 256;
    MIPI_DSI_BRIDGE.raw_buf_almost_empty_thrd.dsi_raw_buf_almost_empty_thrd = 1024 - MIPI_DSI_BRIDGE.dma_req_cfg.dma_burst_len;
    // Enable MIPI DSI.
    MIPI_DSI_BRIDGE.en.dsi_en = 0x1;
    // MIPI_DSI_BRIDGE.dpi_rsv_dpi_data.dpi_rsv_data = 0xFFFFFF;
    // Enable MIPI DPI_EN.
    MIPI_DSI_BRIDGE.dpi_misc_config.dpi_en = 0x1;
    MIPI_DSI_BRIDGE.dpi_config_update.val  = 0x1; // Remember to do update.

    printf("[MIPI-HAL] MIPI DSI Bridge start.\n");

    return ESP_OK;
}
