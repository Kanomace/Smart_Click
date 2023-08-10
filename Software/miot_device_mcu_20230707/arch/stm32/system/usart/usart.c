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

#include "usart.h"	  
 
#if 1
#pragma import(__use_no_semihosting)             
         
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
  
void _sys_exit(int x) 
{ 
	x = x; 
} 
/* redirect fputc() */
int fputc(int ch, FILE *f)
{      
	while((USART1->SR & 0X40) == 0);
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


 
#if EN_USART1_RX  	
u8 USART_RX_BUFFER[USART_RECV_LEN];     

u16 USART_RX_STATE=0;       
  
void uart_init(u32 boundrate) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	

	USART_InitStructure.USART_BaudRate = boundrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);                    
}

void USART1_IRQHandler(void)                	
{
	u8 ch;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		ch = USART_ReceiveData(USART1);	
		
		if((USART_RX_STATE & 0x8000) == 0)
		{
			if(USART_RX_STATE & 0x4000)
			{
				if(ch != 0x0a)
					USART_RX_STATE = 0;
				else 
					USART_RX_STATE |= 0x8000;
			}
			else
			{	
				if(ch == 0x0d)
					USART_RX_STATE |= 0x4000;
				else
				{
					USART_RX_BUFFER[USART_RX_STATE&0X3FFF]=ch ;
					USART_RX_STATE++;
					if(USART_RX_STATE > (USART_RECV_LEN-1))
						USART_RX_STATE = 0;
				}		 
			}
		}   		 
     } 

} 
#endif	

