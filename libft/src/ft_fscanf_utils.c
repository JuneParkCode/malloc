/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fscanf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 14:32:46 by sungjpar          #+#    #+#             */
/*   Updated: 2022/10/22 14:32:47 by sungjpar         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

#define SUCCESS 0
#define FAILED -1
#define TRUE 1
#define FALSE 0

int	read_char(char *dest, int fd)
{
	int	read_size;

	read_size = read(fd, dest, 1);
	if (read_size < 1)
		return (FAILED);
	return (SUCCESS);
}

int	read_int(int *dest, int fd, char *next_char)
{
	int		read_size;
	char	buf[30];
	char	*buf_ptr;

	buf_ptr = buf;
	read_size = read(fd, buf_ptr, 1);
	while (read_size >= 1)
	{
		if (ft_isdigit(*buf_ptr) == FALSE)
			break ;
		++buf_ptr;
		read_size = read(fd, buf_ptr, 1);
	}
	if (read_size == FAILED || buf == buf_ptr)
		return (FAILED);
	*next_char = *buf_ptr;
	*(buf_ptr) = '\0';
	*dest = ft_atoi(buf);
	return (SUCCESS);
}

int	read_float(float *dest, int fd, char *next_char)
{
	int		read_size;
	char	buf[30];
	char	*buf_ptr;
	int		flag;

	buf_ptr = buf;
	flag = 0;
	read_size = read(fd, buf_ptr, 1);
	while (read_size >= 1)
	{
		if (ft_isdigit(*buf_ptr) == FALSE && !flag && *buf_ptr != '.')
			break ;
		if (!flag && *buf_ptr == '.')
			flag = 1;
		++buf_ptr;
		read_size = read(fd, buf_ptr, 1);
	}
	if (read_size == FAILED || buf == buf_ptr)
		return (FAILED);
	*next_char = *buf_ptr;
	*(buf_ptr) = '\0';
	*dest = ft_atof(buf);
	return (SUCCESS);
}
