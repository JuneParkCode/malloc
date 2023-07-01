/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 20:30:18 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/14 18:11:24 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_strnstr
 * Description	:	locate first string needle in string within len bytes
 * Param		
 * 	haystack	:	string where to find string
 * 	needle		:	string to find
 * 	len			:	length of bytes of string to search
 * Return Value	:	address of located needle string in haystack
 */
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t			idx_haystack;
	size_t			idx_needle;

	idx_haystack = 0;
	if (ft_strlen(needle) == 0)
		return ((char *)haystack);
	while (haystack[idx_haystack] && idx_haystack < len)
	{
		idx_needle = 0;
		while (haystack[idx_haystack + idx_needle] && needle[idx_needle]
			&& ((idx_haystack + idx_needle) < len)
			&& haystack[idx_haystack + idx_needle] == needle[idx_needle])
		{
			++idx_needle;
		}
		if (!needle[idx_needle])
			return ((char *)(haystack + idx_haystack));
		++idx_haystack;
	}
	return (NULL);
}
