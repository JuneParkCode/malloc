/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:44:54 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/15 19:39:14 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_lstclear
 * Description	:	Deletes and frees the given element and every successor of 
 * 					that element, using the function ’del’ and free(3).
					Finally, the pointer to the list must be set to NULL.
 * Param		
 * 		lst		:	The address of a pointer to an element.
 * 		del		:	The address of the function used to delete the content of 
 * 					the element.
 * Return Value	:	NONE
 */
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*cur_node;
	t_list	*next_node;

	if (lst == NULL || *lst == NULL)
		return ;
	cur_node = *lst;
	while (cur_node)
	{
		next_node = cur_node -> next;
		ft_lstdelone(cur_node, del);
		cur_node = next_node;
	}
	*lst = NULL;
}
