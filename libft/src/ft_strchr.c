/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:10:00 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:32:15 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

/* Function		:	ft_strchr
 * Description	:	locate first character c in string
 * Param		
 * 			c	:	number to compute
 * Return Value	:	address of located first character c in string
 */
char	*ft_strchr(const char *s, int c)
{
	size_t	idx_str;

	idx_str = 0;
	while (s[idx_str] && (char) c != (char) s[idx_str])
		++idx_str;
	if (s[idx_str] || c == 0)
		return ((char *)(s + idx_str));
	else
		return (NULL);
}
