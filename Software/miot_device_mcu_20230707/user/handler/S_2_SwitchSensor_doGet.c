/**
* Copyright (C) 2013-2015
*
* @file   S_2_SwitchSensor_doGet.c
*
* @remark
*
*/

#include "S_2_SwitchSensor_doGet.h"
#include "operation_code.h"
#include "iid.h"

/**
 * 格式: property_value_new_boolean(true 或 false)　
 * 取值: true 或　false
 */
static void P_2_4_On_doGet(property_operation_t *o)
{
    o->value = property_value_new_boolean(true); // TODO: 这里需要读到属性真正的值
}

void S_2_SwitchSensor_doGet(property_operation_t *o)
{
    switch (o->piid)
    {
        case IID_2_4_On:
            P_2_4_On_doGet(o);
            break;

        default:
            o->code = OPERATION_ERROR_CANNOT_READ;
            break;
    }
}
