/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:12:56 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:24:00 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Function		:	ft_isdigit
 * Description	:	tests for param 'c' is digit (ascii)
 * Param		
 * 			c	:	number to test
 * Return Value	:	1 (TRUE) : c is digit
 * 					0 (FALSE) : c is not digit
 */
int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}
