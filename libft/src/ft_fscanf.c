/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fscanf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 13:00:03 by sungjpar          #+#    #+#             */
/*   Updated: 2022/10/22 13:00:06 by sungjpar         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

#define SUCCESS 0
#define FAILED (-1)
#define TRUE 1
#define FALSE 0

int	read_char(char *dest, int fd);
int	read_int(int *dest, int fd, char *next_char);
int	read_float(float *dest, int fd, char *next_char);

typedef enum e_flag
{
	COMMON_CHAR = 0,
	FORMAT_CHAR = 1
}	t_flag;

static int	read_format(int fd, char format_char, va_list ap, char *next_char)
{
	char	c;

	if (format_char == 'c')
		return (read_char(va_arg(ap, char *), fd));
	if (format_char == 'd' || format_char == 'i')
		return (read_int(va_arg(ap, int *), fd, next_char));
	if (format_char == 'f')
		return (read_float(va_arg(ap, float *), fd, next_char));
	if (format_char == '%')
	{
		read(fd, &c, 1);
		if (c == '%')
			return (SUCCESS);
		return (FAILED);
	}
	return (FAILED);
}

static int	scan_format(const int fd, char *fmt, va_list ap)
{
	int		count_success_read;
	char	read_char;
	char	format_char;

	count_success_read = 0;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			format_char = *(++fmt);
			if (read_format(fd, format_char, ap, &read_char) == -1)
				return (count_success_read);
			if (format_char != 'c' && read_char != *(++fmt))
				return (++count_success_read);
			++count_success_read;
		}
		else if (read(fd, &read_char, 1) < 1 || read_char != *fmt)
			return (count_success_read);
		++fmt;
	}
	return (count_success_read);
}

/**
 * format supports : %d %i %f %c
 * @param fd
 * @param format
 * @param ...
 * @return the number of input items assigned.
 */
int	ft_fscanf(const int fd, char *format, ...)
{
	va_list	ap;
	int		count_success_read;

	if (format == NULL)
		return (FAILED);
	va_start(ap, format);
	count_success_read = scan_format(fd, format, ap);
	va_end(ap);
	return (count_success_read);
}
