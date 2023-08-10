#include "miio_define.h"
#include "arch_os.h"

void arch_os_mutex_get(arch_os_mutex* mutex)
{
#if (defined(USER_OS_ENABLE) && USER_OS_ENABLE)
	pthread_mutex_lock(mutex);
#endif
}

void arch_os_mutex_put(arch_os_mutex* mutex)
{
#if (defined(USER_OS_ENABLE) && USER_OS_ENABLE)
	pthread_mutex_unlock(mutex);
#endif
}

void arch_os_mutex_init(arch_os_mutex* mutex)
{
#if (defined(USER_OS_ENABLE) && USER_OS_ENABLE)
	pthread_mutex_init(mutex, NULL);
#endif
}

void arch_os_mutex_deinit(arch_os_mutex *mutex)
{
#if (defined(USER_OS_ENABLE) && USER_OS_ENABLE)
	pthread_mutex_destroy(mutex);
#endif
}
