#include "malloc_pool.h"
#include "malloc_util.h"

extern t_mmanager g_manager;
extern pthread_mutex_t g_mutex;

/**
 * @brief initialize pmalloc free list
 */
static void init_pmalloc_free_list(t_pmalloc_space *const space)
{
	t_block *block = space->free_list;
	t_block *const end = (t_block *)add_addr(space, space->size);
	size_t count = 0;

	size_t test = PMALLOC_MAX_BLOCK;
	(void)test;
	while (1) {
		++count;
		block->next = (t_block *)add_addr(block, PMALLOC_BLOCK_SIZE);
		if ((t_block *)add_addr(block->next, PMALLOC_BLOCK_SIZE) >= end) {
			block->next = NULL;
			space->free_count = count;
			return;
		}
		block = block->next;
	}
}

static t_pmalloc_space *get_space()
{
	if (g_manager.cache_space) {
		return g_manager.cache_space;
	}
	if (g_manager.free_space) {
		return g_manager.free_space;
	}
	// find new free space
	t_pmalloc_space *space = g_manager.pmalloc_space;

	while (space && space->free_count == 0) {
		space = space->next;
	}
	if (space == NULL) {
		space = allocate_pmalloc_space();
		space->next = g_manager.pmalloc_space;
		g_manager.pmalloc_space = space;
	}
	return space;
}

/**
 * @brief allocate pmalloc's allocation space. it initializes internal values
 *
 * @return t_pmalloc_space : allocated space when failed it returns NULL
 */
t_pmalloc_space *allocate_pmalloc_space(void)
{
	t_pmalloc_space *const space =
		mmap(0, PMALLOC_POOL_SIZE, PROT_READ | PROT_WRITE,
			 MAP_ANON | MAP_PRIVATE, -1, 0);

	if (space == NULL)
		return NULL;
	// initialize
	space->size = PMALLOC_POOL_SIZE;
	space->free_count = 0;
	space->free_list = (t_block *)add_addr(space, sizeof(t_pmalloc_space));
	space->next = NULL;

	g_manager.free_space = space;
	g_manager.cache_space = space;
	init_pmalloc_free_list(space);
	return space;
}

/**
 * @brief allocate pool block
 * @param space pmalloc space ptr
 * @return allocated block
 */
void *pmalloc()
{
	t_block *free_block = NULL;
	t_pmalloc_space *space = get_space();

	free_block = space->free_list;
	space->free_list = space->free_list->next;
	--space->free_count;
	free_block->next = (t_block *)space; // mark space;
	if (g_manager.free_space == space) {
		g_manager.free_space = NULL;
	}
	if (space->free_count > 0) {
		g_manager.cache_space = space;
	} else if (space->free_count == 0) {
		g_manager.cache_space = g_manager.free_space;
	}
	return add_addr(free_block, sizeof(void *));
}

static void shrink_space(t_pmalloc_space *const space)
{
	if (g_manager.free_space && g_manager.free_space != space) {
		t_pmalloc_space *s = g_manager.pmalloc_space;

		// remove space from list
		if (g_manager.pmalloc_space == space) {
			g_manager.pmalloc_space = g_manager.pmalloc_space->next;
		} else {
			while (s && s->next != space) {
				s = s->next;
			}
			s->next = space->next;
		}
		munmap(space, space->size);
	} else { // free_space == NULL || gg_manager.free_space == space
		g_manager.free_space = space;
		g_manager.cache_space = space;
	}
}

/**
 * @brief deallocated pool block
 * @param ptr pool block ptr
 * @param space pmalloc space
 * @return void
 */
void pfree(void *ptr)
{
	t_block *free_block = add_addr(ptr, -sizeof(void *));
	t_pmalloc_space *space = (t_pmalloc_space *)free_block->next; // space

	free_block->next = space->free_list;
	space->free_list = free_block;
	++space->free_count;
	if (space->free_count == PMALLOC_MAX_BLOCK) {
		shrink_space(space);
	}
	return;
}