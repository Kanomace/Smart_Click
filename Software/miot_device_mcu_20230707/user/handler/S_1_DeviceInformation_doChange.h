/**
 * Copyright (C) 2013-2015
 *
 * @file   S_1_DeviceInformation_doChange.h
 *
 * @remark
 *
 */

#ifndef __S_1_DeviceInformation_doChange_H__
#define __S_1_DeviceInformation_doChange_H__

#include "miio_define.h"
#include "property_operation.h"


void P_1_5_SerialNo_doChange(miio_handle_t handle, const char * newValue);
void P_1_5_SerialNo_doChange_notify(const char * newValue);


#endif /* __S_1_DeviceInformation_doChange_H__ */

