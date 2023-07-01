/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:40:19 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/15 20:09:25 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/* Function		:	ft_lstdelone
 * Description	:	Takes as a parameter an element and frees the
 * 					memory of the element’s content using the function
 * 					’del’ given as a parameter and free the element
 * 					The memory of ’next’ must not be freed.
 * Param		
 * 		lst		:	The element to free.
 * 		del		:	The address of the function used to delete the content.
 * Return Value	:	NONE
 */
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst == NULL)
		return ;
	del(lst -> content);
	free(lst);
}
