#include "qspi1.h"
#include "board.h"

#define QSPI_FIFO_DEPTH (32 * 4)

qspi_cmd_type qspi_flash_config;

void qspi_flash_read_config(qspi_cmd_type *qspi_cmd_struct) {
    qspi_cmd_struct->pe_mode_enable         = FALSE;
    qspi_cmd_struct->pe_mode_operate_code   = 0;
    qspi_cmd_struct->instruction_code       = 0x0;
    qspi_cmd_struct->instruction_length     = QSPI_CMD_INSLEN_1_BYTE;
    qspi_cmd_struct->address_code           = 0x0;
    qspi_cmd_struct->address_length         = QSPI_CMD_ADRLEN_3_BYTE;
    qspi_cmd_struct->data_counter           = 0;
    qspi_cmd_struct->second_dummy_cycle_num = 0;
    qspi_cmd_struct->operation_mode         = QSPI_OPERATE_MODE_111;
    qspi_cmd_struct->read_status_config     = QSPI_RSTSC_HW_AUTO;
    qspi_cmd_struct->read_status_enable     = FALSE;
    qspi_cmd_struct->write_data_enable      = FALSE;
}

void qspi_flash_write_config(qspi_cmd_type *qspi_cmd_struct) {
    qspi_cmd_struct->pe_mode_enable         = FALSE;
    qspi_cmd_struct->pe_mode_operate_code   = 0;
    qspi_cmd_struct->instruction_code       = 0x0;
    qspi_cmd_struct->instruction_length     = QSPI_CMD_INSLEN_1_BYTE;
    qspi_cmd_struct->address_code           = 0x0;
    qspi_cmd_struct->address_length         = QSPI_CMD_ADRLEN_3_BYTE;
    qspi_cmd_struct->data_counter           = 0;
    qspi_cmd_struct->second_dummy_cycle_num = 0;
    qspi_cmd_struct->operation_mode         = QSPI_OPERATE_MODE_111;
    qspi_cmd_struct->read_status_config     = QSPI_RSTSC_HW_AUTO;
    qspi_cmd_struct->read_status_enable     = FALSE;
    qspi_cmd_struct->write_data_enable      = TRUE;
}

/* qspi send, then receive */
uint32_t qspi1_send_then_recv(const uint8_t *send_buf, uint32_t send_length, uint8_t *recv_buf, uint32_t recv_length) {
    unsigned char *ptr    = send_buf;
    uint32_t       count  = 0;
    uint32_t       result = 0;
    uint32_t       len    = 0;
    uint32_t       i      = 0;

    qspi_flash_read_config(&qspi_flash_config);

    qspi_flash_config.instruction_code = ptr[0];
    count++;

    /* get address */
    if (send_length > 1) {
        if (BOARD_FLASH_SIZE > 0x1000000 && send_length >= 5) {
            /* medium size greater than 16Mb, address size is 4 Byte */
            qspi_flash_config.address_code   = (ptr[1] << 24) | (ptr[2] << 16) | (ptr[3] << 8) | (ptr[4]);
            qspi_flash_config.address_length = QSPI_CMD_ADRLEN_4_BYTE;
            count += 4;
        } else if (send_length >= 4) {
            /* address size is 3 Byte */
            qspi_flash_config.address_code   = (ptr[1] << 16) | (ptr[2] << 8) | (ptr[3]);
            qspi_flash_config.address_length = QSPI_CMD_ADRLEN_3_BYTE;
            count += 3;
        } else {
            return 0;
        }
    } else {
        /* no address stage */
        qspi_flash_config.address_code   = 0;
        qspi_flash_config.address_length = QSPI_CMD_ADRLEN_0_BYTE;
    }

    /* set dummy cycles */
    if (count != send_length) {
        qspi_flash_config.second_dummy_cycle_num = (send_length - count) * 8;
    } else {
        qspi_flash_config.second_dummy_cycle_num = 0;
    }

    /* set receive size */
    qspi_flash_config.data_counter = recv_length;

    /* switch to command-port mode */
    qspi_xip_enable(QSPI1, FALSE);

    /* execute command */
    qspi_cmd_operation_kick(QSPI1, &qspi_flash_config);

    /* read data */
    do {
        if (recv_length >= QSPI_FIFO_DEPTH) {
            len = QSPI_FIFO_DEPTH;
        } else {
            len = recv_length;
        }
        while (qspi_flag_get(QSPI1, QSPI_RXFIFORDY_FLAG) == RESET) {
        }
        for (i = 0; i < len; ++i) {
            *recv_buf++ = qspi_byte_read(QSPI1);
        }
        recv_length -= len;
    } while (recv_length);

    /* wait command completed */
    while (qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET) {
    }
    qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);

    /* switch back to xip */
    qspi_xip_enable(QSPI1, TRUE);

    return 1;
}

/* qspi send only */
uint32_t qspi1_send(const uint8_t *ptr, uint32_t send_length) {
    uint32_t count = 0;
    uint32_t len   = 0;
    uint32_t i     = 0;

    qspi_flash_write_config(&qspi_flash_config);

    qspi_flash_config.instruction_code = ptr[0];
    count++;

    /* get address */
    if (send_length > 1) {
        if (BOARD_FLASH_SIZE > 0x1000000 && send_length >= 5) {
            /* medium size greater than 16Mb, address size is 4 Byte */
            qspi_flash_config.address_code   = (ptr[1] << 24) | (ptr[2] << 16) | (ptr[3] << 8) | (ptr[4]);
            qspi_flash_config.address_length = QSPI_CMD_ADRLEN_4_BYTE;
            count += 4;
        } else if (send_length >= 4) {
            /* address size is 3 Byte */
            qspi_flash_config.address_code   = (ptr[1] << 16) | (ptr[2] << 8) | (ptr[3]);
            qspi_flash_config.address_length = QSPI_CMD_ADRLEN_3_BYTE;
            count += 3;
        } else {
            return 0;
        }
    } else {
        /* no address stage */
        qspi_flash_config.address_code   = 0;
        qspi_flash_config.address_length = QSPI_CMD_ADRLEN_0_BYTE;
    }

    /* set send cycles */
    if (count != send_length) {
        len = send_length - count;
    } else {
        len = 0;
    }
    qspi_flash_config.data_counter = len;
    uint8_t *buf                   = ptr + count;

    /* switch to command-port mode */
    qspi_xip_enable(QSPI1, FALSE);

    /* execute command */
    qspi_cmd_operation_kick(QSPI1, &qspi_flash_config);

    /* send up to 256 bytes at one time, and only one page */
    for (i = 0; i < len; ++i) {
        while (qspi_flag_get(QSPI1, QSPI_TXFIFORDY_FLAG) == RESET) {
        }
        qspi_byte_write(QSPI1, *buf++);
    }

    /* wait command completed */
    while (qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET) {
    }
    qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);

    /* switch back to xip */
    qspi_xip_enable(QSPI1, TRUE);

    return 1;
}
