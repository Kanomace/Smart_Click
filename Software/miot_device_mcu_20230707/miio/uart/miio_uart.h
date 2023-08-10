/**
 * @author  MIoT
 * @date    2019
 * @par     Copyright (c):
 *
 *    Copyright 2019 MIoT,MI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef __MIIO_UART_H__
#define __MIIO_UART_H__

#include "arch_os.h"
#define LOOP_ADD(des,step_len,max_len) ((des + step_len) >= max_len) ? ((des +step_len) - max_len) : (des + step_len)

typedef enum _uart_error_t {
    UART_OK = 0,
    UART_DESTROY_ERROR	= -1,
    UART_OPEN_ERROR		= -2,
    UART_SET_ARRT_ERROR	= -3,
    UART_SEND_ERROR		= -4,
    UART_RECV_ACK_ERROR	= -5,
    UART_RECV_ERROR		= -6,
} uart_error_t;

typedef int (*fp_uart_init)(void *uart);
typedef int (*fp_uart_send_str)(void *uart, const char *str);
typedef int (*fp_uart_send_byte)(void *uart, const char c);
typedef int (*fp_uart_send_str_wait_ack)(void *uart, const char *str);
typedef int (*fp_uart_recv_str)(void *uart, uint8_t *str, int len, uint32_t timeout_ms);
typedef int (*fp_xmodem_recv_str)(void *uart, uint8_t *str, uint32_t timeout_ms);
typedef int (*fp_uart_recv_byte)(void *uart, uint8_t *c, uint32_t timeout_ms);


void _ringbuff_init(void);
bool _write_ringbuff(uint8_t data);
bool _read_ringbuff(uint8_t *rData);
uint16_t _get_ringbuff_len();



typedef struct _miio_uart_t {
	struct
	{
		uint32_t baud_rate;
		uint8_t data_bits;
		uint8_t parity;
		uint8_t stop_bits;
		uint32_t tx_buf_size;
		uint32_t rx_buf_size;
	} params;

	arch_os_mutex write_mutex;
	arch_os_mutex read_mutex;

	int fd;

	fp_uart_init uart_init;
	fp_uart_send_str send_str;
	fp_uart_send_byte send_byte;
	fp_uart_recv_str recv_str;
	fp_xmodem_recv_str xmodem_recv_str;
	fp_uart_recv_byte recv_byte;
	fp_uart_send_str_wait_ack send_str_wait_ack;

} miio_uart_t;

int miio_uart_instance_create(miio_uart_t* uart, void *uart_init,
                     void * send_str,
                     void * send_str_wait_ack,
                     void * recv_str,
		     void * xmodem_recv_str,
                     void * send_byte,
                     void * recv_byte);
uart_error_t miio_uart_destroy(miio_uart_t uart);
typedef struct
{
		uint16_t head;
		uint16_t tail;
		uint16_t lenght;
		uint8_t ring_buff[RINGBUFF_LEN];
}miio_ringbuff_t;

#endif
