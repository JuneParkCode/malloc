#include "malloc_util.h"

static int get_size(const long long n)
{
	unsigned int nb;
	int size;

	size = (n < 0);
	if (n == 0)
		return (1);
	else if (n < 0)
		nb = -n;
	else
		nb = n;
	while (nb > 0) {
		nb /= 10;
		++size;
	}
	return (size);
}

/**
 * @fn ft_putnbr
 * @brief put number to stdout
 */
int ft_putnbr(const long long n)
{
	const unsigned int size = get_size(n);
	unsigned int nb;
	unsigned int idx;
	char str[12];

	idx = size;
	if (n < 0) {
		nb = -n;
		str[0] = '-';
	} else
		nb = n;
	while (nb >= 10) {
		str[--idx] = nb % 10 + '0';
		nb /= 10;
	}
	str[--idx] = nb % 10 + '0';
	write(1, str, size);
	return (size);
}
