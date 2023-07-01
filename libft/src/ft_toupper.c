/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:04:32 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/17 18:26:38 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	islower(int c)
{
	return ('a' <= c && c <= 'z');
}

/* Function		:	ft_toupper
 * Description	:	convert lower case to upper case letter
 * Param		
 * 			c	:	letter to convert
 * Return Value	:	converted letter c
 */
int	ft_toupper(int c)
{
	if (islower(c))
		return (c + 'A' - 'a');
	return (c);
}
