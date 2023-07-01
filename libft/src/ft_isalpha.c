/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:10:53 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:23:49 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Function		:	ft_isalpha
 * Description	:	tests for param 'c' is alphabet
 * Param		
 * 			c	:	number to test
 * Return Value	:	1 (TRUE) : c is alphabet
 * 					0 (FALSE) : c is not alphabet
 */
int	ft_isalpha(int c)
{
	return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
}
