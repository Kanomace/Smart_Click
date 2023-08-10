/**
 * Copyright (C) 2013-2015
 *
 * @file   operation_executor.c
 *
 * @remark
 *
 */

#include "miio_define.h"

#include "arch_dbg.h"
#include "miio_api.h"
#include "util.h"
#include "on_property_get.h"
#include "on_property_set.h"
#include "on_action_invoke.h"
#include "property_operation_decoder.h"
#include "property_operation_encoder.h"
#include "property_changed_encoder.h"
#include "action_operation_decoder.h"
#include "action_operation_encoder.h"
#include "event_operation_encoder.h"
#include "xmodem.h"

#include "operation_executor.h"

#undef	TAG
#define TAG "operation_executor"

#define ID_MAX_LEN			4
#define VALUE_MAX_LEN		16


/* ==============  Private functions  ============== */

static int miio_event_send(miio_uart_t *uart, char out[], size_t len)
{
	int ret = MIIO_OK;
	char res[RESULT_BUF_SIZE] = { 0 };
	memset(res, 0, RESULT_BUF_SIZE);
	
	int n_send = uart->send_str(uart, out);
	if (n_send < 0)
		return MIIO_ERROR;
	if(n_send != len) {
		LOG_ERROR_TAG(TAG, "send event failed 1");
		ret = MIIO_ERROR;
		return ret;
	}

	int n_read = uart->recv_str(uart, (uint8_t *)res, RESULT_BUF_SIZE, 500);
	if(n_read <= 0) {
		LOG_ERROR_TAG(TAG, "send event failed 2");
		ret = MIIO_ERROR;
		return ret;
	}

	if(0 != strncmp(res, "ok", strlen("ok"))) {
		LOG_ERROR_TAG(TAG, "send event failed 3");
		ret = MIIO_ERROR;
		return ret;
	}

	return ret;
}

static int miio_send_response(void *handle, miio_fp_cmd_delegate_ack_t ack, const char *result)
{
	return ack(handle, result, strlen(result));
}

static int execute_property_changed(miio_handle_t handle, property_operation_t *opt)
{
	int ret = MIIO_OK;
	
	miio_t *miio = (miio_t *)handle;
	miio_uart_t *uart = &(miio->uart);

	char out[RESULT_BUF_SIZE] = { 0 };
	memset(out, 0, RESULT_BUF_SIZE);
	
	miio_changed_operation_encode(opt, out, RESULT_BUF_SIZE);
	int n_send = uart->send_str(uart, out);
	if(n_send <= 0) {
		LOG_ERROR_TAG(TAG, "property changed send failed");
		ret = MIIO_ERROR;
		return ret;
	}

	char res[RESULT_BUF_SIZE] = { 0 };
	memset(res, 0, RESULT_BUF_SIZE);

	int n_read = uart->recv_str(uart, (uint8_t *)res, RESULT_BUF_SIZE, USER_UART_TIMEOUT_MS);
	if(n_read <= 0) {
		LOG_ERROR_TAG(TAG, "property changed send failed");
		ret = MIIO_ERROR;
		return ret;
	}

	if(0 != strncmp(res, "ok", strlen("ok"))) {
		LOG_ERROR_TAG(TAG, "property changed send failed");
		ret = MIIO_ERROR;
		return ret;
	}

	return ret;
}

/* ==============  Public functions  ============== */

/**
 * ------------------------------------------------
 * request : down get_properties 1 1 2 1
 * ------------------------------------------------
 * response : result 1 1 0 "hello" 2 1 0 true
 * ------------------------------------------------
 *
 * 
 * ------------------------------------------------
 * request : down set_properties 1 1 "hello" 2 1 true
 * ------------------------------------------------
 * response : result 1 1 0 2 1 0
 * ------------------------------------------------
 */
int execute_property_operation(void *handle, const char *pbuf, int buf_sz, property_operation_type type, miio_fp_cmd_delegate_ack_t ack)
{
	int ret = 0;

	char *cmd_buf = (char *)calloc(1, buf_sz);
	if(NULL == cmd_buf){
		LOG_ERROR_TAG(TAG, "command buf malloc failed");
		return MIIO_ERROR;
	}

	memcpy(cmd_buf, pbuf, buf_sz - 1);
	cmd_buf[buf_sz - 1] = '\0';	/* remove '\r' at the end */

	int params_pairs = 0;
	
	/* don't need to check temp, cause it checked before enter into this func */
    char *temp = strtok(cmd_buf, " ");	/* pass string "down" */
	temp = strtok(NULL, " ");			/* pass string "get_properties"/"set_properties" */

	while(1) {
		temp = strtok(NULL, " ");
		if (NULL != temp) { params_pairs++; }
		else { break; }
	}

	switch(type){

		case PROPERTY_OPERATION_GET :
		{
			if(params_pairs % 2 != 0) {
				LOG_ERROR_TAG(TAG, "params error");
				ret = -1;
			}
			else { params_pairs /= 2; }
		}
		break;
		
		case PROPERTY_OPERATION_SET :
		{
			if(params_pairs % 3 != 0) {
				LOG_ERROR_TAG(TAG, "params error");
				ret = -1;
			}
			else { params_pairs /= 3; }
		}
		break;
	}

	do {
		bool has_value = (type == PROPERTY_OPERATION_SET);

		char result[RESULT_BUF_SIZE] = {0};
		str_n_cat(result, 1, "result ");

	 	for (uint32_t i = 0; i < params_pairs; ++i) {
			/* decode command params */
	 		property_operation_t * opt = miio_property_operation_decode(pbuf, buf_sz, i, has_value);

			if (opt == NULL) { break; }

			if (has_value) {
				on_property_set(opt);
			} else {
				on_property_get(opt);
			}
			
			property_operation_encode_param(result, RESULT_BUF_SIZE, opt, has_value);
			miio_property_operation_delete(opt);
		}
		
		property_operation_encode_tail(result, RESULT_BUF_SIZE);
		miio_send_response(handle, ack, result);
	} while (false);

	if(NULL != cmd_buf) { free(cmd_buf); }

	return ret;
}


