#include "malloc.h"
#include "malloc_util.h"
#include <stdio.h>

void *malloc(unsigned long size) __attribute__((visibility("default")));

/**
 * @fn malloc
 * @brief allocate memory block by size
 * @param size size
 * @return a pointer to the newly allocated memory, or NULL if failed
 */
void *malloc(unsigned long size)
{
	return ((void *)(size));
}