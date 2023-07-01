/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:45:51 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/15 18:19:55 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_strlcat
 * Description	:	strlcat() appends string src to the end of dst.
 * 					It will append at most dstsize - strlen(dst) - 1
 * 					characters.  It will then NUL-terminate, unless
 * 					dstsize is 0 or the original dst string was
 * 					longer than dstsize (in practice this should not
 * 					happen as it means that either dstsize is incorrect
 * 					or that dst is not a proper string).
 * 					If the src and dst strings overlap, the behavior
 * 					is undefined.
 * Param		
 * 			s1	:	The prefix string.
 * 			s2	:	The suffix string.
 * Return Value	:	address of joined string
 * 					NULL : Memory allocation error
 */
size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	const size_t	len_dest = ft_strlen(dest);
	const size_t	len_src = ft_strlen(src);
	size_t			i_dest;
	size_t			i_src;

	i_dest = len_dest;
	i_src = 0;
	while (i_src < len_src && i_dest + 1 < size)
		dest[i_dest++] = src[i_src++];
	dest[i_dest] = 0;
	if (len_dest > size)
		return (size + len_src);
	return (len_dest + len_src);
}
