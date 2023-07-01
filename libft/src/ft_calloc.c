/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 14:50:09 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/16 15:58:32 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/* Function		:	ft_calloc
 * Description	:	 contigously allocates enough space for count objects 
 * 					that are size bytes of memory each and returns a poin
 * 					ter to the allocated memory.
 * 					 The allocated memory is filled with bytes of value 0
 * Param		
 * 		count	:	number of memory to be allocated	
 * 		size	:	bytes of memory
 * Return Value	:	address of allocated memory
 * 					NULL : allocation does not work normally
 */
void	*ft_calloc(size_t count, size_t size)
{
	void	*res;

	if (!count || !size)
	{
		count = 1;
		size = 1;
	}
	res = malloc(count * size);
	if (res == NULL)
		return (NULL);
	ft_memset(res, 0, count * size);
	return (res);
}
