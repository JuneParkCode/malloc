/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 12:17:46 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/15 14:59:03 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_get_next_line.h"
#include "libft.h"

char	*ft_get_next_line(int fd)
{
	static char	*store[OPEN_MAX];
	char		*str;
	char		*ret;
	char		*line;

	if (fd < 0 || read(fd, NULL, 0) == -1 || BUFFER_SIZE < 1)
		return (NULL);
	if (store[fd] == NULL)
		store[fd] = ft_strdup("");
	if (ft_strchr(store[fd], '\n'))
		str = ft_strdup(store[fd]);
	else
		str = read_file(fd, store);
	line = get_line(str);
	if (ft_strchr(store[fd], '\n'))
		ret = ft_strjoin("", line);
	else
		ret = ft_strjoin(store[fd], line);
	store[fd] = free_and_make_null(store[fd]);
	store[fd] = get_residue(str);
	line = free_and_make_null(line);
	str = free_and_make_null(str);
	if (ret[0] == 0)
		ret = free_and_make_null(ret);
	return (ret);
}

char	*get_line(char *str)
{
	char	*ret;
	char	*lr_pos;
	size_t	size_ret;

	lr_pos = ft_strchr(str, '\n');
	if (lr_pos == NULL)
		return (ft_strdup(str));
	size_ret = lr_pos - str + 2;
	ret = malloc(sizeof(char) * size_ret);
	ft_strlcpy(ret, str, size_ret);
	return (ret);
}

char	*read_file(int fd, char *store[])
{
	char	*ret;
	char	*tmp;
	char	buf[BUFFER_SIZE + 1];
	ssize_t	rd_size;

	ret = ft_strdup("");
	buf[0] = 0;
	while (ft_strchr(buf, '\n') == NULL)
	{
		rd_size = read(fd, buf, BUFFER_SIZE);
		if (rd_size == -1)
		{
			store[fd] = free_and_make_null(store[fd]);
			return (free_and_make_null(ret));
		}
		if (rd_size == 0)
			break ;
		buf[rd_size] = 0;
		tmp = ret;
		ret = ft_strjoin(ret, buf);
		tmp = free_and_make_null(tmp);
	}
	return (ret);
}

char	*get_residue(char *str)
{
	char	*ret;
	char	*residue;
	size_t	size_ret;

	residue = ft_strchr(str, '\n') + 1;
	if (residue - 1 == NULL)
		return (NULL);
	size_ret = ft_strlen(residue) + 1;
	ret = malloc(sizeof(char) * size_ret);
	ft_strlcpy(ret, residue, size_ret);
	return (ret);
}

void	*free_and_make_null(void *addr)
{
	if (addr)
		free(addr);
	return (NULL);
}
