/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 15:09:10 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/15 14:59:22 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int print_format(const char f, va_list ap) {
  if (f == 'c')
    return (ft_putchar_fd(va_arg(ap, int), 1));
  else if (f == 's')
    return (ft_putstr_fd(va_arg(ap, char *), 1));
  else if (f == 'p')
    return (ft_putstr_fd("0x", 1) + ft_putaddr_fd(va_arg(ap, void *), 1));
  else if (f == 'd' || f == 'i')
    return (ft_putnbr_fd(va_arg(ap, int), 1));
  else if (f == 'u')
    return (ft_putnbr_fd((va_arg(ap, unsigned int)), 1));
  else if (f == 'x')
    return (ft_puthex_fd((unsigned int)va_arg(ap, int), 0, 1));
  else if (f == 'X')
    return (ft_puthex_fd((unsigned int)va_arg(ap, int), 1, 1));
  else if (f == '%')
    return (ft_putchar_fd(f, 1));
  else
    return (0);
  return (0);
}

static int do_print(const char *fmt, va_list ap) {
  int idx_fmt;
  int len_str;
  int flag;

  idx_fmt = 0;
  len_str = 0;
  flag = 0;
  while (fmt[idx_fmt]) {
    if (flag) {
      len_str += print_format(fmt[idx_fmt], ap);
      flag = 0;
    } else if (fmt[idx_fmt] == '%')
      flag = 1;
    else {
      ++len_str;
      ft_putchar_fd(fmt[idx_fmt], 1);
    }
    ++idx_fmt;
  }
  return (len_str);
}

/*	Function 	:	ft_printf
 *	Description	:  	The printf utility formats and prints its
 *arguments, after the first, under control of the format. The format is a
 *character string which contains three types of objects: plain characters,
 *which are simply copied to standard output, character escape sequences which
 *are converted and copied to the standard output, and format specifications,
 *each of which causes printing of the next successive argument. Params msg
 *:	msg to print (include format)
 *			...	:
 *	Return Value: length of printed string
 */
int ft_printf(const char *fmt, ...) {
  int len_str;
  va_list ap;

  if (fmt == NULL)
    return (-1);
  va_start(ap, fmt);
  len_str = do_print(fmt, ap);
  va_end(ap);
  return (len_str);
}
