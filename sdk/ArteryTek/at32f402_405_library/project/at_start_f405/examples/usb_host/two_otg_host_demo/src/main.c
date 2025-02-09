/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f402_405_board.h"
#include "at32f402_405_clock.h"
#include "usb_conf.h"
#include "usb_core.h"
#include "usbh_int.h"
#include "usbh_user.h"
#include "usbh_msc_class.h"
#include "usbh_hid_class.h"
#include "ff.h"

/** @addtogroup AT32F405_periph_examples
  * @{
  */

/** @addtogroup 405_USB_host_msc USB_host_msc
  * @{
  */

/* usb global struct define */
otg_core_type otghs_core_struct;
otg_core_type otgfs_core_struct;
void usb_clock48m_select(usb_clk48_s clk_s);
void usb_gpio_config(void);
void usb_low_power_wakeup_config(void);

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  system_clock_config();

  at32_board_init();

  /* usb gpio config */
  usb_gpio_config();

#ifdef USB_LOW_POWER_WAKUP
  usb_low_power_wakeup_config();
#endif

  uart_print_init(115200);

  /* enable otg clock */
  crm_periph_clock_enable(OTGHS_CLOCK, TRUE);
  crm_periph_clock_enable(OTGFS_CLOCK, TRUE);

  /* select usb 48m clcok source */
  usb_clock48m_select(USB_CLK_HEXT);

  /* enable otg irq */
  nvic_irq_enable(OTGHS_IRQ, 0, 0);
  nvic_irq_enable(OTGFS_IRQ, 0, 0);

  /* init usb */
  usbh_init(&otghs_core_struct,
            USBHS_SPEED_CORE_ID,
            USBHS_ID,
            &uhost_msc_class_handler,
            &usbh_user_handle);
            
  usbh_init(&otgfs_core_struct,
            USBFS_SPEED_CORE_ID,
            USBFS_ID,
            &uhost_hid_class_handler,
            &usbh_user_handle);
  while(1)
  {
    usbh_loop_handler(&otghs_core_struct.host);
    usbh_loop_handler(&otgfs_core_struct.host);
  }
}

/**
  * @brief  usb 48M clock select
  * @param  clk_s:USB_CLK_HICK, USB_CLK_HEXT
  * @retval none
  */
void usb_clock48m_select(usb_clk48_s clk_s)
{
  if(clk_s == USB_CLK_HICK)
  {
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_HICK);

    /* enable the acc calibration ready interrupt */
    crm_periph_clock_enable(CRM_ACC_PERIPH_CLOCK, TRUE);

    /* update the c1\c2\c3 value */
    acc_write_c1(7980);
    acc_write_c2(8000);
    acc_write_c3(8020);

    /* open acc calibration */
    acc_calibration_mode_enable(ACC_CAL_HICKTRIM, TRUE);
  }
  else
  {
    crm_pllu_output_set(TRUE);
    /* wait till pll is ready */
    while(crm_flag_get(CRM_PLLU_STABLE_FLAG) != SET)
    {
    }
    crm_usb_clock_source_select(CRM_USB_CLOCK_SOURCE_PLLU);
  }
}

/**
  * @brief  this function config gpio.
  * @param  none
  * @retval none
  */
void usb_gpio_config(void)
{
  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  
  crm_periph_clock_enable(OTGHS_PIN_GPIO_CLOCK, TRUE);
  crm_periph_clock_enable(OTGFS_PIN_GPIO_CLOCK, TRUE);
#ifdef USB_SOF_OUTPUT_ENABLE
  crm_periph_clock_enable(OTGHS_PIN_SOF_GPIO_CLOCK, TRUE);
  gpio_init_struct.gpio_pins = OTGHS_PIN_SOF;
  gpio_init(OTGHS_PIN_SOF_GPIO, &gpio_init_struct);
  gpio_pin_mux_config(OTGHS_PIN_SOF_GPIO, OTGHS_PIN_SOF_SOURCE, OTGHS_PIN_MUX);
  
  crm_periph_clock_enable(OTGFS_PIN_SOF_GPIO_CLOCK, TRUE);
  gpio_init_struct.gpio_pins = OTGFS_PIN_SOF;
  gpio_init(OTGFS_PIN_SOF_GPIO, &gpio_init_struct);
  gpio_pin_mux_config(OTGFS_PIN_SOF_GPIO, OTGFS_PIN_SOF_SOURCE, OTGFS_PIN_MUX);
#endif

  /* use vbus pin */
#ifndef USB_VBUS_IGNORE
  gpio_init_struct.gpio_pins = OTGHS_PIN_VBUS;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  gpio_pin_mux_config(OTGHS_PIN_GPIO, OTGHS_PIN_VBUS_SOURCE, OTGHS_PIN_MUX);
  gpio_init(OTGHS_PIN_GPIO, &gpio_init_struct);
  
  gpio_init_struct.gpio_pins = OTGFS_PIN_VBUS;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  gpio_pin_mux_config(OTGFS_PIN_GPIO, OTGFS_PIN_VBUS_SOURCE, OTGFS_PIN_MUX);
  gpio_init(OTGFS_PIN_GPIO, &gpio_init_struct);
#endif

}
#ifdef USB_LOW_POWER_WAKUP
/**
  * @brief  usb low power wakeup interrupt config
  * @param  none
  * @retval none
  */
void usb_low_power_wakeup_config(void)
{
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
  exint_default_para_init(&exint_init_struct);

  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = OTGHS_WKUP_EXINT_LINE;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);
  nvic_irq_enable(OTGHS_WKUP_IRQ, 0, 0);
  
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = OTGFS_WKUP_EXINT_LINE;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);
  nvic_irq_enable(OTGFS_WKUP_IRQ, 0, 0);
}

/**
  * @brief  this function handles otg wakup interrupt.
  * @param  none
  * @retval none
  */
void OTGHS_WKUP_HANDLER(void)
{
  exint_flag_clear(OTGHS_WKUP_EXINT_LINE);
}

/**
  * @brief  this function handles otg wakup interrupt.
  * @param  none
  * @retval none
  */
void OTGFS_WKUP_HANDLER(void)
{
  exint_flag_clear(OTGFS_WKUP_EXINT_LINE);
}

#endif

/**
  * @brief  this function handles otgfs interrupt.
  * @param  none
  * @retval none
  */
void OTGHS_IRQ_HANDLER(void)
{
  usbh_irq_handler(&otghs_core_struct);
}


/**
  * @brief  this function handles otgfs interrupt.
  * @param  none
  * @retval none
  */
void OTGFS_IRQ_HANDLER(void)
{
  usbh_irq_handler(&otgfs_core_struct);
}

/**
  * @brief  usb delay millisecond function.
  * @param  ms: number of millisecond delay
  * @retval none
  */
void usb_delay_ms(uint32_t ms)
{
  /* user can define self delay function */
  delay_ms(ms);
}

/**
  * @brief  usb delay microsecond function.
  * @param  us: number of microsecond delay
  * @retval none
  */
void usb_delay_us(uint32_t us)
{
  delay_us(us);
}

/**
  * @}
  */

/**
  * @}
  */
