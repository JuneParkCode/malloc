#ifndef MALLOC_UTIL_H
#define MALLOC_UTIL_H

#include "malloc_definitions.h"

// util functions
void *ft_memcpy(void *dst, const void *src, size_t n) __INTERNAL__;
int ft_putaddr(void const *addr) __INTERNAL__;
int ft_putchar(const char c) __INTERNAL__;
int ft_puthex(unsigned long long n, int flag) __INTERNAL__;
int ft_putnbr(const long long n) __INTERNAL__;
int ft_putstr(const char *str) __INTERNAL__;
size_t ft_strlen(const char *s) __INTERNAL__;
void ft_print_page_info(void *block_start, void *block_end,
						size_t size) __INTERNAL__;
void ft_print_allocated_block_info(void *block_start, void *block_end,
								   size_t size) __INTERNAL__;
#endif