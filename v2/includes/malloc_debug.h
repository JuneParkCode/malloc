#ifndef MALLOC_DEBUG_H
#define MALLOC_DEBUG_H

#include "malloc_pool.h"

// print all allocations
void print_allocations(t_mmanager const *manager) __INTERNAL__;
// print pool information
void print_pool_info(t_pool const *pool) __INTERNAL__;
// print all pool informations
void print_pools(t_mmanager const *manager) __INTERNAL__;
// print block information
void print_block_info(t_block const *block, t_pool const *pool) __INTERNAL__;
// print block information using metadata
void print_metadata_info(t_metadata const *metadata,
						 t_pool const *pool) __INTERNAL__;
// print allocation information
void print_allocation_info(size_t request_size, t_block const *block,
						   t_pool const *pool) __INTERNAL__;
// print large allocation zone
void print_large_zone(t_pool const *pool) __INTERNAL__;
// print buddy system zone
void print_buddy_zone(t_pool const *pool) __INTERNAL__;
// print pool block info
void print_pool_blocks_infos(t_pool const *pool) __INTERNAL__;
#endif