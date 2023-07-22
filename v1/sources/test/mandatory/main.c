#include "malloc.h"
#include "test.h"
#include <pthread.h>
#include <stdio.h>

int main()
{
	test_tiny_size(15);
	test_small_size(15);
	test_large_size(15);
	const int size = 16;
	char *ptr[size];

	int multiple = 1;
	for (int i = 0; i < size; ++i) {
		ptr[i] = malloc(multiple);
		multiple *= 2;
	}
	printf("After malloc\n");
	show_alloc_mem();
	for (int i = size - 1; i >= 0; --i) {
		ptr[i] = realloc(ptr[i], multiple);
		multiple /= 2;
	}
	printf("After realloc\n");
	show_alloc_mem();
	for (int i = 0; i < size; ++i) {
		free(ptr[i]);
	}
	printf("After free.\n");
	show_alloc_mem();
}