/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 18:35:53 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/11 15:46:19 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

/* Function		:	ft_strcspn
 * Description	:	find matched character in set in string and return index
 * Param		
 * 			str	:	string target to find
 * 			set	:	set of characters to find
 * Return Value	:	index of found character in set in string
 */
size_t	ft_strcspn(const char *str, const char *set)
{
	size_t	idx_str;

	idx_str = 0;
	while (str[idx_str])
	{
		if (ft_strchr(set, str[idx_str]))
			return (idx_str);
	}
	return (idx_str);
}
