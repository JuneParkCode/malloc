#include "malloc.h"
#include "malloc_util.h"
#include <stdio.h>

t_arena g_arena;

void *malloc(unsigned long size) __attribute__((visibility("default")));

/**
 * @fn __allocate
 * @brief allocate memory block by size
 * @param arena arena
 * @param size size
 * @return new alloc block (skip header for user space), or NULL if failed
 */
static void *__allocate(t_arena *const arena, const size_t size)
{
	const size_t block_size = __get_request_block_size(size);
	void *alloc = NULL;

	switch (GET_SIZE_TYPE(block_size)) {
	case TINY:
		alloc = __allocate_tiny_block(arena, block_size);
		break;
	case SMALL:
		alloc = __allocate_small_block(arena, block_size);
		break;
	case LARGE:
		alloc = __allocate_large_block(arena, block_size);
		break;
	}
	return ((alloc + sizeof(size_t))); // skip header
}

/**
 * @fn malloc
 * @brief allocate memory block by size
 * @param size size
 * @return a pointer to the newly allocated memory, or NULL if failed
 */
void *malloc(unsigned long size)
{
	t_arena *const arena = &g_arena;

	if (size == 0 || MAX_MALLOC_SIZE < size)
		return (NULL);
	return (__allocate(arena, size));
}