#include "malloc_util_bonus.h"

/**
 * @fn __split_small_block
 * @brief split small block to free and alloc block, free block will be append
 * to pool
 * @param arena arena
 * @param block : alloc block
 * @param size alloc size
 * @note free_block always exist because it is created by exist memory block
 * @return new alloc block
 */
void *__split_small_block(t_arena *const arena, t_tcache *const cache,
                          void *const block, const size_t size) {
  t_common_metadata *free_block =
      (t_common_metadata *)((__uint8_t *)(block + size)); // splited free block
  size_t original_block_size = GET_BLOCK_SIZE(GET_HEADER(block));
  size_t free_block_size = original_block_size - size;

  if (IS_THREADED(free_block->header)) {
    __set_small_block(
        free_block, SET_HEADER(free_block_size, FLAG_PREV_USED | FLAG_THREADED),
        cache->small_free_pool, 0);
    if (cache->small_free_pool) {
      cache->small_free_pool->prev_block = free_block;
    }
    arena->small_free_pool = free_block;
  } else {
    pthread_mutex_lock(&arena->lock);
    __set_small_block(free_block, SET_HEADER(free_block_size, FLAG_PREV_USED),
                      arena->small_free_pool, 0);
    if (arena->small_free_pool) {
      arena->small_free_pool->prev_block = free_block;
    }
    arena->small_free_pool = free_block;
    pthread_mutex_unlock(&arena->lock);
  }
  return (block);
}

/**
 * @fn __split_tiny_block
 * @brief split tiny block to free and alloc block, free block will be append
 *to pool
 * @param arena arena
 * @param block : alloc block
 * @param size alloc size
 * @note free_block always exist because it is created by exist memory block
 * @return new alloc block
 */
void *__split_tiny_block(t_arena *const arena, t_tcache *const cache,
                         void *const block, const size_t size) {
  t_tiny_metadata *free_block =
      (t_tiny_metadata *)((__uint8_t *)(block + size)); // splited free block
  size_t original_block_size = GET_BLOCK_SIZE(GET_HEADER(block));
  size_t free_block_size = original_block_size - size;

  if (IS_THREADED(free_block->header)) {
    __set_tiny_block(
        free_block, SET_HEADER(free_block_size, FLAG_PREV_USED | FLAG_THREADED),
        cache->tiny_free_pool);
    free_block->next_block = cache->tiny_free_pool;
    cache->tiny_free_pool = free_block;
  } else {
    pthread_mutex_lock(&arena->lock);
    __set_tiny_block(free_block, SET_HEADER(free_block_size, FLAG_PREV_USED),
                     arena->tiny_free_pool);
    free_block->next_block = arena->tiny_free_pool;
    arena->tiny_free_pool = free_block;
    pthread_mutex_unlock(&arena->lock);
  }
  return (block);
}

/**
 * @fn __set_tiny_block
 * @brief set tiny block metadata
 * @param block : block
 * @param header : header
 * @param next_block : next block
 * @return metadata set memory
 */
void *__set_tiny_block(void *const block, const size_t header,
                       void *next_block) {
  t_tiny_metadata *meta_data = block;

  meta_data->header = header;
  if (!(IS_ALLOCATED(meta_data->header)))
    meta_data->next_block = next_block;
  return (block);
}

/**
 * @fn __set_small_block
 * @brief set small block metadata
 * @param block : block
 * @param header : header
 * @param next_block : next block
 * @param prev_block : prev block
 * @return metadata set memory
 */
void *__set_small_block(void *const block, const size_t header,
                        void *next_block, void *prev_block) {
  t_common_metadata *meta_data = block;
  size_t *block_footer = NULL; // footer will be last 8 byte of block
  size_t block_size;

  meta_data->header = header;
  if (!(IS_ALLOCATED(meta_data->header))) {
    meta_data->next_block = next_block;
    meta_data->prev_block = prev_block;
    block_size = GET_BLOCK_SIZE(meta_data->header);
    block_footer =
        (size_t *)((__uint8_t *)(block + block_size - sizeof(size_t)));
    *block_footer = block_size;
  }
  return (block);
}

/**
 * @fn __set_large_block
 * @brief set large block metadata
 * @param block : block
 * @param header : header
 * @return metadata set memory
 */
