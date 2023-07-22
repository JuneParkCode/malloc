#ifndef MALLOC_H
#define MALLOC_H

// implement this
#include <stddef.h>
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void show_alloc_mem(void); // for debug and logging

#endif