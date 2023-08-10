/**
* Copyright (C) 2013-2015
*
* @file   S_2_SwitchSensor_doSet.c
*
* @remark
*
*/

#include "S_2_SwitchSensor_doSet.h"
#include "S_2_SwitchSensor_doChange.h"
#include "operation_code.h"
#include "iid.h"


static void P_2_4_On_doSet(property_operation_t *o)
{
    // 判断数据格式是否正确，如果错误，返回代码: OPERATION_ERROR_VALUE
    if (o->value->format != PROPERTY_FORMAT_BOOLEAN)
    {
        o->code = OPERATION_ERROR_VALUE;
        return;
    }

    // TODO: 执行写操作: o->value->data.boolean.value;
		if(o->value->data.boolean.value==1){
    TIM_SetCompare1(TIM1,195);//180°
    delay_ms(2000);			
		}
		else{
		TIM_SetCompare1(TIM1,180);//0° 
		delay_ms(2000);	
		}
    // 如果成功，返回代码: OPERATION_OK
    o->code = OPERATION_OK;
    
    //上报状态，通知app状态变化，统一APP修改、定时器触发后的上报机制
//    P_2_4_On_doChange_notify(o->value->data.boolean.value);

    return;
}

void S_2_SwitchSensor_doSet(property_operation_t *o)
{
    switch (o->piid)
    {
        case IID_2_4_On:
            P_2_4_On_doSet(o);
            break;

        default:
            o->code = OPERATION_ERROR_CANNOT_WRITE;
            break;
    }
}