void *__set_large_block(void *const block, const size_t header) {
  size_t *block_header = (size_t *)(block);

  *block_header = header;
  return (block);
}

/**
 * @fn __allocate_tiny_block
 * @brief allocate tiny block from tiny pool
 * @param arena arena
 * @param size alloc size
 * @return allocated block, or NULL if failed
 */
void *__allocate_tiny_block(t_arena *const arena, t_tcache *const cache,
                            const size_t size) {
  void *free_block = __pop_free_tiny_block(arena, cache, size);

  if (!free_block) {
    if (__append_tiny_pool(arena) == NULL)
      return (NULL);
    return (__allocate_tiny_block(arena, cache, size));
  }
  if (GET_BLOCK_SIZE(GET_HEADER(free_block)) > size)
    free_block = __split_tiny_block(arena, cache, free_block, size);
  return (__set_tiny_block(free_block, SET_HEADER(size, FLAG_ALLOC), NULL));
}

/**
 * @fn __allocate_small_block
 * @brief allocate small block from small pool
 * @param arena arena
 * @return allocated block, or NULL if failed
 */
void *__allocate_small_block(t_arena *const arena, t_tcache *const cache,
                             const size_t size) {
  void *free_block = __pop_free_small_block(arena, cache, size);

  if (!free_block) {
    if (__append_small_pool(arena) == NULL)
      return (NULL);
    return (__allocate_small_block(arena, cache, size));
  }
  if (GET_BLOCK_SIZE(GET_HEADER(free_block)) > size)
    free_block = __split_small_block(arena, cache, free_block, size);
  return (
      __set_small_block(free_block, SET_HEADER(size, FLAG_ALLOC), NULL, NULL));
}

/**
 * @fn __allocate_large_block
 * @brief allocate large block
 * @param arena arena
 * @param size alloc size
 * @return allocated block, or NULL if failed
 */
void *__allocate_large_block(t_arena *const arena, const size_t size) {
  void *const alloc =
      mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  t_page_header *alloc_pool = NULL;

  if (alloc == (void *)-1)
    return (NULL);
  pthread_mutex_lock(&arena->lock);
  alloc_pool = __append_arena_large_page(arena, alloc, size);
  pthread_mutex_unlock(&arena->lock);
  return (
      __set_large_block(alloc_pool->first_block, SET_HEADER(size, FLAG_ALLOC)));
}

/**
 * @fn __pop_free_tiny_block
 * @brief pop tiny block in pool
 * @param arena arena
 * @param cache thread cache
 * @param size alloc size
 * @return allocated block, or NULL if failed (have to append pool)
 */
void *__pop_free_tiny_block(t_arena *const arena, t_tcache *const cache,
                            const size_t size) {
  // find in tcache first (lock free)
  void *ret = __pop_tiny_block_from_thread_cache(cache, size);

  if (!ret) {
    pthread_mutex_lock(&arena->lock);
    ret = __pop_free_tiny_block_from_arena(arena, size);
    pthread_mutex_unlock(&arena->lock);
  }
  return (ret);
}

/**
 * @fn __pop_free_small_block
 * @brief find small block in pool
 * @param arena arena
 * @param size alloc size
 * @return allocated block, or NULL if failed (have to append pool)
 */
void *__pop_free_small_block(t_arena *const arena, t_tcache *const cache,
                             const size_t size) {
  // find in tcache first (lock free)
  void *ret = __pop_small_block_from_thread_cache(cache, size);

  if (!ret) {
    pthread_mutex_lock(&arena->lock);
    ret = __pop_free_small_block_from_arena(arena, size);
    pthread_mutex_unlock(&arena->lock);
  }
  return (ret);
}

/**
 * @fn __get_request_block_size
 * @brief get request block size
 * @param size user request size
 * @return block size
 */
size_t __get_request_block_size(size_t size) {
  const size_t size_with_header = size + sizeof(size_t);

  switch (GET_SIZE_TYPE(size_with_header)) {
  case TINY:
    return (GET_MULTIPLE_SIZE(size_with_header, MALLOC_TINY_ALIGN_SIZE));
  case SMALL:
    return (GET_MULTIPLE_SIZE(size_with_header, MALLOC_SMALL_ALIGN_SIZE));
  case LARGE:
    return (GET_MULTIPLE_SIZE(size_with_header + sizeof(t_page_header),
                              MALLOC_LARGE_ALIGN_SIZE));
  }
  return (-1); // NEVER HAPPEN
}

