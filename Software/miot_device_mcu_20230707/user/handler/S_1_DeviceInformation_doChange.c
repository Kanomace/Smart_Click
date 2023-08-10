/**
 * Copyright (C) 2013-2015
 *
 * @file   S_1_DeviceInformation_doChange.c
 *
 * @remark
 *
 */

#include "arch_dbg.h"
#include "arch_os.h"
#include "S_1_DeviceInformation_doChange.h"
#include "operation_executor.h"

#define TAG "S_1_DeviceInformation_doChange"

extern miio_handle_t g_miio_instance_handle;


void P_1_5_SerialNo_doChange(miio_handle_t handle, const char * newValue)
{
    if (send_property_changed(handle, 1, 5, property_value_new_string(newValue)) != MIIO_OK)
    {
        LOG_ERROR_TAG(TAG, "send_property_changed failed!\n");
    }
}
/*
static void P_1_5_SerialNo_doChange_cb(void * newValue)
{
	const char * value = (const char *) newValue;
	if (NULL == g_miio_instance_handle)
	{
		LOG_ERROR("please create miio instance first!\r\n");
		return;
	}

	P_1_5_SerialNo_doChange(g_miio_instance_handle,value);
	
	
	return;
}

void P_1_5_SerialNo_doChange_notify(const char * newValue)
{
	arch_os_async_call(P_1_5_SerialNo_doChange_cb,(void *)newValue,500);
}*/
