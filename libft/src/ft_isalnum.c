/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 18:13:38 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/10 17:48:41 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_isalnum
 * Description	:	tests for param 'c' that  isalpha or is isdigit is true
 * Param		
 * 			c	:	number to test
 * Return Value	:	1 (TRUE) : c is alphabet or digit
 * 					0 (FALSE) : c is not alphabet and digit
 */
int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
