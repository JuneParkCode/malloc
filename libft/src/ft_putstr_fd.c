/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:39:43 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/11 15:59:41 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

/* Function		:	ft_putstr_fd
 * Description	:	Outputs the string s to the given file descriptor.
 * Param		
 * 			s	:	The string to output.
 * 			fd	:	The file descriptor on which to write.
 * Return Value	:	NONE
 */
size_t	ft_putstr_fd(char *s, int fd)
{
	size_t	len;

	if (s == NULL)
		return (ft_putstr_fd("(null)", fd));
	len = ft_strlen(s);
	write(fd, s, len);
	return (len);
}
