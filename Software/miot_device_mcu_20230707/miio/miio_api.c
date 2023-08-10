#include "miio_define.h"

#include "miio_uart.h"
#include "arch_uart.h"
#include "arch_dbg.h"
#include "list.h"
#include "util.h"
#include "xmodem.h"
#include "uart_command.h"
#include "uart_command_decoder.h"
#include "iid.h"
#include "miio_api.h"

#include "arch_os.h"
#include "user_app_main.h"

#undef	TAG
#define TAG "miio_api"

/* ======================== Private functions ================================ */
static int _miio_cmd_ack(void *handle, const char *pbuf, size_t buf_sz)
{
	int ret = MIIO_OK;

	if (NULL == handle || NULL == pbuf || buf_sz <= 0) {
		LOG_ERROR_TAG(TAG, "handle or pbuf is null");
		return MIIO_ERROR;
	}

	miio_t *miio = (miio_t *)handle;
	miio_uart_t *u = &(miio->uart);

	int n_send = u->send_str_wait_ack(u, pbuf);

	if (n_send < 0 ) 
		return MIIO_ERROR;

	if (n_send < buf_sz) {
		LOG_ERROR("uart send result failed");
		ret = MIIO_ERROR;
	}
	return ret;
}

static miio_cmd_t *miio_command_find_by_method(miio_t *miio, char *method)
{
	struct list_head *pos;
	miio_cmd_t *tmp = NULL;
	int checkflag = 0;

	if (NULL == miio || NULL == method) { return NULL; }

	arch_os_mutex_get(&miio->cmd.mutex);
	list_for_each(pos, &miio->cmd.list) {
		tmp = list_entry(pos, miio_cmd_t, list);
		if (!strcmp(tmp->method, method)) {
			checkflag = 1;
			break;
		}
	}
	arch_os_mutex_put(&miio->cmd.mutex);

	if (checkflag == 1) { return tmp; }

	return NULL;
}

static int uart_command_config_arg(miio_cmd_delegate_arg_t *arg, void *pload, size_t pload_len)
{
	if (arg == NULL || NULL == pload){
		return MIIO_ERROR_PARAM;
	}

	arg->pload = pload;
	arg->pload_len = pload_len;

	return MIIO_OK;
}


/* ======================== Public functions ================================ */

miio_handle_t miio_instance_create(void)
{
	miio_t *handle = malloc(sizeof(miio_t));

	if (NULL == handle) {
		LOG_ERROR_TAG(TAG, "miio instance malloc failed");
		return NULL;
	}

	memset(handle, 0, sizeof(miio_t));

	arch_os_mutex_init(&handle->cmd.mutex);
	INIT_LIST_HEAD(&handle->cmd.list);

	return (miio_handle_t)handle;
}

int miio_instance_add_uart(miio_handle_t handle)
{
	int ret = MIIO_OK;
	miio_t *miio = (miio_t *)handle;

	/* create uart instance */
	ret = miio_uart_instance_create(&miio->uart,
									(void *)_uart_init,
									(void *)_uart_send_str,
									(void *)_uart_send_str_wait_ack,
									(void *)_uart_recv_str,
									(void *)_Xmodem_recv_str,
									(void *)_uart_send_byte,
									(void *)_uart_recv_byte);

	return ret;
}

#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)
int miio_instance_add_xmodem(miio_handle_t handle)
{
	int ret = MIIO_OK;
	miio_t *miio = (miio_t *)handle;

	/* create xmodem instance */
	ret = miio_xmodem_create_instance(&(miio->xmodem), &(miio->uart));

	return ret;
}
#endif

int miio_cmd_method_register(miio_handle_t handle, char *method, miio_fp_cmd_handle_t cb)
{
	miio_t *miio = (miio_t *)handle;

	if (NULL == miio || NULL == method || NULL == cb) {
		return MIIO_ERROR;
	}

	miio_cmd_t *cmd = malloc(sizeof(miio_cmd_t));
	if (NULL == cmd) {
		LOG_ERROR_TAG(TAG, "miio command malloc failed");
		return MIIO_ERROR;
	}

	strncpy(cmd->method, method, CMD_METHOD_LEN_MAX - 1);
	cmd->cb = cb;

	arch_os_mutex_get(&miio->cmd.mutex);
	list_add(&cmd->list, &miio->cmd.list);
	arch_os_mutex_put(&miio->cmd.mutex);

	return MIIO_OK;
}

int do_req_error(void *handle, char* req_arg, miio_fp_cmd_delegate_ack_t ack)
{
	do {
		int ret = 0;
		ret = send_error_code(handle, (const char *)req_arg, strlen(req_arg), ack);
		if (ret != 0) {
			/* add error solution here */
			break;
		}
	} while (false);

	return MIIO_OK;
}

