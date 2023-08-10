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
#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f10x.h"

#define USART_RECV_LEN  			256  	
#define EN_USART1_RX 			1		
	  	
extern u8  USART_RX_BUFFER[USART_RECV_LEN]; 
extern u16 USART_RX_STATE;         		

void uart_init(u32 boundrate);
#endif


