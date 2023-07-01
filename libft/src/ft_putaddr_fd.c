/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putaddr_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 14:41:15 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/15 14:46:55 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

size_t	ft_putaddr_fd(void *addr, int fd)
{
	if (addr == 0)
	{
		ft_putchar_fd('0', fd);
		return (1);
	}
	return (ft_puthex_fd((unsigned long long) addr, 0, fd));
}
