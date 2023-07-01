/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:12:58 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/15 20:10:33 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/* Function		:	ft_mapi
 * Description	:	Applies the function ’f’ to each character of
 * 					the string ’s’ , and passing its index as first
 * 					argument to create a new string (with malloc(3))
 * 					resulting from successive applications of ’f’.
 * Param		
 * 			s	:	The string on which to iterate.
 * 			f	:	 The function to apply to each character.
 * Return Value	:	The string created from the successive applications
 * 					of ’f’. Returns NULL if the allocation fails.
 * 					NULL 	: allocation error
 */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	size;
	size_t	idx_str;
	char	*res;

	size = ft_strlen(s);
	res = malloc(sizeof(char) * (size + 1));
	if (res == NULL)
		return (NULL);
	ft_memset(res, 0, size + 1);
	idx_str = 0;
	while (idx_str < size)
	{
		res[idx_str] = f(idx_str, s[idx_str]);
		++idx_str;
	}
	return (res);
}
