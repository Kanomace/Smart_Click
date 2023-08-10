#include "miio_define.h"
#include "miio_api.h"
#include "miio_uart.h"
#include "arch_dbg.h"

#include "user_app_func.h"

#define TAG		"user_app_func"

int app_func_get_time(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[RESULT_BUF_SIZE] = { 0 };
	memset(pbuf, 0, RESULT_BUF_SIZE);

	uart->send_str(uart, "time\r");
	uart->recv_str(uart, pbuf, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);

	LOG_INFO_TAG(TAG, "%s", pbuf);

	return 0;
}

int app_func_get_mac(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[RESULT_BUF_SIZE] = { 0 };
	memset(pbuf, 0, RESULT_BUF_SIZE);

	uart->send_str(uart, "mac\r");
	uart->recv_str(uart, pbuf, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);

	LOG_INFO_TAG(TAG, "%s", pbuf);

	return 0;
}


int app_func_get_net_state(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[RESULT_BUF_SIZE] = { 0 };
	memset(pbuf, 0, RESULT_BUF_SIZE);

	uart->send_str(uart, "net\r");
	uart->recv_str(uart, pbuf, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);

	LOG_INFO_TAG(TAG, "%s", pbuf);

	return 0;
}

int app_func_reboot(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[RESULT_BUF_SIZE] = { 0 };
	memset(pbuf, 0, RESULT_BUF_SIZE);

	uart->send_str(uart, "reboot\r");
	uart->recv_str(uart, pbuf, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);

	LOG_INFO_TAG(TAG, "%s", pbuf);

	return 0;
}

int app_func_restore(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[RESULT_BUF_SIZE] = { 0 };
	memset(pbuf, 0, RESULT_BUF_SIZE);

	uart->send_str(uart, "restore\r");
	uart->recv_str(uart, pbuf, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);

	LOG_INFO_TAG(TAG, "%s", pbuf);

	return 0;
}

int app_func_getwifi(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[RESULT_BUF_SIZE] = { 0 }; /* wifi name max length : 32(IEEEtypes_SSID_SIZE) */
	memset(pbuf, 0, RESULT_BUF_SIZE);

	uart->send_str(uart, "getwifi\r");
	uart->recv_str(uart, pbuf, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);

	LOG_INFO_TAG(TAG, "%s", pbuf);

	return 0;
}

int app_func_setwifi(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[RESULT_BUF_SIZE] = { 0 };
	memset(pbuf, 0, RESULT_BUF_SIZE);

	uart->send_str(uart, "setwifi "USER_SSID" "USER_PASSWD"\r");
	uart->recv_str(uart, pbuf, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);

	LOG_INFO_TAG(TAG, "%s", pbuf);

	return 0;
}

int app_func_get_version(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[RESULT_BUF_SIZE] = { 0 };
	memset(pbuf, 0, RESULT_BUF_SIZE);

	uart->send_str(uart, "version\r");
	uart->recv_str(uart, pbuf, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);

	LOG_INFO_TAG(TAG, "%s", pbuf);

	return 0;
}

int app_func_set_mcu_version(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[32] = { 0 };
	memset(pbuf, 0, 32);

	uart->send_str_wait_ack(uart, "mcu_version "USER_MCU_VERSION"\r");

	return 0;
}

int app_func_get_arch_platform(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[RESULT_BUF_SIZE] = { 0 };
	memset(pbuf, 0, RESULT_BUF_SIZE);

	uart->send_str(uart, "arch\r");
	uart->recv_str(uart, pbuf, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);

	LOG_INFO_TAG(TAG, "%s", pbuf);

	return 0;
}

int app_func_factory(void *argv)
{
	miio_t *miio = (miio_t *)argv;
	miio_uart_t *uart = &(miio->uart);

	unsigned char pbuf[RESULT_BUF_SIZE] = { 0 };
	memset(pbuf, 0, RESULT_BUF_SIZE);

	uart->send_str(uart, "factory\r");
	uart->recv_str(uart, pbuf, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);

	LOG_INFO_TAG(TAG, "%s", pbuf);

	return 0;
}

int app_func_net_change(void *argv)
{
	return 0;
}

bool app_func_is_ota_busy(void *argv)
{
	return false;
}
