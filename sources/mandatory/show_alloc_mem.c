#include "malloc.h"
#include "malloc_util.h"
#include <stdio.h>

int __show_alloc_mem(t_page_header *page) {
  t_metadata *block;
  size_t total = 0;
  size_t block_size = 0;
  size_t page_block_sum;

  while (page) {
    printf("page: %p - %p size : %zu bytes\n", page->first_block,
           (char *)page->first_block + page->size, page->size);
    block = page->first_block;
    page_block_sum = 0;
    while (page_block_sum < page->size) {
      block_size = GET_BLOCK_SIZE(block->header);
      page_block_sum += block_size;
      if (block_size == 0)
        break;
      if (IS_ALLOCATED(block->header)) {
        printf("%p - %p : %zu bytes (allocated)\n", block,
               (char *)block + block_size, block_size);
        total += block_size;
      }
      block = (t_metadata *)((char *)block + block_size);
    }
    page = page->next;
  }
  return (total);
}

void show_alloc_mem(void) {
  t_arena *arena = &g_arena;
  size_t total = 0;

  printf("allocated memory blocks:\n");
  if (arena->tiny_page) {
    printf("TINY\n");
    total += __show_alloc_mem(arena->tiny_page);
  }
  if (arena->small_page) {
    printf("SMALL\n");
    total += __show_alloc_mem(arena->small_page);
  }
  if (arena->large_page) {
    printf("LARGE\n");
    total += __show_alloc_mem(arena->large_page);
  }
  printf("Total : %zu bytes\n", total);
}