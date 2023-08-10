#include "arch_dbg.h"

#define BEIJING_UTC_SECOND_OFFSET			8*3600

log_level_t g_log_level = LOG_LEVEL_INFO;

#if 0
#include <time.h>
#include <sys/time.h>
#endif

void arch_printf_begin(void)
{

#if 0	/* add log time header here */
    long current = time(NULL);
	struct tm* ptm = localtime(&current);

	struct timeval tv;
	gettimeofday(&tv, NULL);

	printf("%02d:%02d:%02d.%03d ", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (int)tv.tv_usec / 1000);
#endif
}

void arch_printf_end(void)
{
	arch_printf("\r\n");
}

