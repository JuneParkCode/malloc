#ifndef MALLOC_H
#define MALLOC_H

#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

// implement this
#include <stdlib.h>
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void show_alloc_mem(); // for debug and logging

// meta data is size + flags(3bit)
#define FLAG_BIT_SIZE 3
#define GET_HEADER(PTR) ((size_t *)(PTR - 1));
#define GET_SIZE(HEADER) (HEADER >> FLAG_BIT_SIZE)
#define IS_ASSIGNED(HEADER) (HEADER & 0b001)
#define IS_PREV_USED(HEADER) (HEADER & 0b010)
#define IS_THREADED(HEADER) (HEADER & 0b100)

// metadata structure for free tiny block
typedef struct s_tiny_metadata {
  size_t meta_data; // will be store size + flags(1byte)
  void *next_block;
} t_tiny_metadata;

// metadata structure for free small block. at last word, it has sizeof block
typedef struct s_common_metadata {
  size_t meta_data; // will be store size + flags(1byte)
  void *next_block;
  void *prev_block;
} t_common_metadata;

// default align size, block must be aligned in multiple of ALIGN_SIZE
#define MALLOC_ALIGN_SIZE (sizeof(size_t) * 2)
#define MALLOC_TINY_ALIGN_SIZE MALLOC_ALIGN_SIZE
// define metadata size
#define MALLOC_TINY_METADATA_SIZE (sizeof(t_tiny_metadata))
#define MALLOC_SMALL_METADATA_SIZE (sizeof(t_small_metadata) + sizeof(size_t))
// SMALL_META_DATA SIZE == sizeof(size_t) * 4 (header + footer)
#define MALLOC_SMALL_ALIGN_SIZE (MALLOC_TINY_SIZE << 5)
// N for TINY_SIZE_MAX
#define MALLOC_TINY_SIZE_MAX                                                   \
  (MALLOC_SMALL_ALIGN_SIZE - MALLOC_TINY_METADATA_SIZE)
// M for SMALL_SIZE_MAX
#define MALLOC_SMALL_SIZE_MAX                                                  \
  ((4 * getpagesize()) - (MALLOC_SMALL_METADATA_SIZE))
// POOL_SIZE = MAX_SIZE * 128 (can contain 128 blocks to maximum)
#define MALLOC_TINY_POOL_SIZE (MALLOC_TINY_SIZE_MAX * 128);
#define MALLOC_SMALL_POOL_SIZE (MALLOC_TINY_SIZE_MAX * 128);

// FOR ARENA
typedef struct s_arena {
  size_t arena_idx;
  void *tiny_pool;
  void *small_pool;
} t_arena;

// FOR BONUS_MALLOC, it can use another functions..
#ifndef _BONUS_MALLOC_
#define ARENA_SIZE 1
#else
#define ARENA_SIZE sysconf(_SC_NPROCESSORS_ONLN)
#endif
// ARENA
static t_arena G_ARENAS[ARENA_SIZE];
static int G_INIT_THREAD = -1;

#endif