/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strpbrk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 18:29:55 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:46:54 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_strpbrk
 * Description	:	The strpbrk() function locates in the null-terminated string 
 * 					s the first occurrence of any character in the string 
 * 					charset and returns a pointer to this character.  If no 
 * 					characters from charset occur anywhere in s strpbrk() returns NULL.
 * Param		
 * 			str	:	target to find
 * 			set	:	string sets to find
 * Return Value	:	pointer of found character in string
 */
char	*ft_strpbrk(const char *str, const char *set)
{
	char	*res;

	while (*str)
	{
		res = ft_strchr(set, *str);
		if (res)
			return ((char *)str);
		++str;
	}
	return (NULL);
}
