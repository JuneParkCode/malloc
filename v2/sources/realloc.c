#include "malloc.h"

void *realloc(void *ptr, size_t size)
{
	return ((void *)ptr + size);
}