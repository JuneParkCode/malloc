/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 19:43:23 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/11 16:04:26 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

static int	get_size(unsigned long long n)
{
	int				size;

	size = (n < 0);
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 16;
		++size;
	}
	return (size);
}

size_t	ft_puthex_fd(unsigned long long n, int flag, int fd)
{
	const int		len = get_size(n);
	char			str[20];
	char			*hex;
	int				idx;

	if (flag)
		hex = "0123456789ABCDEF";
	else
		hex = "0123456789abcdef";
	idx = len;
	while (idx)
	{
		str[--idx] = hex[n % 16];
		n /= 16;
	}
	write(fd, str, len);
	return (len);
}
