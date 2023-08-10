/**
 * Copyright (C) 2013-2015
 *
 * @file   S_2_SwitchSensor_doChange.h
 *
 * @remark
 *
 */

#ifndef __S_2_SwitchSensor_doChange_H__
#define __S_2_SwitchSensor_doChange_H__

#include "miio_define.h"
#include "property_operation.h"


void P_2_4_On_doChange(miio_handle_t handle, bool newValue);
void P_2_4_On_doChange_notify(bool newValue);


#endif /* __S_2_SwitchSensor_doChange_H__ */

