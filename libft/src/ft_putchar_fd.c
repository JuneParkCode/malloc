/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:39:43 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/11 15:59:26 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/* Function		:	ft_putchar_fd
 * Description	:	Outputs the character ’c’ to the given file descriptor.
 * Param		
 * 			c	:	The character to output.
 * 			fd	:	The file descriptor on which to write.
 * Return Value	:	NONE
 */
size_t	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}
