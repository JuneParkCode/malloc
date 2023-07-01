#ifndef MALLOC_UTIL_H
#define MALLOC_UTIL_H

// debug
#include <stdio.h>

#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

// FLAG for header
// FLAG_ALLOC : 1bit, FLAG_PREV_USED : 1bit, FLAG_THREADED : 1bit
// meta data is size + flags(3bit)
#define FLAG_BIT_SIZE 3
// FLAG_ALLOC : allocated = 1, free = 0
#define FLAG_ALLOC (0b001)
// FLAG_PREV_USED : prev block is used = 1, free = 0
#define FLAG_PREV_USED (0b010)
// FLAG_THREADED : threaded = 1, not threaded = 0
#define FLAG_THREADED (0b100)
// MACRO FOR GET HEADER
#define GET_HEADER(BLOCK) (*((size_t *)(BLOCK)))
// MACRO FOR SET HEADER (SET META DATA)
#define SET_HEADER(SIZE, FLAGS) (((SIZE) << FLAG_BIT_SIZE) | (FLAGS))
// below macro functions check by block header (NOT BLOCK!)
// GET_BLOCK_SIZE : get block size from header
#define GET_BLOCK_SIZE(HEADER) ((HEADER) >> FLAG_BIT_SIZE)
// IS_ALLOCATED : check block is allocated
#define IS_ALLOCATED(HEADER) ((HEADER)&FLAG_ALLOC)
// IS_PREV_USED : check previous block is used
#define IS_PREV_USED(HEADER) ((HEADER)&FLAG_PREV_USED)
// IS_THREADED : check block is threaded
#define IS_THREADED(HEADER) ((HEADER)&FLAG_THREADED)
// because of header, size can be sizeof(size_t) * 8 - 3, for safe, set -5
#define MAX_MALLOC_SIZE ((1UL << (sizeof(size_t) * 8 - 5)) - 1)

typedef struct s_metadata {
  size_t header;                 // will be store size + flags(1byte)
  struct s_metadata *next_block; // next free block
} t_metadata;

// metadata structure for free tiny block
typedef struct s_tiny_metadata {
  size_t header;                      // will be store size + flags(1byte)
  struct s_tiny_metadata *next_block; // next free block
} t_tiny_metadata;

// metadata structure for free small block. at last word, it has sizeof block
typedef struct s_common_metadata {
  size_t header;                        // will be store size + flags(1byte)
  struct s_common_metadata *next_block; // next free block
  struct s_common_metadata *prev_block; // prev free block
} t_common_metadata;

// enum for malloc block type
enum e_malloc_pool_type { TINY, SMALL, LARGE };

// GET SIZE that is Multiple of Page size >= size
#define GET_MULTIPLE_SIZE(size, multiples)                                     \
  ((size) % (multiples) ? (((multiples) - (size) % (multiples)) + (size))      \
                        : (size))
// default align size, block must be aligned in multiple of ALIGN_SIZE
#define MALLOC_ALIGN_SIZE (sizeof(size_t) * 2)
// tiny align size = ALIGN_SIZE
#define MALLOC_TINY_ALIGN_SIZE MALLOC_ALIGN_SIZE
// define metadata size
#define MALLOC_TINY_METADATA_SIZE (sizeof(t_tiny_metadata))
#define MALLOC_SMALL_METADATA_SIZE (sizeof(t_common_metadata) + sizeof(size_t))
// SMALL_META_DATA SIZE == sizeof(size_t) * 4 (header + footer)
#define MALLOC_SMALL_ALIGN_SIZE (MALLOC_TINY_ALIGN_SIZE << 5)
// LARGE ALIGN SIZE = pagesize
#define MALLOC_LARGE_ALIGN_SIZE (getpagesize())
// N for TINY_SIZE_MAX
#define MALLOC_TINY_SIZE_MAX                                                   \
  (MALLOC_SMALL_ALIGN_SIZE - MALLOC_TINY_METADATA_SIZE)
// M for SMALL_SIZE_MAX
#define MALLOC_SMALL_SIZE_MAX                                                  \
  ((4 * getpagesize()) - (MALLOC_SMALL_METADATA_SIZE))
// POOL_SIZE = MAX_SIZE * 128 (can contain 128 blocks)
#define MALLOC_TINY_POOL_SIZE                                                  \
  (GET_MULTIPLE_SIZE(MALLOC_TINY_SIZE_MAX * 128, getpagesize()))
// POOL_SIZE = MAX_SIZE * 128 (can contain 128 blocks)
#define MALLOC_SMALL_POOL_SIZE                                                 \
  (GET_MULTIPLE_SIZE(MALLOC_TINY_SIZE_MAX * 128, getpagesize()))
// GET SIZE TYPE, TINY, SMALL, LARGE
#define GET_SIZE_TYPE(size)                                                    \
  ((size) <= MALLOC_TINY_SIZE_MAX                                              \
       ? (TINY)                                                                \
       : ((size) <= MALLOC_SMALL_SIZE_MAX ? (SMALL) : (LARGE)))

#define IS_TINY_BLOCK(header) ((GET_BLOCK_SIZE(header)) <= MALLOC_TINY_SIZE_MAX)
#define IS_SMALL_BLOCK(header)                                                 \
  ((GET_BLOCK_SIZE(header)) <= MALLOC_SMALL_SIZE_MAX)
#define IS_LARGE_BLOCK(header)                                                 \
  ((GET_BLOCK_SIZE(header)) > MALLOC_SMALL_SIZE_MAX)

typedef struct s_page_header {
  size_t size;
  void *first_block;
  struct s_page_header *next;
} t_page_header;

// FOR ARENA
typedef struct s_arena {
  size_t arena_idx;                   // NOT USED IN MANADATORY
  t_tiny_metadata *tiny_free_pool;    // tiny pool free head block
  t_common_metadata *small_free_pool; // small pool free head block
  t_page_header *tiny_page;           // tiny page header (allocated page list)
  t_page_header *small_page;          // small page header (allocated page list)
  t_page_header *large_page;          // large page header (allocated page list)
} t_arena;

// ARENA
t_arena g_arena;
// arena
int __init_arena(t_arena *const arena);
// block
void *__split_small_block(t_arena *const arena, void *const block,
                          const size_t size);
void *__split_tiny_block(t_arena *const arena, void *const block,
                         const size_t size);
void *__set_tiny_block(void *const block, const size_t header,
                       void *next_block);
void *__set_small_block(void *const block, const size_t header,
                        void *next_block, void *prev_block);
void *__set_large_block(void *const block, const size_t header);
void *__allocate_large_block(t_arena *const arena, const size_t size);
void *__allocate_small_block(t_arena *const arena, const size_t size);
void *__allocate_tiny_block(t_arena *const arena, const size_t size);
void *__pop_free_tiny_block(t_arena *const arena, const size_t size);
void *__pop_free_small_block(t_arena *const arena, const size_t size);
size_t __get_request_block_size(size_t size);
// pool
void *__init_small_pool(t_arena *const arena);
void *__init_tiny_pool(t_arena *const arena);
void *__append_small_pool(t_arena *const arena);
void *__append_tiny_pool(t_arena *const arena);
// free
void __free_tiny_block(t_arena *arena, t_tiny_metadata *block);
void __free_small_block(t_arena *arena, t_common_metadata *block);
void __free_large_block(t_arena *arena, t_common_metadata *block);

#endif