#include "malloc.h"

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