/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:15:55 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:23:56 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Function		:	ft_isascii
 * Description	:	tests for param 'c' is ascii (in octet, 00 <= c <= 0177)
 * 					The isascii() function tests if a given character, in the c
 * 					urrent locale, can be represented as a valid 7–bit 
 * 					(US-ASCII character.)
 * Param		
 * 			c	:	number to test
 * Return Value	:	1 (TRUE) : c is ascii code
 * 					0 (FALSE) : c is not ascii code
 */
int	ft_isascii(int c)
{
	return (00 <= c && c <= 0177);
}
