#include "miio_define.h"
#include "arch_dbg.h"
#include "arch_init.h"

#undef	TAG
#define TAG		"arch_init"

int arch_mcu_init(void)
{
	LOG_INFO_TAG(TAG, "<****** add mcu init func here ******>");

	return MIIO_OK;
}
