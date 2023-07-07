#include "malloc_bonus.h"
#include "malloc_util_bonus.h"

extern __thread t_tcache __tcache;

/**
 * @fn malloc_flush_thread_cache
 * @brief flush thread cache in malloc. it affects to current thread. note that
 * it doesn't free allocated memory. just move thread cached to shared memory
 * pool
 * @return size of memory that flushed.
 */
size_t malloc_flush_thread_cache(void) {
  t_tcache *const cache = &__tcache;
  t_arena *const arena = __get_arena(cache);
  size_t flushed_memory = 0;

  pthread_mutex_lock(&arena->lock);
  flushed_memory += __flush_tiny_pool(arena, cache);
  flushed_memory += __flush_small_pool(arena, cache);
  pthread_mutex_unlock(&arena->lock);
  return (flushed_memory);
}

/**
 * @fn __flush_tiny_pool
 * @brief flush tiny pool in thread cache. it moves block from cache to arena
 * @return size of memory that flushed.
 */
size_t __flush_tiny_pool(t_arena *const arena, t_tcache *const cache) {
  t_tiny_metadata *block;
  t_tiny_metadata *next_block;
  size_t flushed_memory = 0;
  int flag = 0;

  block = cache->tiny_free_pool;
  while (block) {
    flag = 0;
    flushed_memory += GET_BLOCK_SIZE(block->header);
    next_block = block->next_block;
    flag = IS_PREV_USED(block->header) ? FLAG_PREV_USED : 0;
    __set_tiny_block(block, SET_HEADER(GET_BLOCK_SIZE(block->header), flag),
                     arena->tiny_free_pool);
    arena->tiny_free_pool = block;
    block = next_block;
  }
  cache->tiny_free_pool = NULL;
  return (flushed_memory);
}

/**
 * @fn __flush_small_pool
 * @brief flush tiny pool in thread cache. it moves block from cache to arena
 * @return size of memory that flushed.
 */
size_t __flush_small_pool(t_arena *const arena, t_tcache *const cache) {
  t_common_metadata *block;
  t_common_metadata *next_block;
  size_t flushed_memory = 0;
  int flag = 0;

  block = cache->small_free_pool;
  while (block) {
    flag = 0;
    flushed_memory += GET_BLOCK_SIZE(block->header);
    next_block = block->next_block;
    flag = IS_PREV_USED(block->header) ? FLAG_PREV_USED : 0;
    __set_small_block(block, SET_HEADER(GET_BLOCK_SIZE(block->header), flag),
                      arena->small_free_pool, 0);
    arena->small_free_pool->prev_block = block;
    arena->small_free_pool = block;
    block = next_block;
  }
  cache->tiny_free_pool = NULL;
  return (flushed_memory);
}