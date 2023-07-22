#include "malloc_util.h"

/**
 * @fn __init_arena
 * @brief init arena tiny pool and small pool
 * @return 0 on success, -1 on failure
 */
int __init_arena(t_arena *const arena)
{
	if (!arena->small_free_pool &&
		(arena->small_free_pool = __init_small_pool(arena)) == NULL)
		return (-1);
	if (!arena->tiny_free_pool &&
		(arena->tiny_free_pool = __init_tiny_pool(arena)) == NULL)
		return (-1);
	return (0);
}