#include "malloc_util_bonus.h"
#include <stddef.h>

/**
 * @fn ft_memcpy
 * @brief	:	copy length byte string to dest (if src and dst is
 * overlapped, behaviour is undefined)
 */
void *ft_memcpy(void *dst, const void *src, size_t n) {
  ft_putstr(__FUNCTION__);
  ft_putchar('\n');
  size_t idx_byte;
  __uint8_t *dest;
  __uint8_t *source;

  if (dst == src)
    return (dst);
  idx_byte = 0;
  dest = (__uint8_t *)dst;
  source = (__uint8_t *)src;
  while (idx_byte < n) {
    dest[idx_byte] = source[idx_byte];
    ++idx_byte;
  }
  return (dst);
}
