#include <stddef.h>

/**
 * @fn ft_memcpy
 * @brief	:	copy length byte string to dest (if src and dst is
 * overlapped, behaviour is undefined)
 */
void *ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t idx_byte;
	unsigned char *dest;
	unsigned char *source;

	if (dst == src)
		return (dst);
	idx_byte = 0;
	dest = (unsigned char *)dst;
	source = (unsigned char *)src;
	while (idx_byte < n) {
		dest[idx_byte] = source[idx_byte];
		++idx_byte;
	}
	return (dst);
}
