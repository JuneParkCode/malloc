/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:29:23 by sungjpar          #+#    #+#             */
/*   Updated: 2022/06/29 21:09:22 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_lstbadd_back
 * Description	:	Adds the element ’new_lst’ at the end of the list.
 * Param		
 * 		lst		:	The address of a pointer to the first link of a list.
 * 		new_lst		:	The address of a pointer to the element to be added to 
 * 					the list.
 * Return Value	:	NONE
 */

void	ft_lstadd_back(t_list **lst, t_list *new_lst)
{
	t_list	*last_node;

	if (lst == NULL)
		return ;
	last_node = ft_lstlast(*lst);
	if (last_node == NULL)
		*lst = new_lst;
	else
		last_node -> next = new_lst;
}
