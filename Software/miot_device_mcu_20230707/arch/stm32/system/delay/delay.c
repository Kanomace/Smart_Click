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
#include "delay.h"

static u8  systime_us = 0;							
static u16 systime_ms = 0;							


void delay_init()
{

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
	systime_us = SystemCoreClock / 8000000;				

	systime_ms = (u16)systime_us * 1000;					

}								    
	    								   
void delay_us(u32 num_us)
{		
	u32 temp;	    	 
	SysTick->LOAD = num_us*systime_us; 					
	SysTick->VAL = 0x00;        					
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	
	do
	{
		temp = SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (1 << 16)));		
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	
	SysTick->VAL = 0X00;      					
}

void delay_ms(u16 num_ms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD = (u32)num_ms*systime_ms;				
	SysTick->VAL = 0x00;							
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	
	do
	{
		temp = SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (1 << 16)));		
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	
	SysTick->VAL = 0X00;       					
} 









































