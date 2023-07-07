#include "malloc_util_bonus.h"

/**
 * @fn ft_putchar
 * @brief put char to stdout
 */
int ft_putchar(const char c) {
  write(1, &c, 1);
  return (1);
}
