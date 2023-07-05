#include "malloc_bonus.h"
#include "malloc_util_bonus.h"
#include <stdio.h>

extern __thread t_tcache __tcache;

void free(void *ptr) __attribute__((visibility("default")));

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
  if (ptr == NULL)
    return;

  void *const block = ptr - sizeof(size_t);
  t_metadata *const meta_data = (t_metadata *)block;
  t_tcache *const cache = &__tcache;
  t_arena *const arena = __get_arena(cache);

  pthread_mutex_lock(&arena->lock);
  if (IS_TINY_BLOCK(meta_data->header)) {
    __free_tiny_block(arena, cache, block);
  } else if (IS_SMALL_BLOCK(meta_data->header)) {
    __free_small_block(arena, cache, block);
  } else {
    __free_large_block(arena, block);
  }
  pthread_mutex_unlock(&arena->lock);
}

/**
 * @fn __free_tiny_block
 * @brief free tiny block
 * @details
 * have to check it is allocated in same arena
 *  if allocated in same arena, pass it to thread cache
 *  if not, free back to original areana
 * @param arena arena
 * @param block block
 * @return void
 */
void __free_tiny_block(t_arena *arena, t_tcache *cache,
                       t_tiny_metadata *block) {
  int flag = (IS_PREV_USED(block->header) ? FLAG_PREV_USED : 0);
  t_arena *block_original_arena =
      __find_tiny_block_original_arena(arena, (t_metadata *)block);

  if (block_original_arena == NULL) {
    return; // abort() in reference malloc
  }
  if (block_original_arena == arena) {
    flag |= IS_THREADED(block->header) ? FLAG_THREADED : 0;
    __set_tiny_block(block, SET_HEADER(GET_BLOCK_SIZE(block->header), flag),
                     cache->tiny_free_pool);
    cache->tiny_free_pool = block;
  } else {
    __set_tiny_block(block, SET_HEADER(GET_BLOCK_SIZE(block->header), flag),
                     block_original_arena->tiny_free_pool);
    block_original_arena->tiny_free_pool = block;
  }
}

/**
 * @fn __free_small_block
 * @brief free small block
 * @param arena arena
 * @param block block
 * @return void
 */
void __free_small_block(t_arena *arena, t_tcache *cache,
                        t_common_metadata *block) {
  int flag = (IS_PREV_USED(block->header) ? FLAG_PREV_USED : 0);
  t_arena *block_original_arena =
      __find_small_block_original_arena(arena, (t_metadata *)block);

  if (block_original_arena == NULL) {
    return; // abort() in reference malloc
  }
  if (block_original_arena == arena) {
    flag |= IS_THREADED(block->header) ? FLAG_THREADED : 0;
    __set_small_block(block, SET_HEADER(GET_BLOCK_SIZE(block->header), flag),
                      cache->small_free_pool, NULL);
    if (cache->small_free_pool)
      cache->small_free_pool->prev_block = NULL;
    cache->small_free_pool = block;
  } else {
    __set_small_block(block, SET_HEADER(GET_BLOCK_SIZE(block->header), flag),
                      block_original_arena->small_free_pool, NULL);
    if (block_original_arena->small_free_pool)
      block_original_arena->small_free_pool->prev_block = NULL;
    block_original_arena->small_free_pool = block;
  }
}

/**
 * @fn __free_large_block
 * @brief free large block
 * @param arena arena
 * @param block block
 * @return void
 */
void __free_large_block(t_arena *arena, t_common_metadata *block) {
  t_arena *block_original_arena = NULL;
  t_page_header *page = NULL;
  t_page_header *prev_page = NULL;
  int flag = 0;

  block_original_arena =
      __find_large_block_original_arena(arena, (t_metadata *)block);
  if (!block_original_arena)
    return;
  page = block_original_arena->large_page;
  flag = (IS_PREV_USED(block->header) ? FLAG_PREV_USED : 0);
  __set_large_block(block, SET_HEADER(GET_BLOCK_SIZE(block->header), flag));
  while (page) {
    if (page->first_block == block) {
      if (prev_page) {
        prev_page->next = page->next;
      } else {
        block_original_arena->large_page = page->next;
      }
      munmap(page, page->size);
      return;
    }
    prev_page = page;
    page = page->next;
  }
}