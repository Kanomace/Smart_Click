#include "stm32f10x.h"
#include "Servo.h"
void TIM1_PWM_Init(u16 arr,u16 psc);

void TIM1_PWM_Init(u16 arr,u16 psc)//�Զ���װ��ֵ Ԥ��Ƶϵ��
{  
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// //ʹ�ܶ�ʱ��1��ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	                                                                     	

   //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 //ʹ������Ƚ�Ԥװ��
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
 
   
}

// int main(void)
// {
//  u16 pwmval;	 
//  TIM1_PWM_Init(199,7199);//20ms
//	delay_init();	    	 //��ʱ������ʼ��	  
// 	while(1)
//   {
// 		delay_ms(500);	 
//		 pwmval=185;  //90��					 
//		TIM_SetCompare1(TIM1,pwmval);	  //1ms  45�� 5%  200-200*5%=190
//        delay_ms(500);	
//		
//		 
////		 TIM_SetCompare1(TIM1,195);//0��   
////    delay_ms(500);			 
////		 TIM_SetCompare1(TIM1,190);//45�� 	  
////    delay_ms(500);	
////		  TIM_SetCompare1(TIM1,185);//90��  	  
////    delay_ms(500);	
////		  TIM_SetCompare1(TIM1,180);//135�� 	  
////    delay_ms(500);	
////		 TIM_SetCompare1(TIM1,175);//180�� 	  
////    delay_ms(500);
//		 
//	 } 
//}

// int main(void)
// {
//  u16 pwmval;	 
//  TIM1_PWM_Init(199,7199);//20ms
//	delay_init();	    	 //��ʱ������ʼ��	  
// 	while(1)
//   {
//		 TIM_SetCompare1(TIM1,195);//0��   
//     delay_ms(6000);		
//     TIM_SetCompare1(TIM1,178);//0��   		 
//		 delay_ms(6000);	
////		 TIM_SetCompare1(TIM1,195);//0��   
////    delay_ms(500);			 
////		 TIM_SetCompare1(TIM1,190);//45�� 	  
////    delay_ms(500);	
////		  TIM_SetCompare1(TIM1,185);//90��  	  
////    delay_ms(500);	
////		  TIM_SetCompare1(TIM1,180);//135�� 	  
////    delay_ms(500);	
////		 TIM_SetCompare1(TIM1,175);//180�� 	  
////    delay_ms(500);
////		 
//	 } 
//}
