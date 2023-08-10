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


#ifndef __ARCH_DBG_H__
#define __ARCH_DBG_H__

#include "miio_define.h"
#include "user_config.h" /* include MIIO_LOG_LEVEL in "user_config.h" */

#define LOG_LEVEL_VERBOSE		0
#define LOG_LEVEL_INFO			1
#define LOG_LEVEL_WARNING		2
#define LOG_LEVEL_ERROR			3
#define LOG_LEVEL_FATAL			4
#define LOG_LEVEL_OFF			5

typedef int log_level_t;

#define arch_dbg_printf				arch_printf
void arch_printf_begin(void);
void arch_printf_end(void);

#ifndef MIIO_LOG_LEVEL
#define MIIO_LOG_LEVEL			LOG_LEVEL_VERBOSE
#endif

#define LOG_LEVEL				MIIO_LOG_LEVEL

#if MIIO_LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_INFO){\
			arch_printf_begin();\
			arch_dbg_printf("[I] "_fmt_, ##__VA_ARGS__);\
			arch_printf_end();\
		}\
	}while(0)

#define LOG_INFO_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_INFO){\
			arch_printf_begin();\
			arch_dbg_printf("[I] %s: "_fmt_, TAG, ##__VA_ARGS__);\
			arch_printf_end();\
		}\
	}while(0)

#else

#define LOG_INFO(_fmt_, ...) \
	do{}while(0)

#define LOG_INFO_TAG(TAG, _fmt_, ...) \
	do{}while(0)

#endif

#if MIIO_LOG_LEVEL <= LOG_LEVEL_WARNING
#define LOG_WARN(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_WARNING){\
			arch_printf_begin();\
			arch_dbg_printf("[W] "_fmt_" (%s, %d, %s)", ##__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__);\
			arch_printf_end();\
		}\
	}while(0)
#define LOG_WARN_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_WARNING){\
			arch_printf_begin();\
			arch_dbg_printf("[W] %s: "_fmt_" (%s, %d, %s)", TAG, ##__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__);\
			arch_printf_end();\
		}\
	}while(0)
#else
#define LOG_WARN(_fmt_, ...) \
	do{}while(0)
#define LOG_WARN_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif

#if MIIO_LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_ERROR){\
			arch_printf_begin();\
			arch_dbg_printf("[E] "_fmt_" (%s, %s, %d)", ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__);\
			arch_printf_end();\
		}\
	}while(0)
#define LOG_ERROR_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_ERROR){\
			arch_printf_begin();\
			arch_dbg_printf("[E] %s: "_fmt_" (%s, %s, %d)", TAG, ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__);\
			arch_printf_end();\
		}\
	}while(0)
#else
#define LOG_ERROR(_fmt_, ...) \
	do{}while(0)
#define LOG_ERROR_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif

#if MIIO_LOG_LEVEL <= LOG_LEVEL_FATAL
#define LOG_FATAL(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_FATAL){\
			arch_printf_begin();\
			arch_dbg_printf("[F] "_fmt_" (%s, %d)", ##__VA_ARGS__, __FUNCTION__, __LINE__);\
			arch_printf_end();\
		}\
	}while(0)
#define LOG_FATAL_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_FATAL){\
			arch_printf_begin();\
			arch_dbg_printf("[F] %s: "_fmt_" (%s, %d)", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__);\
			arch_printf_end();\
		}\
	}while(0)

#else
#define LOG_FATAL(_fmt_, ...) \
	do{}while(0)
#define LOG_FATAL_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif

#if MIIO_LOG_LEVEL <= LOG_LEVEL_VERBOSE
#define LOG_DEBUG(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_VERBOSE){\
			arch_printf_begin();\
			arch_dbg_printf("[D] "_fmt_, ##__VA_ARGS__);\
			arch_printf_end();\
		}\
	}while(0)
#define LOG_DEBUG_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_VERBOSE){\
			arch_printf_begin();\
			arch_dbg_printf("[D] %s: "_fmt_, TAG, ##__VA_ARGS__);\
			arch_printf_end();\
		}\
	}while(0)
#else
#define LOG_DEBUG(_fmt_, ...) \
	do{}while(0)
#define LOG_DEBUG_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif


#endif  /* __ARCH_DBG_H__ */

