/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:39:43 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/11 15:58:32 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

/* Function		:	ft_putendl_fd
 * Description	:	Outputs the string ’s’ to the given file descriptor, 
 * 					followed by a newline.
 * Param		
 * 			s	:	The string to output.
 * 			fd	:	The file descriptor on which to write.
 * Return Value	:	NONE
 */
size_t	ft_putendl_fd(char *s, int fd)
{
	size_t	len;

	len = 0;
	len += ft_putstr_fd(s, fd);
	len += ft_putstr_fd("\n", fd);
	return (len);
}
