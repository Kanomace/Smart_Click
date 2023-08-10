/**
* Copyright (C) 2013-2015
*
* @file   IID.h
*
* @remark
*
*/

#ifndef __IID_H__
#define __IID_H__

#include "S_1_DeviceInformation_doGet.h"
#include "S_1_DeviceInformation_doChange.h"
#include "S_2_SwitchSensor_doSet.h"
#include "S_2_SwitchSensor_doGet.h"
#include "S_2_SwitchSensor_doChange.h"



#define IID_1_DeviceInformation                                               1
#define IID_1_1_Manufacturer                                                  1
#define IID_1_2_Model                                                         2
#define IID_1_3_SerialNumber                                                  3
#define IID_1_4_FirmwareRevision                                              4
#define IID_1_5_SerialNo                                                      5

#define IID_2_SwitchSensor                                                    2
#define IID_2_4_On                                                            4


#endif /* __IID_H__ */
