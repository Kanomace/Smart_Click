/**
 * Copyright (C) 2013-2015
 *
 * @file   on_property_get.c
 *
 * @remark
 *
 */

#include "on_property_get.h"
#include "print_value.h"
#include "arch_dbg.h"
#include "operation_code.h"
#include "iid.h"
#include "S_1_DeviceInformation_doGet.h"
#include "S_2_SwitchSensor_doGet.h"

void on_property_get(property_operation_t *o)
{
    LOG_INFO("on_property_get\n");
    //LOG_INFO("did: %s\n", o->did);
	LOG_INFO("siid: %d\n", o->siid);
	LOG_INFO("piid: %d\n", o->piid);

    switch (o->siid)
    {
            case IID_1_DeviceInformation:
            S_1_DeviceInformation_doGet(o);
            break;

            case IID_2_SwitchSensor:
            S_2_SwitchSensor_doGet(o);
            break;

        default:
            o->code = OPERATION_INVALID;
            break;
    }
}
