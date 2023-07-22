#include "malloc_bonus.h"
#include "malloc_util_bonus.h"

int __show_alloc_mem(t_page_header *page)
	__attribute__((visibility("default")));

int __show_alloc_mem(t_page_header *page)
{
	t_metadata *block;
	size_t total = 0;
	size_t block_size = 0;
	size_t page_block_sum;

	while (page) {
		ft_print_page_info(page->first_block - sizeof(t_page_header),
						   (__uint8_t *)page->first_block + page->size,
						   page->size);
		block = page->first_block;
		page_block_sum = 0;
		while (page_block_sum < page->size) {
			block_size = GET_BLOCK_SIZE(block->header);
			page_block_sum += block_size;
			if (block_size == 0)
				break;
			if (IS_ALLOCATED(block->header)) {
				ft_print_allocated_block_info(
					block, (__uint8_t *)block + block_size, block_size);
				total += block_size;
			}
			block = (t_metadata *)((__uint8_t *)block + block_size);
		}
		page = page->next;
	}
	return (total);
}

void show_alloc_mem(void)
{
	t_arena *arena;
	size_t total = 0;

	ft_putstr("allocated memory blocks:\n");
	ft_putstr("TINY\n");
	for (int i = 0; i < MAX_ARENA_NUMBER; ++i) {
		arena = &g_arenas[i];
		pthread_mutex_lock(&arena->lock);
		if (arena->tiny_page) {
			ft_putstr("arena ");
			ft_putnbr(i);
			ft_putstr(" block info\n");
			total += __show_alloc_mem(arena->tiny_page);
		}
		pthread_mutex_unlock(&arena->lock);
	}
	ft_putstr("SMALL\n");
	for (int i = 0; i < MAX_ARENA_NUMBER; ++i) {
		arena = &g_arenas[i];
		pthread_mutex_lock(&arena->lock);
		if (arena->small_page) {
			ft_putstr("arena ");
			ft_putnbr(i);
			ft_putstr(" block info\n");
			total += __show_alloc_mem(arena->small_page);
		}
		pthread_mutex_unlock(&arena->lock);
	}
	ft_putstr("LARGE\n");
	for (int i = 0; i < MAX_ARENA_NUMBER; ++i) {
		arena = &g_arenas[i];
		pthread_mutex_lock(&arena->lock);
		if (arena->large_page) {
			ft_putstr("arena ");
			ft_putnbr(i);
			ft_putstr(" block info\n");
			total += __show_alloc_mem(arena->large_page);
		}
		pthread_mutex_unlock(&arena->lock);
	}
	ft_putstr("Total : ");
	ft_putnbr(total);
	ft_putstr("bytes\n");
}