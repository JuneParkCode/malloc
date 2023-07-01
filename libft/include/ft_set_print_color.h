/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_print_color.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 13:24:50 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/15 15:03:35 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SET_PRINT_COLOR_H
# define FT_SET_PRINT_COLOR_H

# define ANSI_COLOR_RED "\033[31m"
# define ANSI_COLOR_GREEN "\033[32m"
# define ANSI_COLOR_YELLOW "\033[33m"
# define ANSI_COLOR_BLUE "\033[34m"
# define ANSI_COLOR_MAGENTA "\033[35m"
# define ANSI_COLOR_CYAN "\033[36m"
# define ANSI_COLOR_RESET "\033[0m"

/* BOLD */
# define ANSI_COLOR_RED_BOLD "\033[1;31m"
# define ANSI_COLOR_GREEN_BOLD "\033[1;32m"
# define ANSI_COLOR_YELLOW_BOLD "\033[1;33m"
# define ANSI_COLOR_BLUE_BOLD "\033[1;34m"
# define ANSI_COLOR_MAGENTA_BOLD "\033[1;35m"
# define ANSI_COLOR_CYAN_BOLD "\033[1;36m"

void	ft_set_print_color(const char *color);

#endif
