#include "malloc.h"
#include "malloc_debug.h"
#include "malloc_pool.h"

extern t_mmanager g_manager;

void show_alloc_mem(void)
{
	print_allocations(&g_manager);
}