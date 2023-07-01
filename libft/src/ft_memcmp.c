/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 20:10:17 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:27:54 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

/* Function		:	ft_memcmp
 * Description	:	function compares byte string s1 against byte string s2.
 * 					Both strings are assumed to be n bytes long.
 * Param		
 * 			s1	:	address to fill
 * 			s2	:	value of byte
 * 			n	:	length of byte to compate
 * Return Value	:	>  0 : s1 > s2 
 * 					== 0 : s1 == s2
 * 					<  0 : s1 < s1
 */

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				idx_byte;
	const unsigned char	*str1;
	const unsigned char	*str2;

	idx_byte = 0;
	str1 = (const unsigned char *)s1;
	str2 = (const unsigned char *)s2;
	while (idx_byte < n && str1[idx_byte] == str2[idx_byte])
		++idx_byte;
	if (idx_byte == n)
		return (0);
	else
		return (str1[idx_byte] - str2[idx_byte]);
}
