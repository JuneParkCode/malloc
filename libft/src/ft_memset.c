/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:49:44 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:28:09 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

/* Function		:	ft_memset
 * Description	:	fill a byte string with a byte value
 * Param		
 * 			b	:	address to fill
 * 			c	:	value of byte
 * 			len	:	length of byte to fill
 * Return Value	:	first argument (b)
 */

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*mem_b;
	unsigned char	fill_value;
	size_t			idx_memb;

	mem_b = (unsigned char *) b;
	fill_value = (unsigned char) c;
	idx_memb = 0;
	while (idx_memb < len)
	{
		mem_b[idx_memb] = fill_value;
		++idx_memb;
	}
	return (b);
}
