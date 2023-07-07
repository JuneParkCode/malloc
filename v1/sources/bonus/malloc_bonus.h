#ifndef MALLOC_BONUS_H
#define MALLOC_BONUS_H

// implement this
#include <stddef.h>
void *malloc(size_t size);
size_t malloc_flush_thread_cache(void);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
void show_alloc_mem(void);     // for debug and logging
void show_memory_status(void); // for debug and logging

#endif