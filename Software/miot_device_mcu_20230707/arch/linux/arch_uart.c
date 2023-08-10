#include "miio_define.h"
#include "arch_dbg.h"
#include "miio_uart.h"
#include "arch_uart.h"

#undef	MIIO_LOG_TAG
#define MIIO_LOG_TAG			"arch_uart"

uart_error_t _uart_init(miio_uart_t *uart)
{
	/* the following is an example for linux platform */
	/* user should change below for each mcu platform */
	/* adjust start */
	do {
		char* dev[] = { "/dev/ttyUSB0", "/dev/ttyS1", "/dev/ttyS2" };
#ifndef UART_NUM
#define UART_NUM	0
#endif
		int fd = open(dev[UART_NUM], O_RDWR | O_NOCTTY);
		if (fd < 0) {
			LOG_ERROR_TAG(MIIO_LOG_TAG, "uart open failed");
			return UART_OPEN_ERROR;
		}

		struct termios opt;
		tcgetattr(fd, &opt);

		/* set input&output baud rate*/
		cfsetispeed(&opt, B115200);
		cfsetospeed(&opt, B115200);
		/* set data bits */
		opt.c_cflag &= ~CSIZE;
		opt.c_cflag |= CS8;
		/* set stop bits */
		opt.c_cflag &= ~CSTOPB;
		/* set parity check */
		opt.c_cflag &= ~PARENB;
		opt.c_iflag &= ~INPCK;
		opt.c_cflag &= ~CRTSCTS;

		/* shield unique control character in linux uart */
		do {
			opt.c_cflag |= (CLOCAL | CREAD);
			opt.c_lflag &= ~(ICANON | ISIG | ECHO | ECHOE);
			opt.c_iflag &= ~(INLCR | ICRNL | IGNCR | BRKINT | ICRNL | INPCK | ISTRIP | IXON | IXOFF);
			opt.c_lflag |= NOFLSH;
		} while (false);

		opt.c_cc[VTIME] = USER_UART_TIMEOUT_MS / 100;
#ifndef UART_RECV_MIN
#define UART_RECV_MIN	0
#endif
		opt.c_cc[VMIN] = UART_RECV_MIN;

		if (tcsetattr(fd, TCSAFLUSH, &opt) < 0) {
			LOG_ERROR_TAG(MIIO_LOG_TAG, "set uart attr failed");
			return UART_SET_ARRT_ERROR;
		}

		uart->fd = fd;
	}while(false);
	/* adjust end */

	uart->params.baud_rate = 115200;
	uart->params.data_bits = 8;
	uart->params.parity = 0;
	uart->params.stop_bits = 1;

	return UART_OK;
}

int _uart_send_str(miio_uart_t *uart, const char* str)
{
	int len = strlen(str);
	if (len <= 0) { return UART_OK; }

	arch_os_mutex_get(&(uart->write_mutex));
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */
	/* adjust start */
	int n_send = write(uart->fd, str, strlen(str));
	/* adjust end*/
	arch_os_mutex_put(&(uart->write_mutex));

	if (n_send < len) {
		LOG_INFO_TAG(MIIO_LOG_TAG, "send string failed");
		return UART_SEND_ERROR;
	}

#if PRINT_SEND_BUFF
	LOG_INFO_TAG(MIIO_LOG_TAG, "send string : %s", str);
#endif

	return n_send;
}

int _uart_send_byte(miio_uart_t *uart, const char c)
{
	arch_os_mutex_get(&(uart->write_mutex));
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */
	/* adjust start */
	int n_send = write(uart->fd, &c, 1);
	/* adjust end */
	arch_os_mutex_put(&(uart->write_mutex));

	if (n_send < 1) {
		LOG_INFO_TAG(MIIO_LOG_TAG, "send byte failed : %x[hex]", c);
		return UART_SEND_ERROR;
	}
	
	LOG_INFO_TAG(MIIO_LOG_TAG, "send byte : %x[hex]", c);
	return n_send;
}


