#include "malloc.h"
#include "avl_tree.h"
#include "malloc_pool.h"

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

	pthread_mutex_lock(&g_mutex);
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
	pthread_mutex_unlock(&g_mutex);
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
	// // init
	g_manager.head = NULL;
	for (int i = 0; i < MAX_ORDER_TINY; ++i) {
		g_manager.tiny_free_list[i] = NULL;
	}
	for (int i = 0; i < MAX_ORDER_SMALL; ++i) {
		g_manager.small_free_list[i] = NULL;
	}
	// alloc
	t_pool *tiny_pool = create_tiny_pool(&g_manager);
	t_pool *small_pool = create_small_pool(&g_manager);

	append_pool(tiny_pool, &g_manager);
	append_pool(small_pool, &g_manager);
	// // pthrea mutex
	pthread_mutex_init(&g_mutex, NULL);
}

/**
 * @brief destruct all allocated memory that used in malloc
 *
 */
void memory_destruct(void)
{
	// destory all memory pools
	desturct_pool(g_manager.head);
	// destory all ptr spaces
	desturct_space(g_manager.pmalloc_space);
	pthread_mutex_destroy(&g_mutex);
}

static void free_pool(t_pool *pool)
{
	munmap(pool->addr, pool->size);
	pfree(pool, g_manager.pmalloc_space);
}

/**
 * @brief destruct pool
 *
 * @param head head of pool list
 */
void desturct_pool(t_pool *const head)
{
	map_tree(head, free_pool);
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
