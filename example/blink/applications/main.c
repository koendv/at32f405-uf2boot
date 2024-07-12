/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-18     shelton      first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "drv_gpio.h"

/* defined the led2 pin: pf4 */
//#define LED0_PIN    GET_PIN(F, 4)

/* defined the led2 pin: pc8 */
#define LED0_PIN    GET_PIN(C, 8)

int main(void)
{
    rt_uint32_t speed = 500;
    /* set led pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(speed);
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(speed);
    }
}
