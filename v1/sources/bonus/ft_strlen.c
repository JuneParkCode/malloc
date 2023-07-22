#include "malloc_util_bonus.h"

/**
 * @fn ft_strlen
 * @brief get string length
 */
size_t ft_strlen(const char *s)
{
	size_t length;

	length = 0;
	while (s[length])
		length++;
	return (length);
}
