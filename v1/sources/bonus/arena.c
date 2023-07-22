#include "malloc_util_bonus.h"

extern _Atomic int __thread_count;

/**
 * @fn __get_arena
 * @brief get arena. it will assigned by thread count. (round robin)
 */
t_arena *__get_arena(t_tcache *const cache)
{
	if (cache->arena_idx == -1) {
		++__thread_count;
		cache->arena_idx = (__thread_count % MAX_ARENA_NUMBER);
	}
	return (&g_arenas[cache->arena_idx]);
}

/**
 * @fn __append_arena_large_page
 * @brief append arena's large page.
 */
void *__append_arena_large_page(t_arena *arena, void *alloc, size_t size)
{
	t_page_header *alloc_pool = arena->large_page;

	if (alloc_pool) {
		while (alloc_pool->next) {
			alloc_pool = alloc_pool->next;
		}
		alloc_pool->next = alloc;
	} else {
		arena->large_page = alloc;
	}
	alloc_pool = alloc;
	alloc_pool->next = NULL;
	alloc_pool->first_block = alloc + sizeof(t_page_header);
	alloc_pool->size = size;
	return (alloc_pool);
}

t_arena *__find_tiny_block_original_arena(t_arena *thread_arena,
										  t_metadata *block)
{
	t_page_header *page = NULL;
	int idx = 0;

	// first, find in thread_arena
	page = thread_arena->tiny_page;
	while (page) {
		if ((void *)page <= (void *)block &&
			(__uint8_t *)block <= ((__uint8_t *)page + page->size))
			return (thread_arena);
		page = page->next;
	}
	// find in other arenas
	while (idx < MAX_ARENA_NUMBER) {
		if (thread_arena == &g_arenas[idx]) {
			++idx;
			continue;
		}
		page = g_arenas[idx].tiny_page;
		while (page) {
			if ((void *)page <= (void *)block &&
				(__uint8_t *)block <= ((__uint8_t *)page + page->size))
				return (&g_arenas[idx]);
			page = page->next;
		}
		++idx;
	}
	return (NULL);
}

t_arena *__find_small_block_original_arena(t_arena *thread_arena,
										   t_metadata *block)
{
	t_page_header *page = NULL;
	int idx = 0;

	// first, find in thread_arena
	page = thread_arena->small_page;
	while (page) {
		if ((void *)page <= (void *)block &&
			(__uint8_t *)block <= ((__uint8_t *)page + page->size))
			return (thread_arena);
		page = page->next;
	}
	// find in other arenas
	while (idx < MAX_ARENA_NUMBER) {
		if (thread_arena == &g_arenas[idx]) {
			++idx;
			continue;
		}
		page = g_arenas[idx].small_page;
		while (page) {
			if ((void *)page <= (void *)block &&
				(__uint8_t *)block <= ((__uint8_t *)page + page->size))
				return (&g_arenas[idx]);
			page = page->next;
		}
		++idx;
	}
	return (NULL);
}

t_arena *__find_large_block_original_arena(t_arena *thread_arena,
										   t_metadata *block)
{
	t_page_header *page;
	int idx;

	// first, find in thread_arena
	page = thread_arena->large_page;
	while (page) {
		if ((void *)page <= (void *)block &&
			(__uint8_t *)block <= ((__uint8_t *)page + page->size))
			return (thread_arena);
		page = page->next;
	}
	// find in other arenas
	idx = 0;
	while (idx < MAX_ARENA_NUMBER) {
		if (thread_arena == &g_arenas[idx]) {
			++idx;
			continue;
		}
		page = g_arenas[idx].large_page;
		while (page) {
			if ((void *)page <= (void *)block &&
				(__uint8_t *)block <= ((__uint8_t *)page + page->size))
				return (&g_arenas[idx]);
			page = page->next;
		}
		++idx;
	}
	return (NULL);
}