/*
 * SPDX-FileCopyrightText: 2015-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdbool.h>
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/i2s_std.h"

#include "audio_player.h"
#include "file_iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Player set mute.
 *
 * @param enable: true or false
 *
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_extra_codec_mute_set(bool enable);

/**
 * @brief Player set volume.
 *
 * @param volume: volume set
 * @param volume_set: volume set response
 *
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_extra_codec_volume_set(int volume, int *volume_set);

/**
 * @brief Stop I2S function.
 *
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_extra_codec_dev_stop(void);

/**
 * @brief Resume I2S function.
 *
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_extra_codec_dev_resume(void);

/**
 * @brief Set I2S format to codec.
 *
 * @param rate: Sample rate of sample
 * @param bits_cfg: Bit lengths of one channel data
 * @param ch: Channels of sample
 *
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_extra_codec_set_fs(uint32_t rate, uint32_t bits_cfg, i2s_slot_mode_t ch);

/**
 * @brief Read data from recoder.
 *
 * @param audio_buffer: The pointer of receiving data buffer
 * @param len: Max data buffer length
 * @param bytes_read: Byte number that actually be read, can be NULL if not needed
 * @param timeout_ms: Max block time
 *
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_extra_i2s_read(void *audio_buffer, size_t len, size_t *bytes_read, uint32_t timeout_ms);

/**
 * @brief Write data to player.
 *
 * @param audio_buffer: The pointer of sent data buffer
 * @param len: Max data buffer length
 * @param bytes_written: Byte number that actually be sent, can be NULL if not needed
 * @param timeout_ms: Max block time
 *
 * @return
 *    - ESP_OK: Success
 *    - Others: Fail
 */
esp_err_t bsp_extra_i2s_write(void *audio_buffer, size_t len, size_t *bytes_written, uint32_t timeout_ms);


/**
 * @brief Initialize codec play and record handle.
 *
 * @return
 *      - ESP_OK: Success
 *      - Others: Fail
 */
esp_err_t bsp_extra_codec_init();

/**
 * @brief Initialize audio player task.
 *
 * @param path file path
 *
 * @return
 *      - ESP_OK: Success
 *      - Others: Fail
 */
esp_err_t bsp_extra_player_init(const char *path);

/**
 * @brief Get file_iterator instance.
 *
 * @return pointer of file_iterator_instance_t
 */
file_iterator_instance_t *bsp_extra_get_file_instance(void);

void bsp_extra_player_play_index(int index);

void __attribute__((weak)) bsp_extra_audio_idle_event_callback(audio_player_cb_ctx_t *ctx);
void __attribute__((weak)) bsp_extra_audio_playing_event_callback(audio_player_cb_ctx_t *ctx);

#ifdef __cplusplus
}
#endif
