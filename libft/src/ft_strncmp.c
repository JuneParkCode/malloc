/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:41:10 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/17 18:26:22 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

/* Function 	: 	ft_strncmp
 * Description	:	compare strings n byte
 * Param		:
 * 			s1	:	address of string 1
 * 			s2	:	address of string 2
 * 			n	:	size to compare
 * return value	:	> 0 s1 is greater than s2
 * 					==0 s1 == s2
 * 					< 0 s1 is less than s2
 */
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	idx_byte;

	idx_byte = 0;
	while ((s1[idx_byte] || s2[idx_byte]) && idx_byte < n)
	{
		if (s1[idx_byte] != s2[idx_byte])
			return ((unsigned char) s1[idx_byte] - \
					(unsigned char) s2[idx_byte]);
		++idx_byte;
	}
	return (0);
}
