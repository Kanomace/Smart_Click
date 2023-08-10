#include "miio_define.h"
#include "user_config.h"

#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)

#include "miio_api.h"
#include "arch_uart.h"
#include "miio_uart.h"
#include "xmodem.h"
#include "ota.h"

int execute_ota_operation(void *handle, const char *pbuf, int buf_sz, miio_fp_cmd_delegate_ack_t ack)
{
	int ret = MIIO_OK;

	miio_t *miio = (miio_t *)handle;
	miio_uart_t *uart = &(miio->uart);
	miio_xmodem_t *xmodem = &(miio->xmodem);

	/*
	 * |==============================================|
	 * | ------ Model ------- | ------- MCU --------- |
	 * |                    <--- 'get down\r'         |
	 * | 'down update_fw\r'  --->                     |
	 * |                    <--- 'result \"ready\"\r' |
	 * |              'ok\r' --->                     |
	 * |            <<< xmodem trans >>>              |
	 * |==============================================|
	 */
	if(0 > uart->send_str_wait_ack(uart, "result \"ready\"\r")) {
		return MIIO_ERROR;
	}

	if (MIIO_OK == xmodem_recv_data(xmodem)) {
		/*********************************/
		/* execute reboot operation here */
		/*********************************/
	} else {
		ret = MIIO_ERROR;
	}

	return ret;
}

#endif /* (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE) */

