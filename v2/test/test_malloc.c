#include "malloc.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void test_tiny_size(int count)
{
	printf("====%s START====\n", __FUNCTION__);
	int *ptr[count];
	srand(time(NULL));

	for (int i = 0; i < count; ++i) {
		ptr[i] = malloc(rand() % 512 + 1);
		*ptr[i] = i;
	}
	write(1, "MALLOC TINY DONE\n", 18);
	show_alloc_mem();
	for (int i = 0; i < count; ++i) {
		if (i != *ptr[i])
			printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
		free(ptr[i]);
	}
	write(1, "FREE TINY DONE\n", 16);
	show_alloc_mem();
	printf("====%s DONE====\\n", __FUNCTION__);
}

void test_small_size(int count)
{
	printf("====%s START====\n", __FUNCTION__);
	int *ptr[count];

	for (int i = 0; i < count; ++i) {
		ptr[i] = malloc(rand() % 65536 + 1);
		*ptr[i] = i;
	}
	write(1, "MALLOC SMALL DONE\n", 19);
	show_alloc_mem();
	for (int i = 0; i < count; ++i) {
		if (i != *ptr[i])
			printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
		free(ptr[i]);
	}
	write(1, "FREE SMALL DONE\n", 17);
	show_alloc_mem();
	printf("====%s DONE====\n", __FUNCTION__);
}

void test_large_size(int count)
{
	printf("====%s START====\n", __FUNCTION__);
	int *ptr[count];

	for (int i = 0; i < count; ++i) {
		ptr[i] = malloc(rand() % (1 << rand() % 30) + 1);
		*ptr[i] = i;
	}
	write(1, "MALLOC LARGE DONE\n", 19);
	show_alloc_mem();
	for (int i = 0; i < count; ++i) {
		if (i != *ptr[i])
			printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
		free(ptr[i]);
	}
	write(1, "FREE LARGE DONE\n", 17);
	show_alloc_mem();
	printf("====%s DONE====\n", __FUNCTION__);
}

void test_fail_case()
{
	// int *ptr[count];

	// for (int i = 0; i < count; ++i) {
	//   ptr[i] = malloc(sizeof(int) * ((i % 100)));
	//   *ptr[i] = i;
	// }
	// show_alloc_mem();
	// for (int i = 0; i < count; ++i) {
	//   if (i != *ptr[i])
	//     printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
	//   free(ptr[i]);
	// }
	// show_alloc_mem();
}