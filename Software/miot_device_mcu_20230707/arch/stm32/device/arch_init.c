#include "delay.h"
#include "usart.h"
#include "stm32f10x.h"

#include "miio_define.h"
#include "arch_dbg.h"
#include "arch_init.h"

#undef	TAG
#define TAG		"arch_init"

int arch_mcu_init(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);

	LOG_INFO_TAG(TAG, "<****** add mcu init func here ******>");

	return MIIO_OK;
}
