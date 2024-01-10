#include "malloc.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void test_tiny_size(int count)
{
	show_alloc_mem();
	printf("====%s START====\n", __FUNCTION__);
	int *ptr[count];
	srand(time(NULL));

	write(1, "MALLOC TINY START\n", 18);
	for (int i = 0; i < count; ++i) {
		ptr[i] = malloc(rand() % 256 + 1);
		*ptr[i] = i;
	}
	write(1, "MALLOC TINY DONE\n", 18);
	show_alloc_mem();
	write(1, "FREE TINY START\n", 16);
	for (int i = 0; i < count; ++i) {
		if (i != *ptr[i])
			printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
		free(ptr[i]);
	}
	write(1, "FREE TINY DONE\n", 16);
	show_alloc_mem();
	printf("====%s DONE====\n", __FUNCTION__);
}

void test_small_size(int count)
{
	printf("====%s START====\n", __FUNCTION__);
	int *ptr[count];
	srand(time(NULL));

	write(1, "MALLOC SMALL START\n", 19);
	for (int i = 0; i < count; ++i) {
		ptr[i] = malloc(rand() % 4096 + 256);
		*ptr[i] = i;
	}
	write(1, "MALLOC SMALL DONE\n", 19);
	show_alloc_mem();
	write(1, "FREE SMALL START\n", 17);
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
	show_alloc_mem();
	srand(time(NULL));

	int *ptr[count];

	write(1, "MALLOC LARGE START\n", 19);
	for (int i = 0; i < count; ++i) {
		ptr[i] = malloc((1 << ((rand() % 5) + 12)) + rand() % 4096);
		*ptr[i] = i;
	}
	write(1, "MALLOC LARGE DONE\n", 19);
	show_alloc_mem();
	write(1, "FREE LARGE START\n", 17);
	for (int i = 0; i < count; ++i) {
		if (i != *ptr[i])
			printf("ERROR ON %s:%d\n", __FUNCTION__, __LINE__);
		free(ptr[i]);
	}
	write(1, "FREE LARGE DONE\n", 17);
	show_alloc_mem();
	printf("====%s DONE====\n", __FUNCTION__);
}

void test_random(int count)
{
	printf("====%s RANDOM START====\n", __FUNCTION__);
	int *ptr[count * 3];

	show_alloc_mem();
	for (int i = 0; i < count; ++i) {
		ptr[i] = malloc((1 << (rand() % 16)) + rand() % 256);
		free(ptr[i / 2]);
		ptr[i / 2] = NULL;
	}
	show_alloc_mem();

	for (int i = 0; i < count; ++i) {
		if (ptr[i])
			free(ptr[i]);
	}
	show_alloc_mem();
	printf("====%s DONE====\n", __FUNCTION__);
}