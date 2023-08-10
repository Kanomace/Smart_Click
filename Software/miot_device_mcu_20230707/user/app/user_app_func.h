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


#ifndef __USER_APP_FUNC_H__
#define __USER_APP_FUNC_H__

/**
 * @brief  get time string from wifi module
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_get_time(void *argv);
/**
 * @brief  get mac string from wifi module
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_get_mac(void *argv);
/**
 * @brief  get net state from wifi module
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_get_net_state(void *argv);
/**
 * @brief  reboot wifi module
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_reboot(void *argv);
/**
 * @brief  restore wifi module(clear user info & wifi info)
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_restore(void *argv);
/**
 * @brief  get wifi info from wifi module
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_getwifi(void *argv);
/**
 * @brief  set wifi info to wifi module (ssid & passwd)
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_setwifi(void *argv);
/**
 * @brief  get version from wifi module
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_get_version(void *argv);
/**
 * @brief  set mcu version to wifi module
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_set_mcu_version(void *argv);
/**
 * @brief  get platform info from wifi module
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_get_arch_platform(void *argv);
/**
 * @brief  set factory mode to module
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_factory(void *argv);

/**
 * @brief  wifi module changed callback function
 *
 * @param[in]  argv: net status string: such as "down MIIO_net_change cloud"
 *
 * @return
 *      - 0 : success
 *      - error code
 */
int app_func_net_change(void *argv);

/**
 * @brief  user should judge if device is busy and cannot process ota
 *
 * @param[in]  argv: miio_handle_t pointer
 *
 * @return
 *      - true : device is busy and cannot process ota
 *      - false : device is ready to process ota
 */
bool app_func_is_ota_busy(void *argv);

#endif /* __USER_APP_FUNC_H__ */
