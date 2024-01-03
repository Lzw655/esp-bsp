/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIPI_DSI_RGB888_MODE (0)
#define MIPI_DSI_RGB666_MODE (1)
#define MIPI_DSI_RGB565_MODE (2)
#define MIPI_DSI_YUV422_MODE (3)

#define TEST_DSI_ESCAPE_MODE  (0)
#define TEST_DSI_PATTERN      (0)
#define TEST_DSI_LOOPBACK     (0)
#define TEST_DSI_COLOR_MODE  (MIPI_DSI_RGB565_MODE)
#if (TEST_DSI_COLOR_MODE==MIPI_DSI_RGB888_MODE)
#define TEST_DSI_COLOR_WIDTH (24)
#elif (TEST_DSI_COLOR_MODE==MIPI_DSI_RGB666_MODE)
#define TEST_DSI_COLOR_WIDTH (18)
#elif (TEST_DSI_COLOR_MODE==MIPI_DSI_RGB565_MODE)
#define TEST_DSI_COLOR_WIDTH (16)
#endif
#define TEST_DSI_TR_WIDTH       (64)

#define  MIPI_DSI_RGB888_RED     (0xFF0000)
#define  MIPI_DSI_RGB888_GREEN   (0x00FF00)
#define  MIPI_DSI_RGB888_BLUE    (0x0000FF)
#define  MIPI_DSI_RGB888_YELLOW  (0xFFFF00)
#define  MIPI_DSI_RGB888_CYAN    (0x00FFFF)
#define  MIPI_DSI_RGB888_MAGENTA (0xFF00FF)

#define  MIPI_DSI_RGB565_RED     (0xF800)
#define  MIPI_DSI_RGB565_GREEN   (0x07E0)
#define  MIPI_DSI_RGB565_BLUE    (0x001F)
#define  MIPI_DSI_RGB565_YELLOW  (0xFFE0)
#define  MIPI_DSI_RGB565_CYAN    (0x07FF)
#define  MIPI_DSI_RGB565_MAGENTA (0xF81F)

// #define  MIPI_DSI_BYTE_RATE  (MIPI_DSI_LINE_RATE / 8)

// #define  MIPI_DPI_TIME_FACTOR

typedef struct {
    uint16_t dpi_hsa;
    uint16_t dpi_hbp;
    uint16_t dpi_hfp;
    uint16_t dpi_hact;
    uint16_t dpi_vsa;
    uint16_t dpi_vbp;
    uint16_t dpi_vfp;
    uint16_t dpi_vact;
    uint32_t dpi_clock_rate;
    uint32_t dsi_line_rate;
    uint8_t dsi_line_num;
} mipi_dsi_timing_t;

void mipi_dsi_dphy_write_control (uint32_t testcode, uint32_t testwrite);

/**
 * Write command header in the generic interface
 * (which also sends DCS commands) as a subset
 * @param vc of destination
 * @param packet_type (or type of DCS command)
 * @param ls_byte (if DCS, it is the DCS command)
 * @param ms_byte (only parameter of short DCS packet)
 * @return error code
 */
bool mipi_dsih_hal_gen_packet_header(uint8_t vc, uint8_t packet_type, uint8_t ms_byte, uint8_t ls_byte);

/**
 * Get the FULL status of generic command fifo
 * @return 1 if fifo full
 */
int mipi_dsih_hal_gen_cmd_fifo_full();

/**
 * Get the FULL status of generic write payload fifo
 * @return 1 if fifo full
 */
int mipi_dsih_hal_gen_write_fifo_full();

/**
 * Write the payload of the long packet commands
 * @param payload array of bytes of payload
 * @return error code
 */
bool mipi_dsih_hal_gen_packet_payload(uint32_t payload);

/**
 * Enable command mode (Generic interface)
 * @param  pointer to structure holding the DSI Host core information
 * @param enable
 */
void mipi_dsih_hal_gen_cmd_mode_en(int enable);

/**
 * Enable Bus Turn-around request
 * @param dev pointer to structure holding the DSI Host core information
 * @param enable
 */
void mipi_dsih_hal_bta_en(int enable);

/**
 * Configure the read back virtual channel for the generic interface
 * @param dev pointer to structure holding the DSI Host core information
 * @param vc to listen to on the line
 */
void mipi_dsih_hal_gen_rd_vc(uint8_t vc);

/**
 * Get status of read command
 * @param dev pointer to structure holding the DSI Host core information
 * @return 1 if busy
 */
int mipi_dsih_hal_gen_rd_cmd_busy();

/**
 * Get the EMPTY status of generic read payload fifo
 * @param dev pointer to structure holding the DSI Host core information
 * @return 1 if fifo empty
 */
int mipi_dsih_hal_gen_read_fifo_empty();

/**
 * Write the payload of the long packet commands
 * @param dev pointer to structure holding the DSI Host core information
 * @param payload pointer to 32-bit array to hold read information
 * @return error code
 */
bool mipi_dsih_hal_gen_read_payload(uint32_t *payload);

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
bool mipi_dsih_gen_wr_packet(uint8_t vc, uint8_t data_type, uint8_t *params, uint16_t param_length);

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
                             uint8_t *read_buffer);

void mipi_gen_write_cmd(uint8_t cmd, uint32_t len, ...);

void mipi_gen_read_cmd(uint8_t cmd, uint32_t len, ...);

void mipi_gen_write_data(uint8_t *data, uint32_t len);

void mipi_dcs_write_cmd(uint8_t cmd, uint32_t len, ...);

void mipi_dcs_read_cmd(uint8_t cmd, uint32_t len, ...);

void mipi_dcs_write_data(uint8_t *data, uint32_t len);

void mipi_dsi_set_timing(const mipi_dsi_timing_t *timing);

esp_err_t mipi_dsi_clock_init(void);

esp_err_t mipi_dsi_host_phy_init(void);

esp_err_t mipi_dsi_bridge_init(void);

esp_err_t mipi_dsi_bridge_start(void);

#ifdef __cplusplus
}
#endif
