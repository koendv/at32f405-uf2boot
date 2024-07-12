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

#ifndef __BOARD_H__
#define __BOARD_H__

#include "at32f402_405.h"
#include "at32f402_405_board.h"
#include <stdbool.h>

// clang-format off

//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
#define LED_PORT              GPIOC
#define LED_PIN               GPIO_PINS_8
#define LED_STATE_ON          0

//--------------------------------------------------------------------+
// BUTTON
//--------------------------------------------------------------------+

// go into ufb mode if button pressed
#define CHERRYUF2_DFU_BUTTON 0

// true if user button pressed
bool board_button_pressed(void);

//--------------------------------------------------------------------+
// QSPI FLASH
//--------------------------------------------------------------------+

#define FLASH_BASE_ADDR       0x90000000UL
#define BOARD_FLASH_ADDR_ZERO 0x90000000UL
#define BOARD_FLASH_APP_START 0x90000000UL
#define BOARD_SECTOR_SIZE     4096U
#define BOARD_SECTOR_COUNT    4096
#define BOARD_FLASH_SIZE      (BOARD_SECTOR_SIZE * BOARD_SECTOR_COUNT)

//--------------------------------------------------------------------+
// USB UF2
//--------------------------------------------------------------------+
#define USBD_VID              0x00AA
#define USBD_PID              0xAAFF
#define USB_MANUFACTURER      "ArteryTek"
#define USB_PRODUCT           "AT32F405"

#define UF2_PRODUCT_NAME      USB_MANUFACTURER " " USB_PRODUCT
#define UF2_BOARD_ID          "AT32F405xC"
#define UF2_VOLUME_LABEL      "CherryUF2"
#define UF2_INDEX_URL         "https://www.arterytek.com/cn/product/AT32F405.jsp"

// clang-format on
//--------------------------------------------------------------------+
// CLOCK
//--------------------------------------------------------------------+
void wk_system_clock_config(void);

static inline void clock_init(void) {
    wk_system_clock_config();
}

#endif
