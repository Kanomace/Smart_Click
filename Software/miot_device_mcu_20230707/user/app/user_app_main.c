#include "miio_define.h"
#include "miio_api.h"
#include "miio_uart.h"
#include "arch_dbg.h"
#include "iid.h"
#include "user_app_func.h"
#include "Servo.h"
#include "Key.h"
#include "user_app_main.h"

static int cnt = 0;

int user_app_main(int argc, void *argv)
{
	if(cnt==0)
	{
		delay_init();	    	 //延时函数初始化
		TIM1_PWM_Init(199,7199);//20ms
		KEY_Init();
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0)
			app_func_restore(argv);
	 }
  delay_ms(1000);
	if(cnt++ % 20 == 0) {
//	  app_func_getwifi(argv);
		/* spec functions */
		//E_4_1_Testevent(argv, 1, false);
		//P_3_1_On_doChange(argv, false);

#if 1
		/* add user functions here */
		// app_func_factory(argv);
		//app_func_get_time(argv);
		//app_func_get_mac(argv);
		//app_func_get_net_state(argv);
//    TIM_SetCompare1(TIM1,195);//180°
//    delay_ms(5000);		
//		TIM_SetCompare1(TIM1,178);//0° 
//		delay_ms(5000);			
//		app_func_reboot(argv);
// 		app_func_restore(argv);
//		 app_func_getwifi(argv);
		// app_func_setwifi(argv);
		//app_func_get_version(argv);
		// app_func_set_mcu_version(argv);
		// app_func_get_arch_platform(argv);
#endif
	}

	return MIIO_OK;
}

