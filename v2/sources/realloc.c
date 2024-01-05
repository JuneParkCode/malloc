#include "malloc.h"
#include "malloc_pool.h"
#include "malloc_util.h"

void *realloc(void *ptr, size_t size)
{
	// edge case
	if (ptr == NULL)
		return (malloc(size));
	if (size == 0) {
		free(ptr);
		return (NULL);
	}
	// operations
	t_pool *const pool = find_block_pool(ptr, &g_manager);
	POOL_TYPE const type = pool->type;
	t_metadata *metadata;
	size_t current_block_size;
	size_t request_block_size;
	void *ret;

	switch (type) {
	case TINY:
		request_block_size = MALLOC_TINY_SIZE_MIN << get_order(size, type);
		metadata = get_block_metadata(pool, ptr);
		current_block_size = get_block_size(*metadata, pool->type);
		break;
	case SMALL: {
		request_block_size = MALLOC_SMALL_SIZE_MIN << get_order(size, type);
		metadata = get_block_metadata(pool, ptr);
		current_block_size = get_block_size(*metadata, pool->type);
		break;
	}
	case LARGE: {
		request_block_size = get_align_size(size, PAGE_SIZE);
		current_block_size = pool->size;
		metadata = NULL;
		break;
	}
	}

	size_t min_size = current_block_size < request_block_size
						  ? current_block_size
						  : request_block_size;
	if (request_block_size == current_block_size) {
		ret = ptr;
	} else {
		ret = malloc(size);
		ft_memcpy(ret, ptr, min_size);
		free(ptr);
	}
	return (ret);
}