int _uart_send_str_wait_ack(miio_uart_t *uart, const char* str)
{
	int len = strlen(str);
	if (len <= 0) { return UART_OK; }


	uint8_t ack_buf[ACK_BUF_SIZE] = { 0 };
	memset(ack_buf, 0, ACK_BUF_SIZE);

	arch_os_mutex_get(&(uart->write_mutex));
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */
	/* adjust start */
	int n_send = write(uart->fd, str, strlen(str));
	/* adjust end */
	arch_os_mutex_put(&(uart->write_mutex));

	if (n_send < len) {
		LOG_INFO_TAG(MIIO_LOG_TAG, "send string wait ack failed 1");
		return UART_SEND_ERROR;
	}
		
#if PRINT_SEND_BUFF
	LOG_INFO_TAG(MIIO_LOG_TAG, "send string : %s", str);
#endif
	uart->recv_str(uart, ack_buf, ACK_BUF_SIZE, USER_UART_TIMEOUT_MS);
	if (0 != strncmp((const char*)ack_buf, "ok", strlen("ok"))) {
		LOG_INFO_TAG(MIIO_LOG_TAG, "send string wait ack failed 2 str=%s\n", ack_buf);
		return UART_RECV_ACK_ERROR;
	}
	
	return n_send;
}

int _uart_recv_str(miio_uart_t *uart, uint8_t* buf, int buf_len, uint32_t timeout_ms)
{
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */
	/* adjust start */
	struct termios opt;	/* set uart timeout */
	uint16_t retry;
	tcgetattr(uart->fd, &opt);
	opt.c_cc[VTIME] = timeout_ms / 100;
	tcsetattr(uart->fd, TCSADRAIN, &opt);
	/* adjust end */
	arch_os_mutex_get(&(uart->read_mutex));
	/* adjust start */
	int n_read = read(uart->fd, buf, buf_len);
	retry = 0;
	while(buf[n_read>0?(n_read - 1):0] != 0x0d && retry < USER_RECEIVE_RETRY) {
		
		if (n_read >= buf_len) {
			LOG_INFO_TAG(MIIO_LOG_TAG,"out of buffer %d %d retry=%d\n", buf_len, n_read, retry);
			memset(buf,0,buf_len);
			n_read = 0;
			retry = 0;
			//goto error;
		}
		n_read = n_read + read(uart->fd, buf + n_read, buf_len - n_read);
		retry++;
	}
	
	buf[n_read] = '\0';
	/* adjust end */
	arch_os_mutex_put(&(uart->read_mutex));
#if PRINT_RECV_BUFF
	if(n_read > 0) {
		LOG_INFO_TAG(MIIO_LOG_TAG, "recv string : %s", buf);
	} else {
		LOG_INFO_TAG(MIIO_LOG_TAG, "recv string : null");
	}
#endif

	return n_read;
}

int _Xmodem_recv_str(miio_uart_t *uart, uint8_t* buf, uint32_t timeout_ms)
{
        /* the following is an example for linux platform */
        /* user should adjust below for each mcu platform */
        /* adjust start */
        struct termios opt;     /* set uart timeout */
        tcgetattr(uart->fd, &opt);
        opt.c_cc[VTIME] = timeout_ms / 100;// / 100;
        tcsetattr(uart->fd, TCSADRAIN, &opt);
        /* adjust end */
        arch_os_mutex_get(&(uart->read_mutex));
        /* adjust start */
        int n_read = read(uart->fd, buf, USER_UART_RXBUF_SIZE);
        buf[n_read] = '\0';
        /* adjust end */
        arch_os_mutex_put(&(uart->read_mutex));

        return n_read;
}

int _uart_recv_byte(miio_uart_t *uart, uint8_t* c, uint32_t timeout_ms)
{
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */
	/* adjust start */
	struct termios opt;	/* set uart timeout */
	tcgetattr(uart->fd, &opt);
	opt.c_cc[VTIME] = timeout_ms / 100;
	tcsetattr(uart->fd, TCSADRAIN, &opt);
	/* adjust end */

	arch_os_mutex_get(&(uart->read_mutex));
	/* adjust start */
	int n_read = read(uart->fd, c, 1);
	/* adjust end */
	arch_os_mutex_put(&(uart->read_mutex));

	return n_read;
}

void _uart_deinit(miio_uart_t *uart)
{
	/* the following is an example for linux platform */
	/* user should adjust below for each mcu platform */
	/* adjust start */
	close(uart->fd);
	/* adjust end */
	return;
}

void _uart_interrupt(uint8_t enable)
{

}
