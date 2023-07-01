/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:50:08 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/15 18:15:08 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_lstiter
 * Description	:	Iterates the list ’lst’ and applies the function ’f’ to the 
 * 					content of each element.
 * Param		
 * 		lst		:	The adress of a pointer to an element.
 * 		f		:	The adress of the function used to iterate on the list.
 * Return Value	:	NONE
 */
void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*cur_node;

	cur_node = lst;
	while (cur_node)
	{
		f(cur_node -> content);
		cur_node = cur_node -> next;
	}
}
