#include "malloc_util_bonus.h"

/**
 * @fn ft_print_page_info
 * @brief page: %p - %p size : %u bytes
 */
void ft_print_page_info(void *page_start, void *page_end, size_t size) {
  ft_putstr(__FUNCTION__);
  ft_putchar('\n');
  ft_putstr("page: ");
  ft_putaddr(page_start);
  ft_putstr(" - ");
  ft_putaddr(page_end);
  ft_putstr(" size : ");
  ft_putnbr(size);
  ft_putstr("bytes\n");
}

/**
 * @fn ft_print_block_info
 * @brief block: %p - %p : %u bytes (allocated)\n
 */
void ft_print_block_info(void *block_start, void *block_end, size_t size) {
  ft_putstr(__FUNCTION__);
  ft_putchar('\n');
  ft_putstr("block: ");
  ft_putaddr(block_start);
  ft_putstr(" - ");
  ft_putaddr(block_end);
  ft_putstr(" size : ");
  ft_putnbr(size);
  ft_putstr("bytes (allocated)\n");
}
