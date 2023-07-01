/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 01:23:01 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:23:26 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

/* Function		:	ft_bzero
 * Description	:	The bzero() function writes n zeroed bytes to the string s.
 * 					If n is zero, bzero() does nothing.
 * Param		
 * 			s	:	address to fill
 * 			n	:	length of byte to fill
 * Return Value	:	NONE
 */
void	ft_bzero(void *s, size_t n)
{
	char	*str;
	size_t	idx_str;

	str = s;
	idx_str = 0;
	while (idx_str < n)
	{
		str[idx_str] = 0;
		++idx_str;
	}
}
