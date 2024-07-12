/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Zhaqian
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "board_api.h"
#include "print.h"
#include "qspi1.h"
#include "sfud_cfg.h"
#include <sfud.h>

static bool xip_running = false;

//--------------------------------------------------------------------+
// Boards api
//--------------------------------------------------------------------+
void board_init(void) {
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
    clock_init();
    system_core_clock_update();
    board_timer_stop();
    delay_init();
    at32_button_init();
    uart_print_init(115200);
    /* use qspi flash for app */
    qspi1_init();
    qspi1_xip_init();
    xip_running = true;
}

//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+

const sfud_flash *sfud_dev = NULL;

void board_led_blink(void) {
    while (1) {
        board_led_write(1);
        delay_ms(500);
        board_led_write(0);
        delay_ms(500);
    }
}

#ifdef CHERRYUF2_DFU_BUTTON
bool board_button_pressed(void) {
    return (at32_button_press() == USER_BUTTON);
}
#endif

void board_flash_init(void) {
    if (sfud_init()) {
        printf("Error: sfud_init fail\n");
        board_led_blink();
        return;
    }
    sfud_dev = sfud_get_device(0);
    if (sfud_dev == NULL) {
        printf("Error: sfud_get_device fail\n");
        board_led_blink();
        return;
    }
}

uint32_t board_flash_size(void) {
    return BOARD_FLASH_SIZE;
}

void board_flash_read(uint32_t addr, void *buffer, uint32_t len) {
    if (xip_running) {
        memcpy(buffer, (void *)addr, len);
    } else
        memset(buffer, 0, len);
}

void board_flash_flush(void) {
    printf("\n");
}

void board_flash_write(uint32_t addr, void const *data, uint32_t len) {
    /* erase sector if the address ends in 0x000 */
    if (addr % BOARD_SECTOR_SIZE == 0) {
        printf(".");
        if (xip_running && sfud_dev && sfud_erase_write(sfud_dev, addr - FLASH_BASE_ADDR, len, data)) {
            printf("Error: sfud_erase_write fail\n");
            board_led_blink();
        }
    } else {
        if (xip_running && sfud_dev && sfud_write(sfud_dev, addr - FLASH_BASE_ADDR, len, data)) {
            printf("Error: sfud_write fail\n");
            board_led_blink();
        }
    }
}

void board_flash_erase_app(void) {
    if (xip_running && sfud_dev && sfud_chip_erase(sfud_dev)) {
        printf("Error: sfud_chip_erase fail\n");
        board_led_blink();
    }
}
