#include "malloc.h"
#include "malloc_debug.h"

extern t_mmanager g_manager;
extern pthread_mutex_t g_mutex;

void show_alloc_mem(void)
{
	pthread_mutex_lock(&g_mutex);
	print_allocations(&g_manager);
	pthread_mutex_unlock(&g_mutex);
}