#ifndef MALLOC_H
#define MALLOC_H

// for type definition
#include <stddef.h>
// visability keyword setting
#define OPEN_API __attribute__((visibility("default")));
#define INTERNAL __attribute__((visibility("hidden")));

// USER APIs
// !! MUST INSERT `OPEN_API` after declaration
void *malloc(size_t size) OPEN_API;
void *realloc(void *ptr, size_t size) OPEN_API;
void free(void *ptr) OPEN_API;
void show_alloc_mem(void) OPEN_API;

// Internal functions
// !! MUST INSERT `INTERNAL` after declaration

#endif