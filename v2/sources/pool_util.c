#include "avl_tree.h"
#include "malloc_pool.h"

t_block *get_block_addr(t_metadata const *metadata, t_pool const *pool)
{
	size_t const align =
		pool->type == TINY ? MALLOC_TINY_SIZE_MIN : MALLOC_SMALL_SIZE_MIN;
	size_t const position =
		(size_t)add_addr(metadata, -(uintptr_t)pool->metadata);
	size_t const block_position = align * position;
	t_block *addr = (t_block *)add_addr(pool->addr, block_position);

	return addr;
}

// utilities
/**
 * @brief Set the metadata object
 *
 * @param is_allocated block allocation status
 * @param order block order
 * @param type block pool type
 * @return t_metadata metadata_result
 */
t_metadata set_metadata(bool is_allocated, size_t order)
{
	return (is_allocated << 7) | (order);
}

/**
 * @brief Get the order of block
 *
 * @param size size of block
 * @param type block pool type. ENUM POOL_TYPE (TINY | SMALL)
 * @return size_t order
 */
size_t get_order(size_t size, POOL_TYPE type)
{
	size_t const minimum_size =
		type == TINY ? MALLOC_TINY_SIZE_MIN : MALLOC_SMALL_SIZE_MIN;
	size_t current_size = minimum_size;
	size_t ret = 0;

	while (size > current_size) {
		current_size <<= 1;
		++ret;
	}
	return ret;
}

/**
 * @brief get block's allocation status
 *
 * @param metadata metadata of block
 * @return true
 * @return false
 */
bool is_allocated(t_metadata metadata)
{
	return metadata >> 7;
}

// use in free
size_t get_block_order(t_metadata metadata)
{
	return (metadata & 127); // 0b01111111
}

size_t get_block_size(t_metadata metadata, POOL_TYPE type)
{
	size_t min_size;

	switch (type) {
	case TINY:
		min_size = MALLOC_TINY_SIZE_MIN;
		break;
	case SMALL:
		min_size = MALLOC_SMALL_SIZE_MIN;
		break;
	case LARGE:
		return -1;
	}

	return min_size << get_block_order(metadata);
}

/**
 * @brief Get the align size object
 *
 * @param size size
 * @param align align. must be 2^n
 * @return size_t
 */
size_t get_align_size(size_t size, size_t align)
{
	return (size + align - 1) & ~(align - 1);
}

/**
 * @brief Get the block metadata.
 *
 * @param pool block pool
 * @param block target block
 * @return t_metadata*
 */
t_metadata *get_block_metadata(t_pool const *const pool,
							   t_block const *const block)
{
	size_t const min_size =
		pool->type == TINY ? MALLOC_TINY_SIZE_MIN : MALLOC_SMALL_SIZE_MIN;
	size_t const distance =
		((size_t)(add_addr(block, -(uintptr_t)pool->addr)) / min_size);
	void *const addr = add_addr(pool->metadata, distance);

	return addr;
}

/**
 * @brief Set the block metadata object
 *
 * @param is_allocated block allocation status
 * @param order block order
 * @param pool pool of block
 * @param block target block
 */
void set_block_metadata(bool is_allocated, size_t order, t_pool *const pool,
						t_block *const block)
{
	t_metadata *const metadata = get_block_metadata(pool, block);

	*metadata = set_metadata(is_allocated, order);
}

/**
 * @brief check pool has parameter block
 *
 * @param block target block
 * @param pool pool to find
 * @return true
 * @return false
 */
bool is_block_pool(t_block const *const block, t_pool const *const pool)
{
	uintptr_t start;
	uintptr_t end;

	start = (uintptr_t)pool->addr;
	end = (uintptr_t)pool->metadata;
	return (start <= (uintptr_t)block && (uintptr_t)block < end);
}

/**
 * @brief find block pool from manager.
 *
 * @param block target block
 * @param manager pool manager
 * @return t_pool* found pool
 */
t_pool *find_block_pool(t_block const *const block,
						t_mmanager const *const manager)
{
	return find_node((t_key_const)block, manager->head);
}

void *add_addr(void const *addr, ssize_t size)
{
	if (size < 0)
		return (void *)((uintptr_t)addr - (uintptr_t)(-size));
	return (void *)((uintptr_t)addr + (uintptr_t)size);
}