int send_error(miio_handle_t handle, char *message, int errcode)
{
	char send_msg[RESULT_BUF_SIZE] = { 0 };
	memset(send_msg, 0, RESULT_BUF_SIZE);
	snprintf(send_msg, RESULT_BUF_SIZE, "\"%s\" %d", message, errcode);
	do_req_error(handle, send_msg, _miio_cmd_ack);
	return MIIO_OK;
}
int miio_command_handle_loop(miio_handle_t handle, char *pbuf, int buf_sz)
{
	int ret = MIIO_OK;

	if (NULL == handle || NULL == pbuf || buf_sz <= 0) {
		ret = MIIO_ERROR_PARAM;
		goto safe_exit;
	}

	miio_t *miio	= (miio_t *)handle;
	miio_uart_t* u	= &(miio->uart);
	miio_cmd_t* c	= &(miio->cmd);

	/* init to start command handle loop */
	c->loop_flag = 1;
	char method[CMD_METHOD_LEN_MAX];
	unsigned int method_len = sizeof(method);
	miio_cmd_delegate_arg_t arg;

	{/* report model&mcu_version before loop */
		int result = 0;
		do {
			result = MIIO_OK;

			result |= ((UART_RECV_ACK_ERROR == u->send_str_wait_ack(u, "echo off\r")) ? MIIO_ERROR : MIIO_OK);
			result |= ((UART_RECV_ACK_ERROR == u->send_str_wait_ack(u, "model "USER_MODEL"\r")) ? MIIO_ERROR : MIIO_OK);
			result |= ((UART_RECV_ACK_ERROR == u->send_str_wait_ack(u, "mcu_version "USER_MCU_VERSION"\r")) ? MIIO_ERROR : MIIO_OK);
			if (HAVE_BLE)
				result |= ((UART_RECV_ACK_ERROR == u->send_str_wait_ack(u, "ble_config set "BLE_PID" "USER_MCU_VERSION"\r")) ? MIIO_ERROR : MIIO_OK);

			arch_msleep(USER_POLL_INTERVAL_MS);
		} while (result != MIIO_OK);	/* !!! report until Wi-Fi model respond */
	}

	while (1 == c->loop_flag) {
		arch_msleep(USER_POLL_INTERVAL_MS);

		/* execute user functions here */
		user_app_main(1, handle);

		/* clear command string buffer */
		memset(pbuf, 0, buf_sz);
		memset(method, 0, CMD_METHOD_LEN_MAX);

		u->send_str(u, "get_down\r");

		int recv_bytes = u->recv_str(u, (unsigned char *)pbuf, buf_sz, USER_UART_TIMEOUT_MS);
		if (recv_bytes <= 0) { /* judge if received string */
			LOG_INFO_TAG(TAG, "uart connected error or module rebooting...");
			continue;
		}
		if (pbuf[recv_bytes - 1] != '\r') { /* judge if string received end with '\r' */
			LOG_INFO_TAG(TAG, "uart recv error[%s]", pbuf);
			ret = MIIO_ERROR_PARAM;
			continue;
		}

		ret = uart_comamnd_decoder(pbuf, recv_bytes, method, &method_len);
		if (MIIO_OK != ret) { /* judge if string decoded correctly */
			LOG_INFO_TAG(TAG, "get method failed[%s]", pbuf);
			ret = MIIO_ERROR_PARAM;
			continue;
		}

		if (method_len > 0 && method != NULL) { /* start to find if method contained */
			miio_cmd_t *cmd = miio_command_find_by_method(miio, method);
			if (NULL == cmd) {
				if (strcmp(ERROR_STRING, method) && strcmp(OK_STRING, method))
					send_error(miio, ERROR_MESSAGE_UNCMD, ERROR_CODE_UNCMD);
			} else {
				LOG_INFO_TAG(TAG, "found method : %s", method);
				uart_command_config_arg(&arg, pbuf, recv_bytes); /* assembly arg to passed into callback func */
				cmd->cb(handle, &arg, _miio_cmd_ack);	/* execute callback func contained in method finded */
			}
			continue;
		} else {
			ret = MIIO_ERROR_PARAM;
			LOG_INFO_TAG(TAG, "unknown command : %s", (char *)pbuf);
			continue;
		}
	}

safe_exit:
	return ret;
}

void miio_command_handle_leave(miio_handle_t handle)
{
	if (NULL == handle) {
		LOG_INFO_TAG(TAG, "miio handle is NULL");
		return;
	}

	miio_t *miio = (miio_t *)handle;
	miio_cmd_t * c = &(miio->cmd);
	c->loop_flag = 0;
	return;
}

void miio_instance_destroy(miio_handle_t handle)
{
	miio_cmd_t *tmp;
	list_head_t *pos, *q;

	if (NULL == handle) {
		LOG_INFO_TAG(TAG, "create miio handle instance first");
		return;
	}

	miio_t *miio = (miio_t *)handle;
	arch_os_mutex_get(&miio->cmd.mutex);
	do {
		miio_uart_t* uart = &(miio->uart);
		miio_uart_destroy(*uart);

		list_for_each_safe(pos, q, &miio->cmd.list) {
			tmp = list_entry(pos, miio_cmd_t, list);
			list_del(pos);
			free(tmp);
			tmp = NULL;
		}
	} while (false);
	arch_os_mutex_put(&miio->cmd.mutex);
	arch_os_mutex_deinit(&miio->cmd.mutex);

	LOG_INFO_TAG(TAG, "miio instance destroyed");

	free(miio);

	return;
}
