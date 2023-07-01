/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:41:42 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:37:08 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/* Function		:	ft_strtrim
 * Description	:	Allocates (with malloc(3)) and returns a copy of ’s1’ with 
 * 					the characters specified in ’set’ removed from the beginnin
 * 					g and the end of the string.
 * Param		
 * 			s1	: 	address of bytes
 * 			set	:	number to find
 * Return Value	:	address of copy of s1 with the chars specified in set
 * 					NULL : Memory allocation error
 */
char	*ft_strtrim(char const *s1, char const *set)
{
	char			*res;
	size_t			start;
	size_t			end;
	size_t			size;

	start = 0;
	while (ft_isinset(s1[start], set) && s1[start])
		++start;
	end = ft_strlen(s1) - 1 + (ft_strlen(s1) == 0);
	while (ft_isinset(s1[end], set) && start < end)
		--end;
	size = end - start + 2;
	if (end == start)
		size = 1;
	res = malloc(sizeof(char) * (size));
	if (res == NULL)
		return (NULL);
	ft_memset(res, 0, size);
	ft_strlcpy(res, s1 + start, size);
	return (res);
}
