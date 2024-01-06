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

	while (1) {
		block->next = (t_block *)add_addr(block, sizeof(t_pool));
		if ((t_block *)add_addr(block->next, sizeof(t_pool)) >= end) {
			block->next = NULL;
			return;
		}
		block = block->next;
	}
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
	space->next = NULL;
	space->free_list = (t_block *)add_addr(space, sizeof(t_pmalloc_space));
	init_pmalloc_free_list(space);
	return space;
}

/**
 * @brief allocate pool block
 * @param space pmalloc space ptr
 * @return allocated block
 */
void *pmalloc(t_pmalloc_space *const space)
{
	t_block *free_block = NULL;

	// if space is not enough, append new allocation space
	if (space->free_list == NULL) {
		t_pmalloc_space *new_space = allocate_pmalloc_space();

		if (new_space == NULL)
			return NULL;
		new_space->free_list->next = NULL;
		space->free_list = new_space->free_list;
		new_space->free_list = NULL;
	}
	free_block = space->free_list;
	space->free_list = free_block->next;
	return free_block;
}

/**
 * @brief deallocated pool block
 * @param ptr pool block ptr
 * @param space pmalloc space
 * @return void
 */
void pfree(void *ptr, t_pmalloc_space *const space)
{
	t_block *free_block = ptr;

	free_block->next = space->free_list;
	space->free_list = free_block;
}