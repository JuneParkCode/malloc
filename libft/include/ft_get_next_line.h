/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 14:18:47 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/15 14:58:21 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GET_NEXT_LINE_H
# define FT_GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <limits.h>

# ifndef OPEN_MAX
#  define OPEN_MAX 256
# endif

/* get_next_line.c */
static char	*get_line(char *str);
static char	*read_file(int fd, char *store[]);
static char	*get_residue(char *str);
static void	*free_and_make_null(void *addr);

#endif
