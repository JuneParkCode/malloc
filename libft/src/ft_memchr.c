/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:49:10 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:27:49 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

/* Function		:	ft_memchr
 * Description	:	locate byte in byte string
 * Param		
 * 			s	: 	address of bytes
 * 			c	:	number to find
 * 			n	:	size of bytes
 * Return Value	:	pointer to the byte located
 * 					NULL : no such byte exists whithin n bytes
 */
void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				idx_byte;
	const unsigned char	*str;

	idx_byte = 0;
	str = (const unsigned char *) s;
	while (idx_byte < n && str[idx_byte] != (unsigned char) c)
		++idx_byte;
	if (idx_byte == n)
		return (NULL);
	else
		return ((void *)(s + idx_byte));
}
