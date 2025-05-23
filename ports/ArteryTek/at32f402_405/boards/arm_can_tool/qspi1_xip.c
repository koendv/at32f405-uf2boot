#include "at32f402_405.h"
#include "at32f402_405_qspi.h"
#include "qspi1.h"

/*
 * after code in AT32F402_405_Firmware_Library/project/at_start_f405/examples/qspi/run_in_qspi_flash/
 *
 * assumption:
 * - qspi clock is already enabled
 * - qspi pins are already configured
 */

/**
 * @brief  en25qh128a cmd rsten config
 * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
 * @retval none
 */
void en25qh128a_cmd_rsten_config(qspi_cmd_type *qspi_cmd_struct) {
    qspi_cmd_struct->pe_mode_enable         = FALSE;
    qspi_cmd_struct->pe_mode_operate_code   = 0;
    qspi_cmd_struct->instruction_code       = 0x66;
    qspi_cmd_struct->instruction_length     = QSPI_CMD_INSLEN_1_BYTE;
    qspi_cmd_struct->address_code           = 0;
    qspi_cmd_struct->address_length         = QSPI_CMD_ADRLEN_0_BYTE;
    qspi_cmd_struct->data_counter           = 0;
    qspi_cmd_struct->second_dummy_cycle_num = 0;
    qspi_cmd_struct->operation_mode         = QSPI_OPERATE_MODE_111;
    qspi_cmd_struct->read_status_config     = QSPI_RSTSC_HW_AUTO;
    qspi_cmd_struct->read_status_enable     = FALSE;
    qspi_cmd_struct->write_data_enable      = TRUE;
}

/**
 * @brief  en25qh128a cmd rst config
 * @param  qspi_cmd_struct: the pointer for qspi_cmd_type parameter
 * @retval none
 */
void en25qh128a_cmd_rst_config(qspi_cmd_type *qspi_cmd_struct) {
    qspi_cmd_struct->pe_mode_enable         = FALSE;
    qspi_cmd_struct->pe_mode_operate_code   = 0;
    qspi_cmd_struct->instruction_code       = 0x99;
    qspi_cmd_struct->instruction_length     = QSPI_CMD_INSLEN_1_BYTE;
    qspi_cmd_struct->address_code           = 0;
    qspi_cmd_struct->address_length         = QSPI_CMD_ADRLEN_0_BYTE;
    qspi_cmd_struct->data_counter           = 0;
    qspi_cmd_struct->second_dummy_cycle_num = 0;
    qspi_cmd_struct->operation_mode         = QSPI_OPERATE_MODE_111;
    qspi_cmd_struct->read_status_config     = QSPI_RSTSC_HW_AUTO;
    qspi_cmd_struct->read_status_enable     = FALSE;
    qspi_cmd_struct->write_data_enable      = TRUE;
}

void en25qh128a_qspi_cmd_send(qspi_cmd_type *cmd) {
    qspi_cmd_operation_kick(QSPI1, cmd);
    /* wait command completed */
    while (qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET)
        ;
    qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}

void qspi1_flash_reset() {
    qspi_cmd_type en25qh128a_cmd_config;
    en25qh128a_cmd_rsten_config(&en25qh128a_cmd_config);
    en25qh128a_qspi_cmd_send(&en25qh128a_cmd_config);
    en25qh128a_cmd_rst_config(&en25qh128a_cmd_config);
    en25qh128a_qspi_cmd_send(&en25qh128a_cmd_config);
}

/**
 * @brief  en25qh128a xip init config
 * @param  qspi_xip_struct: the pointer for qspi_xip_type parameter
 * @retval none
 */
void en25qh128a_xip_init_config(qspi_xip_type *qspi_xip_struct) {
    qspi_xip_struct->read_instruction_code        = 0x6B;
    qspi_xip_struct->read_address_length          = QSPI_XIP_ADDRLEN_3_BYTE;
    qspi_xip_struct->read_operation_mode          = QSPI_OPERATE_MODE_114;
    qspi_xip_struct->read_second_dummy_cycle_num  = 8;
    qspi_xip_struct->write_instruction_code       = 0x32;
    qspi_xip_struct->write_address_length         = QSPI_XIP_ADDRLEN_3_BYTE;
    qspi_xip_struct->write_operation_mode         = QSPI_OPERATE_MODE_114;
    qspi_xip_struct->write_second_dummy_cycle_num = 0;
    qspi_xip_struct->write_select_mode            = QSPI_XIPW_SEL_MODED;
    qspi_xip_struct->write_time_counter           = 0x7F;
    qspi_xip_struct->write_data_counter           = 0x1F;
    qspi_xip_struct->read_select_mode             = QSPI_XIPR_SEL_MODED;
    qspi_xip_struct->read_time_counter            = 0x7F;
    qspi_xip_struct->read_data_counter            = 0x1F;
}

void en25qh128a_qspi_xip_init(void) {
    qspi_cmd_type en25qh128a_cmd_config;
    qspi_xip_type en25qh128a_xip_init;

    /* switch to command-port mode */
    qspi_xip_enable(QSPI1, FALSE);

    /* issue reset command */
    qspi1_flash_reset();

    /* initial xip */
    en25qh128a_xip_init_config(&en25qh128a_xip_init);
    qspi_xip_init(QSPI1, &en25qh128a_xip_init);
    qspi_xip_enable(QSPI1, TRUE);
}

void qspi1_xip_init(void) {
    /* switch to cmd port */
    qspi_xip_enable(QSPI1, FALSE);

    /* set sclk */
    qspi_clk_division_set(QSPI1, QSPI_CLK_DIV_2); // 108 MHz
    // qspi_clk_division_set(QSPI1, QSPI_CLK_DIV_4); // 54 MHz

    /* set sck idle mode 0 */
    qspi_sck_mode_set(QSPI1, QSPI_SCK_MODE_0);

    /* set wip in bit 0 */
    qspi_busy_config(QSPI1, QSPI_BUSY_OFFSET_0);

    /* enable auto ispc */
    qspi_auto_ispc_enable(QSPI1);

    /* configure xip mode, set qspi_xip_enable TRUE */
    en25qh128a_qspi_xip_init();

    /* check the led toggle in qspi */
    // XXX qspi_run();

    return;
}