/**
 * ------------------------------------------------
 * request : down action 4 1 1 false
 * ------------------------------------------------
 * response : result 4 1 0 1 false
 * ------------------------------------------------
 */
int execute_action_invocation(void *handle, const char *pbuf, int buf_sz, miio_fp_cmd_delegate_ack_t ack)
{
	int ret = 0;
	
		char *cmd_buf = (char *)calloc(1, buf_sz);
		if(NULL == cmd_buf){
			LOG_ERROR_TAG(TAG, "command buf malloc failed");
			return MIIO_ERROR;
		}
	
		memcpy(cmd_buf, pbuf, buf_sz - 1);
		cmd_buf[buf_sz - 1] = '\0'; /* remove '\r' at the end */
		int params_pairs = 0;
		
		/* don't need to check temp, cause it checked before enter into this func */
		char *temp = strtok(cmd_buf, " ");	/* pass string "down" */
		temp = strtok(NULL, " ");	/* pass string "action" */
	
		while(1) {
			temp = strtok(NULL, " ");
			if (NULL != temp) { params_pairs++; }
			else { break; }
		}

		if(params_pairs >= 4 && params_pairs % 2 != 0) {
			LOG_ERROR(TAG, "params error");
			ret = -1;
			send_error(handle, ERROR_MESSAGE_UNPARAMS, ERROR_CODE_UNPARAMS);
			return ret;
		} else {
			params_pairs = (params_pairs + 3) / 4;
		}
	
		do 
		{
			char result[RESULT_BUF_SIZE] = {0};
			str_n_cat(result, 1, "result ");
	
			for (uint32_t i = 0; i < params_pairs; ++i) {

				// decode command params
				action_operation_t * opt = action_operation_decode(pbuf, buf_sz, i);

				if (opt == NULL) { break; }

				on_action_invoke(opt);
				action_operation_encode_param(result, sizeof(result), opt);
				action_operation_delete(opt);
			}
			
			action_operation_encode_tail(result, sizeof(result));
	
			miio_send_response(handle, ack, result);
		} while (false);
	
		if(NULL != cmd_buf) { free(cmd_buf); }

	return ret;
}

int send_error_code(void *handle, const char *pbuf, int buf_sz, miio_fp_cmd_delegate_ack_t ack)
{
	int ret = 0;
		do 
		{
			char result[RESULT_BUF_SIZE] = {0};
			str_n_cat(result, 2, "error ", pbuf);
			action_operation_encode_tail(result, sizeof(result));
			miio_send_response(handle, ack, result);
		} while (false);

	return ret;
}

int send_property_changed(miio_handle_t handle, uint32_t siid, uint32_t piid, property_value_t *newValue)
{
	int ret = 0;
	LOG_INFO_TAG(TAG, "=============== property changed ===============");
	
	property_operation_t *opt = NULL;

	if(NULL == handle || NULL == newValue) {
		ret = MIIO_ERROR_PARAM;
		return ret;
	}

	do {
		opt = miio_property_operation_new();
		opt->code = 0;
		opt->siid = siid;
		opt->piid = piid;
		opt->value = newValue;
	} while (false);

	ret = execute_property_changed(handle, opt);

	if(ret != MIIO_OK) {
		LOG_INFO_TAG(TAG, "========= send property changed failed =========");
	} else {
		LOG_INFO_TAG(TAG, "======== send property changed success =========");
	}

	miio_property_operation_delete(opt);

	return ret;
}

int send_event_occurred(miio_handle_t handle, event_operation_t *event)
{
	int ret = MIIO_OK;
					   
	LOG_INFO_TAG(TAG, "================= event occurred ===============");

	if(NULL == handle || NULL == event) {
		LOG_ERROR_TAG(TAG, "handle is NULL");
		ret = MIIO_ERROR_PARAM;
		return ret;
	}

	miio_t *miio = (miio_t *)handle;
	miio_uart_t *uart = &(miio->uart);

	do
	{
		char out[RESULT_BUF_SIZE] = { 0 };
		memset(out, 0, RESULT_BUF_SIZE);
		
		miio_event_operation_encode(event, out, RESULT_BUF_SIZE);

		ret = miio_event_send(uart, out, strlen(out));
	}while(false);

	if(MIIO_OK == ret) {
		LOG_INFO_TAG(TAG, "============== event send success ==============");
	} else {
		LOG_INFO_TAG(TAG, "============== event send failed  ==============");
	}

	return ret;
}
