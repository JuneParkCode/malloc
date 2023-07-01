/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:26:45 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:24:05 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Function		:	ft_isprint
 * Description	:	tests for param 'c' is printable (ascii)
 * 					The isprint() function tests for any printing character,
 * 					including space(` ').  
 * 					The value of the argument must be representable as an unsigned
 * 					char or the value of EOF.
 * 					In the ASCII character set, this includes the following
 * 					characters (preceded by their numeric values, in octal):
 * Param		
 * 			c	:	number to test
 * Return Value	:	1 (TRUE) : c is printable
 * 					0 (FALSE) : c is not printable
 */
int	ft_isprint(int c)
{
	return (040 <= c && c <= 0176);
}
