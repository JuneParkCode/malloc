#include "malloc_util.h"

static int get_size(long long n) {
  int size;

  size = (n < 0);
  if (n == 0)
    return (1);
  while (n > 0) {
    n /= 16;
    ++size;
  }
  return (size);
}

/**
 * @fn ft_put_hex
 * @brief put hex to stdout
 */
int ft_puthex(unsigned long long n, int flag) {
  const int len = get_size(n);
  char str[20];
  char *hex;
  int idx;

  if (flag)
    hex = "0123456789ABCDEF";
  else
    hex = "0123456789abcdef";
  idx = len;
  while (idx) {
    str[--idx] = hex[n % 16];
    n /= 16;
  }
  write(1, str, len);
  return (len);
}
