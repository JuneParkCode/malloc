#include "malloc_debug.h"
#include "malloc_pool.h"
#include "malloc_util.h"
#include <stdlib.h>
/**
 * @brief allocate block from buddy pool
 *
 * @param size request size.
 * @param manager pool manager
 * @param type enum POOL_TYPE
 * @return void* allocated block. its size aligned by pool minimum block size
 */
void *allocate_buddy_block(size_t size, t_mmanager *const manager,
						   POOL_TYPE type)
{
	size_t request_size;
	t_pool *pool;

	switch (type) {
	case TINY:
		request_size = MALLOC_TINY_SIZE_MIN << get_order(size, type);
		pool = manager->tiny_pool_head;
		break;
	case SMALL:
		request_size = MALLOC_SMALL_SIZE_MIN << get_order(size, type);
		pool = manager->small_pool_head;
		break;
	case LARGE:
		return NULL;
	}

	// operations
	void *ret = NULL;

	while (pool != NULL) {
		if ((ret = get_block_from_pool(pool, request_size))) {
#ifdef DEBUG
			print_allocation_info(size, ret, pool);
#endif
			return ret;
		}
		pool = pool->next;
	}
	// when no space to allocate
	t_pool *const new_pool = create_buddy_pool(manager, type);
	if (new_pool == NULL)
		return NULL;

	append_pool(new_pool, manager);
	ret = get_block_from_pool(new_pool, request_size);
#ifdef DEBUG
	print_allocation_info(size, ret, new_pool);
#endif
	return ret;
}

/**
 * @brief allocate tiny block.
 *
 * @param size request size
 * @param manager pool manager
 * @return void* allocated block. its size aligned by pool minimum block size
 */
void *allocate_tiny_block(size_t size, t_mmanager *const manager)
{
	return allocate_buddy_block(size, manager, TINY);
}

/**
 * @brief allocate small block
 *
 * @param size request size
 * @param manager pool manager
 * @return void* allocated block. its size aligned by pool minimum block size
 */
void *allocate_small_block(size_t size, t_mmanager *const manager)
{
	return allocate_buddy_block(size, manager, SMALL);
}

/**
 * @brief allocate large block
 *
 * @param size request size
 * @param manager pool manager
 * @return void* allocated block. its size aligned by pool minimum block size
 */
void *allocate_large_block(size_t size, t_mmanager *const manager)
{
	t_pool *const allocated = create_large_pool(size, manager);
	void *const ret = allocated->addr;

	append_pool(allocated, manager);
	return ret;
}

/**
 * @brief Create a buddy pool object
 *
 * @param manager pool manager
 * @param type POOL_TYPE (TINY | SMALL)
 * @return t_pool* | NULL. It returns allocated pool object.
 * @note it must be free by @ref pfree(void *ptr, t_pmalloc_space *const space)
 */
