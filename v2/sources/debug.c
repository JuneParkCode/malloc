#include "avl_tree.h"
#include "malloc_debug.h"
#include "malloc_pool.h"
#include "malloc_util.h"

// print all allocations
void print_allocations(t_mmanager const *manager)
{
	size_t allocation_size = 0;
	t_pool const *node = manager->head;

	while (node->left) {
		node = node->left;
	}

	while (node) {
		allocation_size += node->allocated_size;
		if (node->type == LARGE) {
			print_large_zone(node);
		} else {
			print_buddy_zone(node);
		}
		node = next_node(node);
	}
	ft_putstr("total : ");
	ft_putnbr(allocation_size);
	ft_putstr(" byte\n");
	ft_putstr("===== DONE =====\n");
}

// print buddy system zone
void print_buddy_zone(t_pool const *pool)
{
	print_pool_info(pool);
	print_pool_blocks_infos(pool);
}

// print large allocation zone
void print_large_zone(t_pool const *pool)
{
	print_pool_info(pool);
	ft_print_allocated_block_info(pool->addr, add_addr(pool->addr, pool->size),
								  pool->size);
}

// print pool information
void print_pool_info(t_pool const *pool)
{
	const char *TYPES[3] = {"TINY", "SMALL", "LARGE"};
	// EXAMPLE
	// ----------------------------------------
	// POOL [ LARGE ] (0x09ae1fa ~ 0x09be1fa)
	// ----------------------------------------
	// POOL_ALLOCATION_SIZE : 10240 byte
	// POOL_USER_SPACE_SIZE : 10240 byte
	// POOL_META_DATA_SIZE : 0 byte
	// POOL_USAGE : 100 % ( 10240 / 10240 )
	// USER_SPACE_ADDR : 0x09ae1fa ~ 0x09af1fa
	// METADATA_ADDR : 0x09ae1fa ~ 0x09be1fa
	// ----------------------------------------
	// 왜 printf 못쓰는거야~

	ft_putstr("----------------------------------------\n");
	// POOL [ LARGE ] (0x09ae1fa ~ 0x09be1fa)
	ft_putstr("POOL [");
	ft_putstr(TYPES[pool->type == TINY ? 0 : pool->type == SMALL ? 1 : 2]);
	ft_putstr("] (");
	ft_putaddr(pool->addr);
	ft_putstr(" ~ ");
	ft_putaddr(add_addr(pool->addr, pool->size));
	ft_putstr(")\n");
	ft_putstr("----------------------------------------\n");
	// POOL_ALLOCATION_SIZE : 10240 byte
	ft_putstr("POOL_ALLOCATION_SIZE : ");
	ft_putnbr(pool->size);
	ft_putstr(" byte\n");
	// POOL_USER_SPACE_SIZE : 10240 byte
	ft_putstr("POOL_USER_SPACE_SIZE : ");
	ft_putnbr(pool->user_space_size);
	ft_putstr(" byte\n");
	// POOL_META_DATA_SIZE : 0 byte
	ft_putstr("POOL_META_DATA_SIZE : ");
	ft_putnbr(pool->size - pool->user_space_size);
	ft_putstr(" byte\n");
	// POOL_USAGE : 100 % ( 10240 / 10240 )
	ft_putstr("POOL_USAGE : ");
	size_t p = ((float)pool->allocated_size / pool->user_space_size) * 10000;
	size_t n = p / 100;
	size_t f = p % 100;
	ft_putnbr(n);
	ft_putchar('.');
	ft_putnbr(f);
	ft_putstr(" \% ( ");
	ft_putnbr(pool->allocated_size);
	ft_putstr(" / ");
	ft_putnbr(pool->user_space_size);
	ft_putstr(" )\n");
	// USER_SPACE_ADDR : 0x09ae1fa ~ 0x09af1fa
	ft_putstr("USER_SPACE_ADDR : ");
	ft_putaddr(pool->addr);
	ft_putstr(" ~ ");
	ft_putaddr(add_addr(pool->addr, pool->user_space_size));
	ft_putchar('\n');

	if (pool->type != LARGE) {
		// METADATA_ADDR : 0x09ae1fa ~ 0x09be1fa
		ft_putstr("METADATA_ADDR : ");
		ft_putaddr(pool->metadata);
		ft_putstr(" ~ ");
		ft_putaddr(
			add_addr(pool->metadata, (pool->size - pool->user_space_size)));
		ft_putchar('\n');
	}
	ft_putstr("----------------------------------------\n");
}

