#include "malloc_util.h"

/**
 * @fn ft_putaddr
 * @brief put address of pointer to stdout
 */
int ft_putaddr(void const *addr)
{
	if (addr == 0) {
		ft_putchar('0');
		return (1);
	}
	ft_putstr("0x");
	return (ft_puthex((unsigned long long)addr, 0));
}
