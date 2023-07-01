/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:15:49 by sungjpar          #+#    #+#             */
/*   Updated: 2022/06/29 21:08:54 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_lstadd_front
 * Description	:	Adds the element ’new’ at the beginning of the list.
 * Param		
 * 			lst	:	The address of a pointer to the first link of list
 * 			new	:	The address of a pointer to the element to be added to list
 * Return Value	:	NONE
 */
void	ft_lstadd_front(t_list **lst, t_list *new_lst)
{
	t_list	*head;

	if (lst == NULL)
		return ;
	head = *lst;
	*lst = new_lst;
	if (head != NULL)
		new_lst -> next = head;
}
