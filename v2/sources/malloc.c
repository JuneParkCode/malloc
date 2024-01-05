#include "malloc.h"
#include "malloc_debug.h"
#include "malloc_pool.h"
#include "malloc_util.h"

static void memory_construct(void) __CONSTRUCTOR__;
static void memory_destruct(void) __DESTRUCTOR__;
static void desturct_pool(t_pool *const head);
static void desturct_space(t_pmalloc_space *const head);

t_mmanager g_manager __INTERNAL__;
pthread_mutex_t g_mutex __INTERNAL__;

/**
 * @brief allocate memory from heap
 *
 * @param size size to allocate
 * @return void* allocated memory address. if failed, it returns NULL
 */
void *malloc(unsigned long size)
{
	const POOL_TYPE type = GET_SIZE_TYPE(size);
	void *ret = NULL;
	// allocate memory by size
#ifdef DEBUG
	ft_putstr("malloc called\n");
#endif
	switch (type) {
	case TINY:
		ret = allocate_tiny_block(size, &g_manager);
		break;
	case SMALL:
		ret = allocate_small_block(size, &g_manager);
		break;
	case LARGE:
		ret = allocate_large_block(size, &g_manager);
		break;
	}
#ifdef DEBUG
	ft_putstr("CALL MALLOC : ALLOCATION STATUS\n");
	print_allocations(&g_manager);
#endif
	return ret;
}

/**
 * @brief construct memory pool (warm cache)
 *
 */
void memory_construct(void)
{
	// allocate space
	g_manager.pmalloc_space = allocate_pmalloc_space();
	// init
	g_manager.tiny_pool_head = NULL;
	g_manager.small_pool_head = NULL;
	g_manager.large_pool_head = NULL;
	// alloc
	g_manager.tiny_pool_head = create_tiny_pool(&g_manager);
	g_manager.small_pool_head = create_small_pool(&g_manager);
	// pthrea mutex
	pthread_mutex_init(&g_mutex, NULL);
#ifdef DEBUG
	ft_putstr("construct done\n");
	print_allocations(&g_manager);
#endif
}

/**
 * @brief destruct all allocated memory that used in malloc
 *
 */
void memory_destruct(void)
{
	// destory all memory pools
	desturct_pool(g_manager.tiny_pool_head);
	desturct_pool(g_manager.small_pool_head);
	desturct_pool(g_manager.large_pool_head);
	// destory all ptr spaces
	desturct_space(g_manager.pmalloc_space);
}

/**
 * @brief destruct pool
 *
 * @param head head of pool list
 */
void desturct_pool(t_pool *const head)
{
	t_pool *ptr = head;
	t_pool *next = NULL;

	while (ptr) {
		next = ptr->next;
		munmap(ptr->addr, ptr->size);
		pfree(ptr, g_manager.pmalloc_space);
		ptr = next;
	}
}

/**
 * @brief destruct pmalloc space
 *
 * @param head head of pmalloc space list
 */
void desturct_space(t_pmalloc_space *const head)
{
	t_pmalloc_space *ptr = head;
	t_pmalloc_space *next = NULL;

	while (ptr) {
		next = ptr->next;
		munmap(ptr, ptr->size);
		ptr = next;
	}
}
