#include "malloc_util.h"

/**
 * @fn __init_small_pool
 * @brief init arena small pool
 * @param arena arena
 * @return initialized pool ptr, or NULL if failed
 */
void *__init_small_pool(t_arena *const arena) {
  void *const new_pool = mmap(0, MALLOC_SMALL_POOL_SIZE, PROT_READ | PROT_WRITE,
                              MAP_ANON | MAP_PRIVATE, -1, 0);
  t_page_header *alloc_pool = arena->small_page;

  if (new_pool == (void *)-1)
    return (NULL);
  if (alloc_pool) {
    while (alloc_pool->next)
      alloc_pool = alloc_pool->next;
    alloc_pool->next = new_pool;
  } else {
    arena->small_page = new_pool;
  }
  alloc_pool = new_pool;
  alloc_pool->first_block = new_pool + sizeof(t_page_header);
  alloc_pool->size = MALLOC_SMALL_POOL_SIZE - sizeof(t_page_header);
  return (__set_small_block(alloc_pool->first_block,
                            SET_HEADER(alloc_pool->size, 0), NULL, NULL));
}

/**
 * @fn __init_tiny_pool
 * @brief init arena tiny pool
 * @param arena arena
 * @return initialized pool ptr, or NULL if failed
 */
void *__init_tiny_pool(t_arena *const arena) {
  void *const new_pool = mmap(0, MALLOC_TINY_POOL_SIZE, PROT_READ | PROT_WRITE,
                              MAP_ANON | MAP_PRIVATE, -1, 0);
  t_page_header *alloc_pool = arena->tiny_page;

  if (new_pool == (void *)-1)
    return (NULL);
  if (alloc_pool) {
    while (alloc_pool->next)
      alloc_pool = alloc_pool->next;
    alloc_pool->next = new_pool;
  } else {
    arena->tiny_page = new_pool;
  }
  alloc_pool = new_pool;
  alloc_pool->first_block = new_pool + sizeof(t_page_header);
  alloc_pool->size = MALLOC_TINY_POOL_SIZE - sizeof(t_page_header);
  return (__set_tiny_block(alloc_pool->first_block,
                           SET_HEADER(alloc_pool->size, 0), NULL));
}

/**
 * @fn __append_small_pool
 * @brief append new arena small pool
 * @param arena arena
 * @return appended pool ptr, or NULL if failed
 */
void *__append_small_pool(t_arena *const arena) {
  t_common_metadata *new_pool_block = __init_small_pool(arena);
  t_common_metadata *head = arena->small_free_pool;

  if (new_pool_block == (void *)-1)
    return (NULL);
  if (head) {
    head->prev_block = new_pool_block;
    new_pool_block->next_block = head;
  }
  arena->small_free_pool = new_pool_block;
  return (new_pool_block);
}

/**
 * @fn __append_tiny_pool
 * @brief append new arena tiny pool
 * @return appended pool ptr, or NULL if failed
 */
void *__append_tiny_pool(t_arena *const arena) {
  t_tiny_metadata *new_pool_block = __init_tiny_pool(arena);
  t_tiny_metadata *head = arena->tiny_free_pool;

  if (new_pool_block == (void *)-1)
    return (NULL);
  if (head) {
    new_pool_block->next_block = head;
  }
  arena->tiny_free_pool = new_pool_block;
  return (new_pool_block);
}