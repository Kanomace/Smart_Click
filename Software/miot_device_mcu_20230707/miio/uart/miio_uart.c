#include "miio_define.h"
#include "arch_dbg.h"
#include "miio_uart.h"
#include "arch_uart.h"

#undef  TAG
#define TAG     "miio_uart"

miio_ringbuff_t ring_buff;

int miio_uart_instance_create(miio_uart_t* uart,
								void *uart_init,
								void * send_str,
								void * send_str_wait_ack,
								void * recv_str,
								void * xmodem_recv_str,
								void * send_byte,
								void * recv_byte)
{
	uart->uart_init			= (fp_uart_init)uart_init;
	uart->send_str			= (fp_uart_send_str)send_str;
	uart->recv_str			= (fp_uart_recv_str)recv_str;
	uart->send_str_wait_ack = (fp_uart_send_str_wait_ack)send_str_wait_ack;
	uart->recv_byte			= (fp_uart_recv_byte)recv_byte;
	uart->xmodem_recv_str           = (fp_xmodem_recv_str)xmodem_recv_str;
	uart->send_byte			= (fp_uart_send_byte)send_byte;

	arch_os_mutex_init(&(uart->write_mutex));
	arch_os_mutex_init(&(uart->read_mutex));

	if(MIIO_OK != uart->uart_init(uart))
		return MIIO_ERROR;

	return MIIO_OK;
}

uart_error_t miio_uart_destroy(miio_uart_t uart)
{
	// wait for tx buffer drain
	// tcdrain(uart.fd);
	// close(uart.fd);

	LOG_INFO_TAG(TAG, "miio uart destroyed");

	return UART_OK;
}


void _ringbuff_init(void)
{
	ring_buff.head = 0;
	ring_buff.tail = 0;
	ring_buff.lenght = 0;
}

bool _write_ringbuff(uint8_t data)
{
	if(ring_buff.lenght >= RINGBUFF_LEN)
	{
		return false;
	}
	ring_buff.ring_buff[ring_buff.tail]=data;
	//ring_buff.tail = (ring_buff.tail+1)%RINGBUFF_LEN;
	ring_buff.tail = LOOP_ADD(ring_buff.tail, 1, RINGBUFF_LEN);
	ring_buff.lenght++;
	return true;
}

bool _read_ringbuff(uint8_t *rData)
{
	if(ring_buff.lenght == 0)
	{
		return false;
	}
	_uart_interrupt(DISABLE);
	*rData = ring_buff.ring_buff[ring_buff.head];

	//ring_buff.head = (ring_buff.head+1)%RINGBUFF_LEN;
	ring_buff.head = LOOP_ADD(ring_buff.head, 1, RINGBUFF_LEN);
	ring_buff.lenght--;
	_uart_interrupt(ENABLE);
	return true;
}

uint16_t _get_ringbuff_len()
{
	return ring_buff.lenght;
}
