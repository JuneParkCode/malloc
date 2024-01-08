#ifndef MALLOC_POOL_H
#define MALLOC_POOL_H

#include "malloc_definitions.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

#ifndef MAP_ANON
	#define MAP_ANON MAP_ANONYMOUS
#endif

#define PAGE_SIZE (getpagesize())

// bucket order max
#define MAX_ORDER_TINY 6  // 8 ~ 256
#define MAX_ORDER_SMALL 3 // 512 ~ 4096
// 42 subject requirement
#define LEAST_SIZE 128

// size definition
// tiny size definitions

#define MALLOC_TINY_SIZE_MIN (sizeof(size_t))
#define MALLOC_TINY_SIZE_MAX (MALLOC_TINY_SIZE_MIN << (MAX_ORDER_TINY - 1))
#define MALLOC_TINY_POOL_SIZE (MALLOC_TINY_SIZE_MAX * LEAST_SIZE) // 32 kb
#define MALLOC_TINY_METADATA_SIZE                                              \
	(MALLOC_TINY_POOL_SIZE / MALLOC_TINY_SIZE_MIN) // 4 kb ( 1 page )
#define MALLOC_TINY_ALLOC_SIZE                                                 \
	(MALLOC_TINY_POOL_SIZE + MALLOC_TINY_METADATA_SIZE)

// small size definitions

#define MALLOC_SMALL_SIZE_MIN (MALLOC_TINY_SIZE_MAX << 1) // 512 byte
#define MALLOC_SMALL_SIZE_MAX                                                  \
	(MALLOC_SMALL_SIZE_MIN << (MAX_ORDER_SMALL - 1))				// 2048 byte
#define MALLOC_SMALL_POOL_SIZE (MALLOC_SMALL_SIZE_MAX * LEAST_SIZE) // 256 kb
#define MALLOC_SMALL_METADATA_SIZE                                             \
	(MALLOC_SMALL_POOL_SIZE / MALLOC_SMALL_SIZE_MIN) // 0.5 kb ( 0.25 page )
#define MALLOC_SMALL_ALLOC_SIZE                                                \
	(MALLOC_SMALL_POOL_SIZE + MALLOC_SMALL_METADATA_SIZE)

// GET SIZE TYPE, TINY, SMALL, LARGE

#define PMALLOC_POOL_SIZE (PAGE_SIZE * 4)
// minumum page usage..
// [ 4 KB ALIGNED]
// (PMALLOC_POOL_SIZE + MALLOC_TINY_POOL_SIZE + MALLOC_SMALL_POOL_SIZE) /
// PAGE_SIZE

#define GET_SIZE_TYPE(size)                                                    \
	((size) <= MALLOC_TINY_SIZE_MAX                                            \
		 ? (TINY)                                                              \
		 : ((size) <= MALLOC_SMALL_SIZE_MAX ? (SMALL) : (LARGE)))

void *add_addr(void const *addr, ssize_t size) __INTERNAL__;

// block byte metadata
typedef uint8_t t_metadata;

// block node
typedef struct s_block {
	struct s_block *next;
} t_block;

// size definitions.. enum
typedef enum e_pool_type { TINY, SMALL, LARGE } POOL_TYPE;

// pmalloc data space
typedef struct s_pmalloc_space {
	struct s_pmalloc_space *next;
	t_block *free_list;
	size_t size;
} t_pmalloc_space;

// pool sturcture
typedef struct s_pool {
	struct s_pool *parent;	// parent node
	struct s_pool *left;	// left node
	struct s_pool *right;	// right node
	size_t height;			// height of tree
	void *addr;				// pool addr
	t_metadata *metadata;	// information data space address in pool
	POOL_TYPE type;			// type of pool
	size_t size;			// pool size (user space + metadata)
	size_t allocated_size;	// actually allocated size in pool
	size_t user_space_size; // maximum allocation size
} t_pool;

// ptr space
typedef struct s_pool_space {
	t_pool *head;
	t_block *tiny_free_list[MAX_ORDER_TINY];
	t_block *small_free_list[MAX_ORDER_SMALL];
	bool tiny_has_free_pool;
	bool small_has_free_pool;
	t_pmalloc_space *pmalloc_space;
} t_mmanager;

// have to allocate ptr space.
// pmalloc() -> get new pool block allocation
// pfree() -> free ptr space
// pappend() -> append ptr space

t_pmalloc_space *allocate_pmalloc_space(void) __INTERNAL__;
void *pmalloc(t_pmalloc_space *space) __INTERNAL__;
void pfree(void *ptr, t_pmalloc_space *space) __INTERNAL__;

// allocate memory. it will return address of memory

void *allocate_buddy_block(size_t size, t_mmanager *const manager,
						   POOL_TYPE type) __INTERNAL__;
void *allocate_tiny_block(size_t size, t_mmanager *manager) __INTERNAL__;
void *allocate_small_block(size_t size, t_mmanager *manager) __INTERNAL__;
void *allocate_large_block(size_t size, t_mmanager *manager) __INTERNAL__;

// create pool functions return newly allocated pool

t_pool *create_buddy_pool(t_mmanager *const manager,

						  POOL_TYPE type) __INTERNAL__;
t_pool *create_tiny_pool(t_mmanager *manager) __INTERNAL__;
t_pool *create_small_pool(t_mmanager *manager) __INTERNAL__;
t_pool *create_large_pool(size_t size, t_mmanager *manager) __INTERNAL__;

// shrink pool (when pool is empty)

void remove_pool(t_pool *pool, t_mmanager *manager) __INTERNAL__;
void shrink_pool(t_pool *target, t_mmanager *manager) __INTERNAL__;

// append pool functions

void append_pool(t_pool *pool, t_mmanager *manager) __INTERNAL__;

// remove block from pool

void *remove_block_from_pool(t_block *const block, t_pool *const pool,
							 size_t order) __INTERNAL__;

// util
t_block *get_block_addr(t_metadata const *metadata,
						t_pool const *pool) __INTERNAL__;
size_t get_order(size_t size, POOL_TYPE type) __INTERNAL__;
size_t get_block_order(t_metadata info) __INTERNAL__;
size_t get_block_size(t_metadata metadata, POOL_TYPE type) __INTERNAL__;
bool is_allocated(t_metadata info) __INTERNAL__;
size_t get_align_size(size_t size, size_t align) __INTERNAL__;
t_metadata set_metadata(bool is_allocated, size_t size,
						POOL_TYPE type) __INTERNAL__;
t_metadata *get_block_metadata(t_pool const *pool,
							   t_block const *block) __INTERNAL__;
void set_block_metadata(bool is_allocated, size_t size, t_pool *pool,
						t_block *block) __INTERNAL__;
t_pool *find_block_pool(t_block const *block,
						t_mmanager const *manager) __INTERNAL__;
bool is_block_pool(t_block const *block, t_pool const *pool) __INTERNAL__;

// block

void split_block(t_pool *pool, size_t order) __INTERNAL__;
void *get_block_from_pool(t_pool *pool, size_t request_size) __INTERNAL__;
void merge_block(t_block *block, t_pool *pool) __INTERNAL__;
void append_block(t_block *block, t_pool *pool, size_t order) __INTERNAL__;
extern t_mmanager g_manager;
#endif