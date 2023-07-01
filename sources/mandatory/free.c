#include "malloc.h"
#include "malloc_util.h"

/**
 * @fn free
 * @brief free memory
 * @param ptr memory address
 * @return void
 *
 * @note we have to abort on double free but we can not use abort() in this
 * project so, just return it. if you want to use abort(), just replace return
 * to abort() if (!IS_ALLOCATED(meta_data->header)
 */
void free(void *ptr) {
  void *block = ptr - sizeof(size_t);
  t_metadata *meta_data = (t_metadata *)block;
  t_arena *arena = &g_arena;

  if (IS_TINY_BLOCK(meta_data->header)) {
    __free_tiny_block(arena, block);
  } else if (IS_SMALL_BLOCK(meta_data->header)) {
    __free_small_block(arena, block);
  } else {
    __free_large_block(arena, block);
  }
}

/**
 * @fn __free_tiny_block
 * @brief free tiny block
 * @param arena arena
 * @param block block
 * @return void
 */
void __free_tiny_block(t_arena *arena, t_tiny_metadata *block) {
  t_tiny_metadata *free_block = arena->tiny_free_pool;

  arena->tiny_free_pool = block;
  __set_tiny_block(block,
                   SET_HEADER(GET_BLOCK_SIZE(block->header),
                              IS_PREV_USED(block->header) ? FLAG_PREV_USED : 0),
                   free_block);
}

/**
 * @fn __free_small_block
 * @brief free small block
 * @param arena arena
 * @param block block
 * @return void
 */
void __free_small_block(t_arena *arena, t_common_metadata *block) {
  t_common_metadata *free_block = arena->small_free_pool;

  arena->small_free_pool = block;
  if (free_block) {
    free_block->prev_block = block;
  }
  __set_small_block(
      block,
      SET_HEADER(GET_BLOCK_SIZE(block->header),
                 IS_PREV_USED(block->header) ? FLAG_PREV_USED : 0),
      free_block, NULL);
}

/**
 * @fn __free_large_block
 * @brief free large block
 * @param arena arena
 * @param block block
 * @return void
 */
void __free_large_block(t_arena *arena, t_common_metadata *block) {
  t_page_header *page = arena->large_page;
  t_page_header *prev_page = NULL;

  while (page) {
    if (page->first_block == block) {
      if (prev_page) {
        prev_page->next = page->next;
      } else {
        arena->large_page = page->next;
      }
      munmap(page, page->size);
      return;
    }
    prev_page = page;
    page = page->next;
  }
}