/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:14:15 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/15 20:09:39 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/* Function		:	ft_lstnew
 * Description	:	Allocates (with malloc(3)) and returns a new 
 * 					element. The variable ’content’ is initialize
 * 					with the value of the parameter ’content’. The
 * 					variable ’next’ is initialized to NULL.
 * Param		
 * 		content	:	The content to create the new element with.
 */
t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return (NULL);
	new_node -> content = content;
	new_node -> next = NULL;
	return (new_node);
}
