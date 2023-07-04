#include "malloc_bonus.h"
#include "malloc_util_bonus.h"

int __show_alloc_mem(t_page_header *page) {
  ft_putstr(__FUNCTION__);
  ft_putchar('\n');
  t_metadata *block;
  size_t total = 0;
  size_t block_size = 0;
  size_t page_block_sum;

  while (page) {
    ft_print_page_info(page->first_block,
                       (__uint8_t *)page->first_block + page->size, page->size);
    block = page->first_block;
    page_block_sum = 0;
    while (page_block_sum < page->size) {
      block_size = GET_BLOCK_SIZE(block->header);
      page_block_sum += block_size;
      if (block_size == 0)
        break;
      if (IS_ALLOCATED(block->header)) {
        ft_print_block_info(block, (__uint8_t *)block + block_size, block_size);
        total += block_size;
      }
      block = (t_metadata *)((__uint8_t *)block + block_size);
    }
    page = page->next;
  }
  return (total);
}

void show_alloc_mem(void) {
  ft_putstr(__FUNCTION__);
  ft_putchar('\n');
  t_arena *arena;
  size_t total = 0;

  ft_putstr("allocated memory blocks:\n");
  ft_putstr("TINY\n");
  for (int i = 0; i < MAX_ARENA_NUMBER; ++i) {
    arena = &g_arenas[i];
    if (arena->tiny_page) {
      total += __show_alloc_mem(arena->tiny_page);
    }
  }
  ft_putstr("SMALL\n");
  for (int i = 0; i < MAX_ARENA_NUMBER; ++i) {
    arena = &g_arenas[i];
    if (arena->small_page) {
      total += __show_alloc_mem(arena->small_page);
    }
  }
  ft_putstr("LARGE\n");
  for (int i = 0; i < MAX_ARENA_NUMBER; ++i) {
    arena = &g_arenas[i];
    if (arena->large_page) {
      total += __show_alloc_mem(arena->large_page);
    }
  }
  ft_putstr("Total : ");
  ft_putnbr(total);
  ft_putstr("bytes\n");
}