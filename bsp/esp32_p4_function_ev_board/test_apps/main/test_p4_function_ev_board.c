/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "unity.h"
#include "unity_test_runner.h"

#define TEST_DELAY_TIME_MS          (3000)

static char *TAG = "p4_function_ev_board_test";

TEST_CASE("test display", "[p4_function_ev_board][display]")
{
    bsp_display_start();
}

// Some resources are lazy allocated in the LCD driver, the threadhold is left for that case
#define TEST_MEMORY_LEAK_THRESHOLD (-300)

static size_t before_free_8bit;
static size_t before_free_32bit;

static void check_leak(size_t before_free, size_t after_free, const char *type)
{
    ssize_t delta = after_free - before_free;
    printf("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_MESSAGE(delta >= TEST_MEMORY_LEAK_THRESHOLD, "memory leak");
}

void setUp(void)
{
    before_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
}

void tearDown(void)
{
    size_t after_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t after_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

void app_main(void)
{
    /**
     *   / _ \ / __\/ _ \  /_\  / _ \/ |
     *  / /_\// /  | (_) |//_\\| | | | |
     * / /_\\/ /___ \__, /  _  \ |_| | |
     * \____/\____/   /_/\_/ \_/\___/|_|
     */
    printf("   ___   ___  ___    _    ___  _\r\n");
    printf("  / _ \\ / __\\/ _ \\  /_\\  / _ \\/ |\r\n");
    printf(" / /_\\// /  | (_) |//_\\\\| | | | |\r\n");
    printf("/ /_\\\\/ /___ \\__, /  _  \\ |_| | |\r\n");
    printf("\\____/\\____/   /_/\\_/ \\_/\\___/|_|\r\n");
    unity_run_menu();
}
