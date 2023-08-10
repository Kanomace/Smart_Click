/**
 * @par     Copyright (c):
 *
 *    Copyright 2019 MIoT,MI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */


#ifndef __OPERATION_EXECUTOR_H__
#define __OPERATION_EXECUTOR_H__

#include "miio_define.h"
#include "property_operation.h"
#include "event_operation.h"

int execute_property_operation(void *handle, const char *pbuf, int buf_sz, property_operation_type type, miio_fp_cmd_delegate_ack_t ack);
int execute_action_invocation(void *handle, const char *pbuf, int buf_sz, miio_fp_cmd_delegate_ack_t ack);
int send_error_code(void *handle, const char *pbuf, int buf_sz, miio_fp_cmd_delegate_ack_t ack);
int send_event_occurred(miio_handle_t handle, event_operation_t *event);
int send_property_changed(miio_handle_t handle, uint32_t siid, uint32_t piid, property_value_t *newValue);

#endif /* __OPERATION_EXECUTOR_H__ */
