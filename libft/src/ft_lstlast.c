/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:26:33 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/13 19:17:31 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_lstlast
 * Description	:	Returns the last element of the list.
 * Param		
 * 		lst		:	The beginning of the list.
 * Return Value	:	Last element of the list.
 */

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*cur_node;

	if (lst == NULL)
		return (NULL);
	cur_node = lst;
	while (cur_node -> next)
		cur_node = cur_node -> next;
	return (cur_node);
}
