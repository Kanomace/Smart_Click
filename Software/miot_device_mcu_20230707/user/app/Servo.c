#include "stm32f10x.h"
#include "Servo.h"
void TIM1_PWM_Init(u16 arr,u16 psc);

void TIM1_PWM_Init(u16 arr,u16 psc)//自动重装载值 预分频系数
{  
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// //使能定时器1的时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能
	                                                                     	

   //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能	 //使能输出比较预装载
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
 
   
}

// int main(void)
// {
//  u16 pwmval;	 
//  TIM1_PWM_Init(199,7199);//20ms
//	delay_init();	    	 //延时函数初始化	  
// 	while(1)
//   {
// 		delay_ms(500);	 
//		 pwmval=185;  //90°					 
//		TIM_SetCompare1(TIM1,pwmval);	  //1ms  45° 5%  200-200*5%=190
//        delay_ms(500);	
//		
//		 
////		 TIM_SetCompare1(TIM1,195);//0°   
////    delay_ms(500);			 
////		 TIM_SetCompare1(TIM1,190);//45° 	  
////    delay_ms(500);	
////		  TIM_SetCompare1(TIM1,185);//90°  	  
////    delay_ms(500);	
////		  TIM_SetCompare1(TIM1,180);//135° 	  
////    delay_ms(500);	
////		 TIM_SetCompare1(TIM1,175);//180° 	  
////    delay_ms(500);
//		 
//	 } 
//}

// int main(void)
// {
//  u16 pwmval;	 
//  TIM1_PWM_Init(199,7199);//20ms
//	delay_init();	    	 //延时函数初始化	  
// 	while(1)
//   {
//		 TIM_SetCompare1(TIM1,195);//0°   
//     delay_ms(6000);		
//     TIM_SetCompare1(TIM1,178);//0°   		 
//		 delay_ms(6000);	
////		 TIM_SetCompare1(TIM1,195);//0°   
////    delay_ms(500);			 
////		 TIM_SetCompare1(TIM1,190);//45° 	  
////    delay_ms(500);	
////		  TIM_SetCompare1(TIM1,185);//90°  	  
////    delay_ms(500);	
////		  TIM_SetCompare1(TIM1,180);//135° 	  
////    delay_ms(500);	
////		 TIM_SetCompare1(TIM1,175);//180° 	  
////    delay_ms(500);
////		 
//	 } 
//}
