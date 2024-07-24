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

#ifndef BOARD_H_
#define BOARD_H_

#include "stm32f0xx.h"

// clang-format off
//--------------------------------------------------------------------+
// LED
//--------------------------------------------------------------------+
#define LED_PORT             GPIOA
#define LED_PIN              GPIO_PIN_1
#define LED_STATE_ON         0

//--------------------------------------------------------------------+
// FLASH
//--------------------------------------------------------------------+
#define BOARD_SECTOR_SIZE   1024U
#define BOARD_SECTOR_COUNT  128
#define BOARD_FLASH_SIZE    (BOARD_SECTOR_SIZE * BOARD_SECTOR_COUNT)

//--------------------------------------------------------------------+
// USB UF2
//--------------------------------------------------------------------+
#define USBD_VID            0x00AA
#define USBD_PID            0xAAFF
#define USB_MANUFACTURER    "STMicroelectronics"
#define USB_PRODUCT         "STM32F072"

#define UF2_PRODUCT_NAME    USB_MANUFACTURER " " USB_PRODUCT
#define UF2_BOARD_ID        "STM32F072xB"
#define UF2_VOLUME_LABEL    "CherryUF2"
#define UF2_INDEX_URL       "https://www.st.com/en/microcontrollers-microprocessors/stm32f0-series.html"

// clang-format on
//--------------------------------------------------------------------+
// CLOCK
//--------------------------------------------------------------------+
static inline void clock_init(void) {
    RCC_OscInitTypeDef       RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef       RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit     = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSEState       = RCC_HSE_OFF;
    RCC_OscInitStruct.HSI48State     = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL6;
    RCC_OscInitStruct.PLL.PREDIV     = RCC_PREDIV_DIV1;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
}
#endif
