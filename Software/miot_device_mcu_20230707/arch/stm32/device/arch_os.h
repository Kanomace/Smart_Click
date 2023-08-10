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


#ifndef __ARCH_OS_H__
#define __ARCH_OS_H__

#include "miio_define.h"

#if (defined(USER_OS_ENABLE) && USER_OS_ENABLE)
#define arch_os_mutex	pthread_mutex_t
#else
#define arch_os_mutex	unsigned char
#endif

/**
 * @brief  get mutex if mcu support OS
 *
 * @param[in]  mutex: arch_os_mutex pointer(defined in arch_os.h)
 * @return     none
 */
void arch_os_mutex_get(arch_os_mutex* mutex);
/**
 * @brief  put mutex if mcu support OS
 *
 * @param[in]  mutex: arch_os_mutex pointer(defined in arch_os.h)
 * @return     none
 */
void arch_os_mutex_put(arch_os_mutex* mutex);
/**
 * @brief  init mutex if mcu support OS
 *
 * @param[in]  mutex: arch_os_mutex pointer(defined in arch_os.h)
 * @return     none
 */
void arch_os_mutex_init(arch_os_mutex *mutex);
/**
 * @brief  deinit mutex if mcu support OS
 *
 * @param[in]  mutex: arch_os_mutex pointer(defined in arch_os.h)
 * @return     none
 */
void arch_os_mutex_deinit(arch_os_mutex *mutex);

#endif	/*__ARCH_OS_H__*/
