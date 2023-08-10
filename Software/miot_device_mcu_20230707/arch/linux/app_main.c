#include "miio_define.h"

#if (defined(USER_SIG_FUNC_ENABLE) && USER_SIG_FUNC_ENABLE)
#include <signal.h>
#endif

#include "arch_dbg.h"
#include "arch_uart.h"
#include "arch_init.h"
#include "miio_api.h"
#include "operation_executor.h"
#include "miio_uart.h"
#include "user_app_func.h"

#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)
#include "xmodem.h"
#include "ota.h"
#endif

#undef	TAG
#define TAG "miio_main"

miio_handle_t miio_handle = NULL;

#if (defined(USER_SIG_FUNC_ENABLE) && USER_SIG_FUNC_ENABLE)
void signal_handle(int signum)
{
	LOG_INFO_TAG(TAG, "get exit signal");

	miio_command_handle_leave(miio_handle);

	/* delete miio instance */
	miio_instance_destroy(miio_handle);

	exit(0);
}
#endif

int do_get_properties(void *handle, miio_cmd_delegate_arg_t *arg, miio_fp_cmd_delegate_ack_t ack)
{
	LOG_INFO_TAG(TAG, "==================== get_properties ====================");

	do {
		int ret = 0;

		ret = execute_property_operation(handle, (const char *)arg->pload, arg->pload_len, PROPERTY_OPERATION_GET, ack);
		if (ret != 0) {
			/* add error solution here */
			break;
		}
	} while (false);

	return MIIO_OK;
}

int do_set_properties(void *handle, miio_cmd_delegate_arg_t *req_arg, miio_fp_cmd_delegate_ack_t ack)
{
	LOG_INFO_TAG(TAG, "==================== set_properties ====================");

	do {
		int ret = 0;

		ret = execute_property_operation(handle, (const char *)req_arg->pload, req_arg->pload_len, PROPERTY_OPERATION_SET, ack);
		if (ret != 0) {
			/* add error solution here */
			break;
		}
	} while (false);

	return MIIO_OK;
}



int do_invoke_action(void *handle, miio_cmd_delegate_arg_t *req_arg, miio_fp_cmd_delegate_ack_t ack)
{
	LOG_INFO_TAG(TAG, "======================== action =========================");

	do {
		int ret = 0;

		ret = execute_action_invocation(handle, (const char *)req_arg->pload, req_arg->pload_len, ack);
		if (ret != 0) {
			// TODO : action error handle
			break;
		}
	} while (false);

	return MIIO_OK;
}

int do_invoke_none(void *handle, miio_cmd_delegate_arg_t *req_arg, miio_fp_cmd_delegate_ack_t ack)
{
	LOG_INFO_TAG(TAG, "========================= none ==========================");
	do {
	} while (false);

	return MIIO_OK;
}


int do_invoke_ota(void *handle, miio_cmd_delegate_arg_t *arg, miio_fp_cmd_delegate_ack_t ack)
{
	LOG_INFO_TAG(TAG, "========================== ota ==========================");
	do {
		int ret = MIIO_OK;

#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)
		bool is_ota_busy = false;
		is_ota_busy = app_func_is_ota_busy(handle);
		if(is_ota_busy) {
#if 0
			miio_t *miio = (miio_t *)handle;
			miio_uart_t *u = &(miio->uart);
			u->send_str_wait_ack(u, "result \"busy\"\r");
#endif
			break;
		}
		ret = execute_ota_operation(handle, (const char *)arg->pload, arg->pload_len, ack);
#else
		LOG_ERROR_TAG(TAG, "no ota handle function");
#if 0
		miio_t *miio = (miio_t *)handle;
		miio_uart_t *u = &(miio->uart);
		u->send_str_wait_ack(u, "result \"busy\"\r");
#endif
		ret = MIIO_ERROR;
#endif
		if (ret != 0) {
			/* user error solution here */
			break;
		}
	} while (false);

	return MIIO_OK;
}

int do_net_changed(void *handle, miio_cmd_delegate_arg_t *arg, miio_fp_cmd_delegate_ack_t ack)
{
	char *strtmp = strchr(arg->pload, ' ');
	if(NULL == strtmp) {
		return MIIO_ERROR;
	} else {
		LOG_INFO_TAG(TAG, "%s", ++strtmp);
	}

	app_func_net_change(arg->pload);

	return MIIO_OK;
}

/* !!! user may need to change this entry name to suit platform */
/* E.g: change this main function to "void app_main()" */
int main(int argc, char *argv[])
{
	int ret = 0;
	char pbuf[CMD_STR_MAX_LEN] = {0};
	arch_mcu_init();
	
	memset(pbuf, 0, CMD_STR_MAX_LEN);

	/* create miio intance */
	miio_handle = miio_instance_create();
	if (NULL == miio_handle) {
		LOG_ERROR_TAG(TAG, "create miio instance failed");
		goto safe_exit;
	}

	/* create uart instance */
	ret = miio_instance_add_uart(miio_handle);
	if (ret < 0) {
		LOG_ERROR_TAG(TAG, "create uart instance failed");
		goto safe_exit;
	}

#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)
	/* create xmodem instance */
	ret = miio_instance_add_xmodem(miio_handle);
	if (ret < 0) {
		LOG_ERROR_TAG(TAG, "create xmodem instance failed");
		goto safe_exit;
	}
#endif

	/* register methods */
	ret = miio_cmd_method_register(miio_handle, "none", do_invoke_none);
	if (ret != MIIO_OK) {
		goto safe_exit;
	}

	ret = miio_cmd_method_register(miio_handle, "get_properties", do_get_properties);
	if (ret != MIIO_OK) {
		goto safe_exit;
	}

	ret = miio_cmd_method_register(miio_handle, "set_properties", do_set_properties);
	if (ret != MIIO_OK) {
		goto safe_exit;
	}

	ret = miio_cmd_method_register(miio_handle, "update_fw", do_invoke_ota);
	if (ret != MIIO_OK) {
		goto safe_exit;
	}

	ret = miio_cmd_method_register(miio_handle, "action", do_invoke_action);
	if (ret != MIIO_OK) {
		goto safe_exit;
	}

	ret = miio_cmd_method_register(miio_handle, "MIIO_net_change", do_net_changed);
	if (ret != MIIO_OK) {
		goto safe_exit;
	}

	/* command handle loop */
	ret = miio_command_handle_loop(miio_handle, pbuf, CMD_STR_MAX_LEN);
	if (ret != MIIO_OK) {
		goto safe_exit;
	}

	/* delete miio instance */
	miio_instance_destroy(miio_handle);

	return MIIO_OK;

safe_exit:
	/* delete miio instance */
	miio_instance_destroy(miio_handle);

	return MIIO_ERROR;
}
