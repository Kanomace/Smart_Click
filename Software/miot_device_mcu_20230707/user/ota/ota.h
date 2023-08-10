/**
 * @author  MIoT
 * @date    2019
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

#ifndef __OTA_H__
#define __OTA_H__

#include "user_config.h"

#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)
/**
 * @brief  execute ota operation when mcu received "down update_fw\r" from wifi module
 *
 * @param[in]  handle: miio_handle_t pointer
 *             pbuf: input parameters buffer (not used)
 *             buf_sz: length of pbuf (not used)
 *             ack: ack function (not used)
 * @return
 *             - MIIO_OK: success
 *             - MIIO_ERROR: failed
 */
int execute_ota_operation(void *handle, const char *pbuf, int buf_sz, miio_fp_cmd_delegate_ack_t ack);

#endif /* (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE) */

#endif  /* __OTA_H__ */