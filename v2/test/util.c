#include "malloc_pool.h"

void *add_addr(void const *addr, ssize_t size)
{
	if (size < 0)
		return (void *)((uintptr_t)addr - (uintptr_t)(-size));
	return (void *)((uintptr_t)addr + (uintptr_t)size);
}