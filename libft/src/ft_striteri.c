/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:31:02 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/15 18:18:24 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_striteri
 * Description	:	Applies the function f to each character of the 
 * 					string passed as argument, and passing its index
 * 					as first argument. Each character is passed by
 * 					address to f to be modified if necessary
 * Param		
 * 			s	:	string to iterate
 * 			f	:	The function to apply to each character.
 * Return Value	:	NONE
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	idx_s;

	idx_s = 0;
	while (s[idx_s])
	{
		f(idx_s, &s[idx_s]);
		++idx_s;
	}
}
