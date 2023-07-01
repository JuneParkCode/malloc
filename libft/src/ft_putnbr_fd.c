/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:39:43 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/11 15:59:05 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static int get_size(const ssize_t n) {
  size_t nb;
  ssize_t size;

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

/* Function		:	ft_putnbr_fd
 * Description	:	Outputs the number n to the given file descriptor.
 * Param
 * 			n	:	The integer to output.
 * 			fd	:	The file descriptor on which to write.
 * Return Value	:	NONE
 */
size_t ft_putnbr_fd(ssize_t n, int fd) {
  const size_t size = get_size(n);
  size_t nb;
  size_t idx;
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
  write(fd, str, size);
  return (size);
}
