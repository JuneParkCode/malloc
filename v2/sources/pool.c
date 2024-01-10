#include "avl_tree.h"
#include "malloc_debug.h"
#include "malloc_pool.h"

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
	size_t order;
	void *ret;

	switch (type) {
	case TINY:
		order = get_order(size, type);
		ret = get_block_from_list(manager, order, type);
		if (ret)
			return ret;
		break;
	case SMALL:
		order = get_order(size, type);
		ret = get_block_from_list(manager, order, type);
		if (ret)
			return ret;
		break;
	case LARGE:
		return NULL;
	}

	// when no space to allocate
	t_pool *const new_pool = create_buddy_pool(manager, type);
	if (new_pool == NULL)
		return NULL;

	append_pool(new_pool, manager);
	return get_block_from_list(manager, order, type);
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
	t_pool *const pool = create_large_pool(size);
	if (pool == NULL)
		return NULL;
	void *const ret = pool->addr;

	append_pool(pool, manager);
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
	size_t max_order;

	switch (type) {
	case TINY:
		allocation_size = get_align_size(MALLOC_TINY_ALLOC_SIZE, PAGE_SIZE);
		pool_size = MALLOC_TINY_POOL_SIZE;
		max_order = MAX_ORDER_TINY;
		break;
	case SMALL:
		allocation_size = get_align_size(MALLOC_SMALL_ALLOC_SIZE, PAGE_SIZE);
		pool_size = MALLOC_SMALL_POOL_SIZE;
		max_order = MAX_ORDER_SMALL;
		break;
	case LARGE:
		return NULL;
	}

	// allocate pool
	void *const addr = mmap(0, allocation_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (addr == MAP_FAILED)
		return NULL;
	t_pool *const pool = pmalloc();
	if (pool == NULL) {
		munmap(addr, allocation_size);
		return NULL;
	}

	// initialize pool block data
	t_metadata *const metadata_block = (t_metadata *)add_addr(addr, pool_size);

	pool->parent = NULL;
	pool->left = NULL;
	pool->right = NULL;
	pool->height = 1;
	pool->addr = addr;
	pool->metadata = metadata_block;
	pool->size = allocation_size;
	pool->allocated_size = 0;
	pool->user_space_size = pool_size;
	pool->type = type;

	// initialize pool block
	t_block *block = pool->addr;
	set_block_metadata(false, max_order - 1, pool, block);
	append_block(block, manager, pool->type, max_order - 1);
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
t_pool *create_large_pool(size_t size)
{
	// allocate pool
	size_t const allocation_size = get_align_size(size, PAGE_SIZE);
	void *const addr = mmap(0, allocation_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (addr == MAP_FAILED)
		return NULL;
	t_pool *const pool = pmalloc();
	if (pool == NULL) {
		munmap(addr, allocation_size);
		return NULL;
	}

	pool->parent = NULL;
	pool->left = NULL;
	pool->right = NULL;
	pool->height = 1;
	pool->addr = addr;
	pool->metadata = NULL;
	pool->type = LARGE;
	pool->size = allocation_size;
	pool->allocated_size = allocation_size;
	pool->user_space_size = allocation_size;

	return pool;
}

/**
 * @brief remove unused pool
 *
 * @param pool
 * @param manager
 */
void shrink_pool(t_pool *const pool, t_mmanager *const manager)
{
	// In the case of previously existing free_pools, they remain unused for a
	// long time. Therefore, it is more beneficial for performance to remove the
	// previously existing free_pool.
	t_pool *delete_pool = NULL;
	t_block *block = NULL;
	t_block **free_list = NULL;

	switch (pool->type) {
	case TINY:
		delete_pool = manager->tiny_free_pool;
		manager->tiny_free_pool = pool;
		// delete all blocks
		free_list = &manager->tiny_free_list[MAX_ORDER_TINY - 1];
		break;
	case SMALL:
		delete_pool = manager->small_free_pool;
		manager->small_free_pool = pool;
		free_list = &manager->small_free_list[MAX_ORDER_SMALL - 1];
		break;
	case LARGE:
		return; // nothing todo
	}
	if (delete_pool != NULL) {
		block = delete_pool->addr;
		remove_block_from_list(block, free_list);
		remove_node(delete_pool, &manager->head);
	}
}

/**
 * @brief append pool to pool list in manager
 *
 * @note it must be called when free pool is not exist
 * @param pool pool object
 * @param manager pool manager
 * @return t_node * (head)
 */
void append_pool(t_pool *const pool, t_mmanager *const manager)
{
	insert_node(pool, &manager->head);

	switch (pool->type) {
	case TINY:
		manager->tiny_free_pool = pool;
		break;
	case SMALL:
		manager->small_free_pool = pool;
		break;
	case LARGE:
		return;
	}
}

/**
 * @brief remove block from pool
 *
 * @param block target to remove
 * @param pool block's pool
 * @param order block's order
 * @return void* if success, it returns block. else, returns NULL
 */
void *remove_block_from_list(t_block *const block, t_block **free_list)
{
	t_block *current_block = *free_list; // head

	if (current_block == block) {
		*free_list = (*free_list)->next;
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
void split_block(t_pool *const pool, size_t order, t_block *free_lists[])
{
	size_t const minimum_block_size =
		pool->type == TINY ? MALLOC_TINY_SIZE_MIN : MALLOC_SMALL_SIZE_MIN;
	size_t const split_size = minimum_block_size << (order - 1);
	t_block *const left_block = free_lists[order];
	t_block *const right_block = (t_block *)add_addr(left_block, split_size);

	remove_block_from_list(left_block, &free_lists[order]);
	set_block_metadata(false, order - 1, pool, left_block);
	set_block_metadata(false, order - 1, pool, right_block);
	// append to free list
	left_block->next = right_block;
	right_block->next = free_lists[order - 1];
	free_lists[order - 1] = left_block;
}

/**
 * @brief Get the block from list object
 *
 * @param manager
 * @param order
 * @param type
 * @return void* block
 */
void *get_block_from_list(t_mmanager *manager, size_t order, POOL_TYPE type)
{
	size_t max_order;
	size_t min_block_size;
	t_block *free_block;
	t_block **free_lists;
	t_pool **free_pool;

	switch (type) {
	case TINY:
		max_order = MAX_ORDER_TINY;
		min_block_size = MALLOC_TINY_SIZE_MIN;
		free_lists = manager->tiny_free_list;
		free_pool = &manager->tiny_free_pool;
		break;
	case SMALL:
		max_order = MAX_ORDER_SMALL;
		min_block_size = MALLOC_SMALL_SIZE_MIN;
		free_lists = manager->small_free_list;
		free_pool = &manager->small_free_pool;
		break;
	case LARGE:
		return NULL;
	}

	size_t current_order = order;
	t_pool *pool;
	while (current_order < max_order) {
		free_block = free_lists[current_order];
		// not found
		if (free_block == NULL) {
			++current_order;
			continue;
		}
		// found
		if (order == current_order) {
			pool = find_block_pool(free_block, manager);
			if (remove_block_from_list(free_block, &free_lists[order]) == NULL)
				return NULL;
			set_block_metadata(true, order, pool, free_block);
			pool->allocated_size += (min_block_size << order);
			if (pool == *free_pool)
				*free_pool = NULL;
			return free_block;
		}
		// found but too large to allocate
		if (order < current_order) {
			pool = find_block_pool(free_block, manager);
			split_block(pool, current_order, free_lists);
			--current_order;
		}
	}
	return NULL;
}

void remove_pool(t_pool *pool, t_mmanager *manager)
{
	t_pool **head = &manager->head;

	remove_node(pool, head);
	pfree(pool);
	return;
	// abort();
}

void merge_block(t_block *const block, t_pool *const pool,
				 t_mmanager *const manager)
{
	t_metadata *const metadata = get_block_metadata(pool, block);
	size_t const block_size = get_block_size(*metadata, pool->type);
	size_t const bucket_size =
		pool->type == TINY ? MALLOC_TINY_POOL_SIZE : MALLOC_SMALL_POOL_SIZE;
	size_t const position =
		((size_t)add_addr(block, -(uintptr_t)pool->addr)) % bucket_size;
	size_t const order = get_block_order(*metadata);
	size_t const max_order =
		pool->type == TINY ? MAX_ORDER_TINY : MAX_ORDER_SMALL;
	t_block *left_block;
	t_block *right_block;
	t_block **free_lists =
		pool->type == TINY ? manager->tiny_free_list : manager->small_free_list;
	size_t target_block_size;

	if (order >= max_order - 1)
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
	remove_block_from_list(left_block, &free_lists[order]);
	remove_block_from_list(right_block, &free_lists[order]);
	set_block_metadata(false, order + 1, pool, left_block);
	set_block_metadata(false, order + 1, pool, right_block);
	append_block(left_block, manager, pool->type, order + 1);
	merge_block(left_block, pool, manager);
}

void append_block(t_block *block, t_mmanager *manager, POOL_TYPE type,
				  size_t order)
{
	if (type == TINY) {
		block->next = manager->tiny_free_list[order];
		manager->tiny_free_list[order] = block;
	} else if (type == SMALL) {
		block->next = manager->small_free_list[order];
		manager->small_free_list[order] = block;
	}
}