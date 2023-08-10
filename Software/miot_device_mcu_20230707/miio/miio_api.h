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
#ifndef _MIIO_API_H_
#define _MIIO_API_H_

#include "miio_define.h"
#include "miio_uart.h"
#include "list.h"
#include "uart_command.h"

#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)
#include "xmodem.h"
#endif

typedef struct miio {
	miio_cmd_t cmd;
	miio_uart_t uart;
#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)
	miio_xmodem_t xmodem;
#endif
}miio_t;

/**
 * @brief  create miio instance
 *
 * @param[in]  void
 *
 * @return
 *      - miio_handle_t pointer
 *
 * @note
 *      - this function will malloc memory for miio_t struct, \n
 *        and it will be release in miio_instance_destroy() function
 */
miio_handle_t miio_instance_create(void);

/**
 * @brief  create uart instance into miio instance
 *
 * @param[in]  handle: miio_handle_t struct created in miio_instance_create()
 *
 * @return
 *      - MIIO_OK: success
 *      - error code
 */
int miio_instance_add_uart(miio_handle_t handle);

#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)
/**
 * @brief  create xmodem instance into miio instance
 *
 * @param[in]  handle: miio_handle_t struct created in miio_instance_create()
 *
 * @return
 *      - MIIO_OK: success
 *      - error code
 * @note
 *      - this function must be called after uart instance create, \n
 *        cause xmodem needs to include uart function.
 */
int miio_instance_add_xmodem(miio_handle_t handle);
#endif

/**
 * @brief  register method handle function for miio instance
 *
 * @param[in]  handle: miio_handle_t struct created in miio_instance_create()
 *             method: method name, this name will be used to find method callback function, \n
 *                     in miio_command_handle_loop() function
 *             cb: callback function for method handle
 *
 * @return
 *      - MIIO_OK: success
 *      - MIIO_ERROR: failure
 */
int miio_cmd_method_register(miio_handle_t handle, char *method, miio_fp_cmd_handle_t cb);

/**
 * @brief  handle loop to handle uart command
 *
 * @param[in]  handle: miio_handle_t struct created in miio_instance_create()
 *             pbuf: buffer to receive uart command
 *             len: buffer length
 *
 * @return
 *      - MIIO_OK: exit normaly
 *      - MIIO_ERROR: exit due to error
 */
int miio_command_handle_loop(miio_handle_t handle, char *pbuf, int len);

/**
 * @brief  exit from uart command handle loop
 *
 * @param[in]  handle: miio_handle_t struct created in miio_instance_create()
 *
 * @return
 *      - none
 */
void miio_command_handle_leave(miio_handle_t handle);

/**
 * @brief  destroy miio instance
 *
 * @param[in]  handle: miio_handle_t struct created in miio_instance_create()
 *
 * @return
 *      - none
 *
 * @note
 *      - this functio will release memory malloced in miio_instance_create()
 */
void miio_instance_destroy(miio_handle_t handle);

int send_error(miio_handle_t handle, char *message, int errcode);

#endif /* _MIIO_API_H_ */
