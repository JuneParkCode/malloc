#ifndef MALLOC_H
#define MALLOC_H

#include "malloc_definitions.h"

// USER APIs
// !! MUST INSERT `OPEN_API` after declaration
void *malloc(size_t size) __OPEN_API__;
void *realloc(void *ptr, size_t size) __OPEN_API__;
void free(void *ptr) __OPEN_API__;
void show_alloc_mem(void) __OPEN_API__;

#endif