/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:57:56 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/15 18:23:56 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_strlcpy
 * Description	:	size-bounded string copying
 * 					(more safer than strcpy)
 * Param		
 * 			dst	:	destination address to copy
 * 			src	:	source address to copy
 * 		dstsize	:	size of destination size
 * Return Value	:	original value of dst
 */
size_t	ft_strlcpy(char *dst, const char *src, size_t dst_size)
{
	const size_t	len_src = ft_strlen(src);
	size_t			idx_byte;

	idx_byte = 0;
	if (dst_size == 0)
		return (len_src);
	while (idx_byte + 1 < dst_size && src[idx_byte])
	{
		dst[idx_byte] = src[idx_byte];
		++idx_byte;
	}
	dst[idx_byte] = 0;
	return (len_src);
}