// print all pool informations
void print_pools(t_mmanager const *manager)
{
	t_pool const *node = manager->head;

	while (node) {
		print_pool_info(node);
		node = next_node(node);
	}
}

// print all of pool block informations
void print_pool_blocks_infos(t_pool const *pool)
{
	t_metadata const *metadata = pool->metadata;
	size_t const min_size =
		pool->type == TINY ? MALLOC_TINY_SIZE_MIN : MALLOC_SMALL_SIZE_MIN;
	size_t block_size;
	size_t sum = 0;

	ft_putstr("----------------------------------------\n");
	while (sum < pool->user_space_size) {
		if (is_allocated(*metadata))
			print_metadata_info(metadata, pool);
		block_size = get_block_size(*metadata, pool->type);
		metadata = (t_metadata *)add_addr(metadata, block_size / min_size);
		sum += block_size;
	}
	ft_putstr("----------------------------------------\n");
}

// print block information
void print_block_info(t_block const *block, t_pool const *pool)
{
	t_metadata const *const metadata = get_block_metadata(pool, block);

	print_metadata_info(metadata, pool);
}

// print block information using metadata
void print_metadata_info(t_metadata const *metadata, t_pool const *pool)
{
	const char *ALLOC_STATUS[2] = {"free", "allocated"};
	size_t const size = get_block_size(*metadata, pool->type);
	size_t const order = get_block_order(*metadata);
	t_block const *const block_start = get_block_addr(metadata, pool);
	t_block const *const block_end = (t_block *)add_addr(block_start, size);

	// EXAMPLE
	// block: %p - %p : %u bytes (allocated) [order : %d] \n
	ft_putaddr(block_start);
	ft_putstr(" - ");
	ft_putaddr(block_end);
	ft_putstr(" : ");
	ft_putnbr(size);
	ft_putstr(" byte (");
	ft_putstr(ALLOC_STATUS[is_allocated(*metadata)]);
	ft_putstr(") ");
	ft_putstr(" [order : ");
	ft_putnbr(order);
	ft_putstr("] \n");
}

// print allocation information
void print_allocation_info(size_t request_size, t_block const *block,
						   t_pool const *pool)
{
	char const *const TYPES[3] = {"TINY", "SMALL", "LARGE"};
	t_metadata const *const metadata = get_block_metadata(pool, block);
	// EXAMPLE
	// ----------------------------------------
	//          ALLOCATION INFORMATION
	// ----------------------------------------
	// [ print block info() ]
	// REQUEST : 30
	// ALLOCATE : 32
	// POOL : TINY
	// ----------------------------------------
	ft_putstr("----------------------------------------\n");
	ft_putstr("         ALLOCATION INFORMATION \n");
	ft_putstr("----------------------------------------\n");
	print_block_info(block, pool);
	ft_putstr("REQUEST : ");
	ft_putnbr(request_size);
	ft_putstr(" byte\n");
	ft_putstr("ALLOCATE : ");
	ft_putnbr(get_block_size(*metadata, pool->type));
	ft_putstr(" byte\n");
	ft_putstr("POOL : ");
	ft_putstr(TYPES[pool->type == TINY ? 0 : pool->type == SMALL ? 1 : 2]);
	ft_putchar('\n');
	ft_putstr("----------------------------------------\n");
}