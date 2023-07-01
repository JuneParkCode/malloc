/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:33:33 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/14 18:11:30 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_strrchr
 * Description	:	locate last character c in string
 * Param		
 * 			c	:	number to compute
 * Return Value	:	address of located last character c in string
 */
char	*ft_strrchr(const char *s, int c)
{
	size_t	idx_str;

	idx_str = ft_strlen(s);
	while (idx_str > 0)
	{
		if (c == s[idx_str])
			return ((char *)(s + idx_str));
		--idx_str;
	}
	if (c == s[0])
		return ((char *)s);
	return (NULL);
}
