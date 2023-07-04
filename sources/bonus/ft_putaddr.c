#include "malloc_util_bonus.h"

/**
 * @fn ft_putaddr
 * @brief put address of pointer to stdout
 */
int ft_putaddr(void *addr) {
  ft_putstr(__FUNCTION__);
  ft_putchar('\n');
  if (addr == 0) {
    ft_putchar('0');
    return (1);
  }
  ft_putstr("0x");
  return (ft_puthex((unsigned long long)addr, 0));
}