t_pool *create_buddy_pool(t_mmanager *const manager, POOL_TYPE type)
{
	size_t allocation_size;
	size_t pool_size;
	size_t block_max_size;

	switch (type) {
	case TINY:
		allocation_size = MALLOC_TINY_ALLOC_SIZE;
		pool_size = MALLOC_TINY_POOL_SIZE;
		block_max_size = MALLOC_TINY_SIZE_MAX;
		break;
	case SMALL:
		allocation_size = MALLOC_SMALL_ALLOC_SIZE;
		pool_size = MALLOC_SMALL_POOL_SIZE;
		block_max_size = MALLOC_SMALL_SIZE_MAX;
		break;
	case LARGE:
		return NULL;
	}

	// allocate pool
	t_pool *const pool = pmalloc(manager->pmalloc_space);
	if (pool == NULL)
		return NULL;
	void *const addr = mmap(0, allocation_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (addr == NULL)
		return NULL;

	// initialize pool block data
	t_metadata *const metadata_block = (t_metadata *)add_addr(addr, pool_size);

	for (int i = 0; i < MAX_ORDER; ++i) {
		pool->free_list[i] = NULL;
	}
	pool->addr = addr;
	pool->metadata = metadata_block;
	pool->size = allocation_size;
	pool->allocated_size = 0;
	pool->max_size = pool_size;
	pool->free_list[MAX_ORDER - 1] = addr;
	pool->type = type;

	// initialize pool block
	t_block *block = pool->addr;
	while ((void *)block < (void *)metadata_block) {
		block->next = (t_block *)add_addr(block, block_max_size);
		set_block_metadata(false, block_max_size, pool, block);
		block = block->next;
	}
	// init last block
	block = add_addr(metadata_block, -block_max_size);
	block->next = NULL;
	return pool;
}

/**
 * @brief Create a tiny pool object
 *
 * @param manager pool manager
 * @return t_pool* allocated pool.
 * @note it must be free by @ref pfree(void *ptr, t_pmalloc_space *const space)
 */
t_pool *create_tiny_pool(t_mmanager *const manager)
{
	return create_buddy_pool(manager, TINY);
}

/**
 * @brief Create a small pool object
 *
 * @param manager pool manager
 * @return t_pool* allocated pool.
 * @note it must be free by @ref pfree(void *ptr, t_pmalloc_space *const space)
 */
t_pool *create_small_pool(t_mmanager *const manager)
{
	return create_buddy_pool(manager, SMALL);
}

/**
 * @brief Create a large pool object
 *
 * @param manager pool manager
 * @return t_pool* allocated pool.
 * @note it must be free by @ref pfree(void *ptr, t_pmalloc_space *const space)
 */
t_pool *create_large_pool(size_t size, t_mmanager *const manager)
{
	// allocate pool
	size_t const allocation_size = get_align_size(size, PAGE_SIZE);
	t_pool *const pool_data = pmalloc(manager->pmalloc_space);
	void *const addr = mmap(0, allocation_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);

	pool_data->size = allocation_size;
	pool_data->addr = addr;
	pool_data->metadata = NULL;
	pool_data->type = LARGE;
	pool_data->allocated_size = allocation_size;
	pool_data->max_size = allocation_size;
	append_pool(pool_data, manager);
	return pool_data;
}

// shrink pool (when pool is empty)
void shrink_pool(t_pool *const pool, t_mmanager *const manager)
{
	t_pool **head;
	size_t free_pool_count = 0;

	switch (pool->type) {
	case TINY:
		head = &(manager->tiny_pool_head);
		break;
	case SMALL:
		head = &(manager->small_pool_head);
		break;
	case LARGE:
		return; // nothing todo
	}

	t_pool *node = *head;

	while (node != NULL) {
		if (node->allocated_size == 0)
			++free_pool_count;
		node = node->next;
	}
	if (free_pool_count > 2)
		remove_pool(pool, manager);
}

/**
 * @brief append pool to pool list in manager
 *
 * @param pool pool object
 * @param manager pool manager
 */
void append_pool(t_pool *const pool, t_mmanager *const manager)
{
	switch (pool->type) {
	case TINY:
		pool->next = manager->tiny_pool_head;
		manager->tiny_pool_head = pool;
		break;
	case SMALL:
		pool->next = manager->small_pool_head;
		manager->small_pool_head = pool;
		break;
	case LARGE:
		pool->next = manager->large_pool_head;
		manager->large_pool_head = pool;
		break;
	}
#ifdef DEBUG
	ft_putstr("append pool\n");
	print_pools(&g_manager);
#endif
}

/**
 * @brief remove block from pool
 *
 * @param block target to remove
 * @param pool block's pool
 * @param order block's order
 * @return void* if success, it returns block. else, returns NULL
 */
void *remove_block_from_pool(t_block *const block, t_pool *const pool,
							 size_t order)
{
	t_block *current_block = pool->free_list[order];

	if (current_block == block) {
		pool->free_list[order] = pool->free_list[order]->next;
		return block;
	}

	while (current_block->next != NULL) {
		if (current_block->next == block) {
			current_block->next = block->next;
			return block;
		}
		current_block = current_block->next;
	}
	return NULL;
}

/**
 * @brief split the block in half and update it to the free list.
 *
 * @param pool pool
 * @param order order of block
 */
void split_block(t_pool *const pool, size_t order)
{
	size_t const minimum_block_size =
		pool->type == TINY ? MALLOC_TINY_SIZE_MIN : MALLOC_SMALL_SIZE_MIN;
	size_t const split_size = minimum_block_size << (order - 1);
	t_block *const left_block = pool->free_list[order];
	t_block *const right_block = (t_block *)add_addr(left_block, split_size);

	remove_block_from_pool(left_block, pool, order);
	set_block_metadata(false, split_size, pool, left_block);
	set_block_metadata(false, split_size, pool, right_block);
	// append to free list
	left_block->next = right_block;
	right_block->next = pool->free_list[order - 1];
	pool->free_list[order - 1] = left_block;
}

/**
 * @brief Get the block from pool object
 *
 * @param pool pool
 * @param request_size allocation request size. must be aligned.
 * @return void* allocated block. if failed, it returns NULL
 */
void *get_block_from_pool(t_pool *const pool, size_t request_size)
{
	size_t const order = get_order(request_size, pool->type);
	size_t current_order = order;
	t_block *free_block;

	while (current_order < MAX_ORDER) {
		free_block = pool->free_list[current_order];
		// not found
		if (free_block == NULL) {
			++current_order;
			continue;
		}
		// found
		if (order == current_order) {
			if (remove_block_from_pool(free_block, pool, current_order) == NULL)
				return NULL;
			set_block_metadata(true, request_size, pool, free_block);
			pool->allocated_size += request_size;
			return free_block;
		}
		// found but too large to allocate
		if (order < current_order) {
			split_block(pool, current_order);
			--current_order;
		}
	}
	return NULL;
}

void remove_pool(t_pool *pool, t_mmanager *manager)
{
	t_pool **head;

	switch (pool->type) {
	case TINY:
		head = &(manager->tiny_pool_head);
		break;
	case SMALL:
		head = &(manager->small_pool_head);
		break;
	case LARGE:
		head = &(manager->large_pool_head);
		break;
	}

	t_pool *node = *head;

	if (node == pool) {
		*head = node->next;
		pfree(pool, manager->pmalloc_space);
		return;
	}
	// find previous node and replace next
	while (node->next != NULL) {
		if (node->next == pool) {
			node->next = pool->next;
			pfree(pool, manager->pmalloc_space);
			return;
		}
		node = node->next;
	}
	// abort();
}

void merge_block(t_block *const block, t_pool *const pool)
{
	t_metadata *const metadata = get_block_metadata(pool, block);
	size_t const block_size = get_block_size(*metadata, pool->type);
	size_t const bucket_size =
		pool->type == TINY ? MALLOC_TINY_SIZE_MAX : MALLOC_SMALL_SIZE_MAX;
	size_t const position =
		((size_t)add_addr(block, -(uintptr_t)pool->addr)) % bucket_size;
	size_t const order = get_block_order(*metadata);
	t_block *left_block;
	t_block *right_block;
	size_t target_block_size;

	if (order >= MAX_ORDER - 1)
		return;
	if (position & block_size) {
		left_block = (t_block *)add_addr(block, -block_size);
		right_block = block;
		target_block_size =
			get_block_size(*get_block_metadata(pool, left_block), pool->type);
		if (is_allocated(*get_block_metadata(pool, left_block)) ||
			target_block_size != block_size)
			return;
	} else {
		left_block = block;
		right_block = (t_block *)add_addr(block, +block_size);
		target_block_size =
			get_block_size(*get_block_metadata(pool, right_block), pool->type);
		if (is_allocated(*get_block_metadata(pool, right_block)) ||
			target_block_size != block_size)
			return;
	}
#ifdef DEBUG
	ft_putstr("MERGE BEFORE\n");
	print_pool_info(pool);
	print_pool_blocks_infos(pool);
	ft_putstr("left : ");
	ft_putaddr(left_block);
	ft_putstr("right : ");
	ft_putaddr(right_block);
	ft_putstr("\n");
#endif
	remove_block_from_pool(left_block, pool, order);
	remove_block_from_pool(right_block, pool, order);
	set_block_metadata(false, block_size << 1, pool, left_block);
	set_block_metadata(false, block_size << 1, pool, right_block);
	append_block(left_block, pool, order + 1);
#ifdef DEBUG
	ft_putstr("MERGE AFTER\n");
	print_pool_info(pool);
	print_pool_blocks_infos(pool);
#endif
	merge_block(left_block, pool);
}

void append_block(t_block *const block, t_pool *const pool, size_t order)
{
	block->next = pool->free_list[order];
	pool->free_list[order] = block;
}