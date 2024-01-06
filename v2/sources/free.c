#include "malloc.h"
#include "malloc_pool.h"

extern pthread_mutex_t g_mutex;
/**
 * @brief free memory
 * @param ptr memory address
 * @note in exception case, it does not abort()
 */
void free(void *ptr)
{
	if (ptr == NULL)
		return; // abort();

	pthread_mutex_lock(&g_mutex);
	t_block *block = ptr;
	t_pool *pool = find_block_pool(ptr, &g_manager);

	if (pool == NULL)
		return; // abort();
	if (pool->type == LARGE) {
		remove_pool(pool, &g_manager);
		pthread_mutex_unlock(&g_mutex);
		return;
	}
	// buddy allocation case
	t_metadata *metadata = get_block_metadata(pool, block);
	size_t order = get_block_order(*metadata);
	size_t block_size = get_block_size(*metadata, pool->type);

	append_block(block, pool, order);
	pool->allocated_size -= get_block_size(*metadata, pool->type);
	*metadata = set_metadata(false, block_size, pool->type);
	merge_block(block, pool);

	// shrink process
	if (pool->allocated_size == 0)
		shrink_pool(pool, &g_manager);
	pthread_mutex_unlock(&g_mutex);
}
