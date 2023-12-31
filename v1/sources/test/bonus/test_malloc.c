#include "malloc_bonus.h"
#include "test_bonus.h"
#include <stdio.h>

void *test_tiny_size(void *param)
{
	t_arg *arg = param;
	int count = arg->count;
	int *ptr[count];

	for (int i = 0; i < count; ++i) {
		ptr[i] = malloc(16);
		*ptr[i] = i;
	}
	for (int i = 0; i < count; ++i) {
		if (i != *ptr[i])
			printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
		free(ptr[i]);
	}
	// printf("flushed %ld\n", malloc_flush_thread_cache());
	malloc_flush_thread_cache();
	return (malloc(42));
}

void *test_small_size(void *param)
{
	t_arg *arg = param;
	int count = arg->count;
	int *ptr[count];

	for (int i = 0; i < count; ++i) {
		ptr[i] = malloc(512 * ((i % 16) + 1));
		*ptr[i] = i;
	}
	for (int i = 0; i < count; ++i) {
		if (i != *ptr[i])
			printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
		free(ptr[i]);
	}
	// printf("flushed %ld\n", malloc_flush_thread_cache());
	malloc_flush_thread_cache();
	return (malloc(4200));
}

void *test_large_size(void *param)
{
	t_arg *arg = param;
	int count = arg->count;
	int *ptr[count];

	for (int i = 0; i < count; ++i) {
		ptr[i] = malloc(42000);
		*ptr[i] = i;
	}
	for (int i = 0; i < count; ++i) {
		if (i != *ptr[i])
			printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
		free(ptr[i]);
	}
	// printf("flushed %ld\n", malloc_flush_thread_cache());
	malloc_flush_thread_cache();
	return (malloc(42000));
}
