/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 18:26:49 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/11 15:45:45 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_isspace
 * Description	:	return true if character c is space
 * Param		
 * 			c	:	character to test
 * Return Value	:	true 	: character is space
 * 					false 	: character is not space
 */
int	ft_isspace(const char c)
{
	const char	*spaces = "\t\n\r\v\f ";

	return (ft_isinset(c, spaces));
}