/**
 * @fn __pop_free_tiny_block_from_arena
 * @brief pop free block from tiny pool in arena
 * @param arena arena of thread
 * @param size block size
 * @return free block or NULL if there are no free block
 */
void *__pop_free_tiny_block_from_arena(t_arena *arena, size_t size) {
  t_tiny_metadata *pool_block = arena->tiny_free_pool;
  void *ret = NULL;

  if (!pool_block)
    return (NULL);
  if (GET_BLOCK_SIZE(pool_block->header) >= size) {
    arena->tiny_free_pool = pool_block->next_block;
    return (pool_block);
  }
  while (pool_block->next_block &&
         GET_BLOCK_SIZE(pool_block->next_block->header) < size) {
    pool_block = pool_block->next_block;
  }
  if (!pool_block->next_block)
    return (NULL);
  ret = pool_block->next_block;
  pool_block->next_block = pool_block->next_block->next_block;
  return (ret);
}

/**
 * @fn __pop_free_small_block_from_arena
 * @brief pop free block from small pool in arena
 * @param arena arena of thread
 * @param size block size
 * @return free block or NULL if there are no free block
 */
void *__pop_free_small_block_from_arena(t_arena *arena, size_t size) {
  t_common_metadata *pool_block = arena->small_free_pool;
  void *ret = NULL;

  if (!pool_block)
    return (NULL);
  if (GET_BLOCK_SIZE(pool_block->header) >= size) {
    arena->small_free_pool = pool_block->next_block;
    if (pool_block->next_block)
      pool_block->next_block->prev_block = NULL;
    return (pool_block);
  }
  while (pool_block->next_block &&
         GET_BLOCK_SIZE(pool_block->next_block->header) < size) {
    pool_block = pool_block->next_block;
  }
  if (!pool_block->next_block)
    return (NULL);
  if (pool_block->next_block->next_block)
    pool_block->next_block->next_block->prev_block = pool_block;
  ret = pool_block->next_block;
  pool_block->next_block = pool_block->next_block->next_block;
  return (ret);
}

/**
 * @fn __pop_free_tiny_block_from_thread_cache
 * @brief pop free block from tiny pool in thread_cache
 * @param arena arena of thread
 * @param size block size
 * @return free block or NULL if there are no free block
 */
void *__pop_tiny_block_from_thread_cache(t_tcache *const cache, size_t size) {
  // get data from cache
  t_tiny_metadata *pool_block = cache->tiny_free_pool;
  void *ret = NULL;

  if (!pool_block)
    return (NULL);
  if (GET_BLOCK_SIZE(pool_block->header) >= size) {
    cache->tiny_free_pool = pool_block->next_block;
    return (pool_block);
  }
  while (pool_block->next_block &&
         GET_BLOCK_SIZE(pool_block->next_block->header) < size) {
    pool_block = pool_block->next_block;
  }
  if (!pool_block->next_block)
    return (NULL);
  ret = pool_block->next_block;
  pool_block->next_block = pool_block->next_block->next_block;
  return (ret);
}

/**
 * @fn __pop_free_small_block_from_thread_cache
 * @brief pop free block from small pool in thread_cache
 * @param arena arena of thread
 * @param size block size
 * @return free block or NULL if there are no free block
 */
void *__pop_small_block_from_thread_cache(t_tcache *const cache, size_t size) {
  t_common_metadata *pool_block = cache->small_free_pool;
  void *ret = NULL;

  if (!pool_block)
    return (NULL);
  if (GET_BLOCK_SIZE(pool_block->header) >= size) {
    cache->small_free_pool = pool_block->next_block;
    if (pool_block->next_block)
      pool_block->next_block->prev_block = NULL;
    return (pool_block);
  }
  while (pool_block->next_block &&
         GET_BLOCK_SIZE(pool_block->next_block->header) < size) {
    pool_block = pool_block->next_block;
  }
  if (!pool_block->next_block)
    return (NULL);
  if (pool_block->next_block->next_block)
    pool_block->next_block->next_block->prev_block = pool_block;
  ret = pool_block->next_block;
  pool_block->next_block = pool_block->next_block->next_block;
  return (ret);
}