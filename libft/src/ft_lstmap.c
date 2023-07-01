/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:54:38 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/16 14:07:14 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function		:	ft_lstmap
 * Description	:	Iterates the list ’lst’ and applies the function
 * 					’f’ to the content of each element. Creates a new
 * 					list resulting of the successive applications of
 * 					the function ’f’. The ’del’ function is used to
 * 					delete the content of an element if needed.
 * Param		
 * 		lst		:	The adress of a pointer to an element.
 * 		f		:	The adress of the function used to iterate on the list.
 *		del		:	The adress of the function used to delete the content of an
 *					element if needed.
 * Return Value	:	The new list. 
 * 					NULL : the allocation failed
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list			*cur_node_oldlst;
	t_list			*cur_node_newlst;
	t_list			*new_node;
	t_list			*res;

	cur_node_oldlst = lst;
	cur_node_newlst = NULL;
	while (cur_node_oldlst)
	{
		new_node = ft_lstnew(f(cur_node_oldlst -> content));
		if (new_node == NULL)
		{
			ft_lstclear(&new_node, del);
			return (NULL);
		}
		if (cur_node_newlst == NULL)
			res = new_node;
		else
			cur_node_newlst -> next = new_node;
		cur_node_newlst = new_node;
		cur_node_oldlst = cur_node_oldlst -> next;
	}
	return (res);
}
