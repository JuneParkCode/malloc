/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isinset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 13:25:04 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:53:14 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Function		:	ft_isinset
 * Description	:	return true if character c is in set
 * Param		
 * 			c	:	character to test
 * 			set	:	char set to test
 * Return Value	:	true 	: c is in set
 * 					false	: c is not in set
 */
int	ft_isinset(const char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		++set;
	}
	return (0);
}
