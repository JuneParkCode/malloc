#include "malloc_util_bonus.h"

/**
 * @fn ft_putstr
 * @brief put string to stdout
 */
int ft_putstr(const char *str) {
  ft_putstr(__FUNCTION__);
  ft_putchar('\n');
  size_t len;

  if (str == NULL) {
    ft_putstr("(null)");
    return (6);
  }
  len = ft_strlen(str);
  write(1, str, len);
  return (len);
}
