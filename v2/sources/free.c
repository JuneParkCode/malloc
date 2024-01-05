#include "malloc.h"
#include "malloc_debug.h"
#include "malloc_pool.h"
#include "malloc_util.h"

extern pthread_mutex_t g_mutex;
/**
 * @brief free memory
 * @param ptr memory address
 * @note in exception case, it does not abort()
 */
void free(void *ptr)
{
#ifdef DEBUG
	ft_putstr("free called\n");
	ft_putstr("addr : ");
	ft_putaddr(ptr);
	ft_putchar('\n');
#endif
	if (ptr == NULL)
		return; // abort();
	t_block *block = ptr;
	t_pool *pool = find_block_pool(ptr, &g_manager);

	if (pool == NULL)
		return; // abort();
	if (pool->type == LARGE) {
		remove_pool(pool, &g_manager);
#ifdef DEBUG
		print_allocations(&g_manager);
#endif
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
#ifdef DEBUG
	ft_putstr("free size : ");
	ft_putnbr(block_size);
	ft_putchar('\n');
	print_allocations(&g_manager);
#endif
}
