#ifndef __QSPI1_H__
#define __QSPI1_H__

#include <stdint.h>

/* configure qspi1 pins, init qspi1 */
void qspi1_init(void);

/* reset flash connected to qspi1 */
void qspi1_flash_reset(void);

/* qspi send/receive, needed by sfud */
uint32_t qspi1_send_then_recv(const uint8_t *send_buf, uint32_t send_length, uint8_t *recv_buf, uint32_t recv_length);
uint32_t qspi1_send(const uint8_t *send_buf, uint32_t length);

/* memory-map external flash to 0x90000000 */
void qspi1_xip_init(void);

#endif
