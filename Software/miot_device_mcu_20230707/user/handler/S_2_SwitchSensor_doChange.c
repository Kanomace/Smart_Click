/**
 * Copyright (C) 2013-2015
 *
 * @file   S_2_SwitchSensor_doChange.c
 *
 * @remark
 *
 */

#include "arch_dbg.h"
#include "arch_os.h"
#include "S_2_SwitchSensor_doChange.h"
#include "operation_executor.h"

#define TAG "S_2_SwitchSensor_doChange"

extern miio_handle_t g_miio_instance_handle;


void P_2_4_On_doChange(miio_handle_t handle, bool newValue)
{
    if (send_property_changed(handle, 2, 4, property_value_new_boolean(newValue)) != MIIO_OK)
    {
        LOG_ERROR_TAG(TAG, "send_property_changed failed!\n");
    }
}
/*
static void P_2_4_On_doChange_cb(void * newValue)
{
	bool value = (bool) newValue;
	if (NULL == g_miio_instance_handle)
	{
		LOG_ERROR("please create miio instance first!\r\n");
		return;
	}

	P_2_4_On_doChange(g_miio_instance_handle,value);
	
	
	return;
}

void P_2_4_On_doChange_notify(bool newValue)
{
	arch_os_async_call(P_2_4_On_doChange_cb,(void *)newValue,500);
}*/
