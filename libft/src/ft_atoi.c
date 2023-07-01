/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 21:07:18 by sungjpar          #+#    #+#             */
/*   Updated: 2022/04/15 12:05:37 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isspace(const char c)
{
	const char	*spaces = "\t\n\r\v\f ";

	while (*spaces)
	{
		if (c == *spaces)
			return (1);
		++spaces;
	}
	return (0);
}

static int	issign(const char c)
{
	return (c == '-' || c == '+');
}

static int	get_sign(const char c)
{
	if (c == '-')
		return (-1);
	else
		return (1);
}

static int	get_res(int res, long long checker, int sign)
{
	if (checker < 0 && sign == 1)
		return (-1);
	else if (checker > 0 && sign == -1)
		return (0);
	else if (checker == -2147483648)
		return (-2147483648);
	return (res);
}

/* Function		:	ft_atoi
 * Description	:	 convert the initial portion of the string pointed
 * 					to by str to int respresntation
 * Param		
 * 			str	:	string to convert
 * Return Value	:	!0 	: converted number
 * 					-1	: can not convert( < LL_MIN) or number is -1
 * 					0 	: can not convert( > LL_MAX) or number is zero
 */
int	ft_atoi(const char *str)
{
	unsigned int	idx_str;
	int				res;
	int				sign;
	long long		checker;

	idx_str = 0;
	res = 0;
	checker = 0;
	while (isspace(str[idx_str]))
		++idx_str;
	sign = get_sign(str[idx_str]);
	while (issign(str[idx_str]))
			++idx_str;
	while (ft_isdigit(str[idx_str]))
	{
		res = res * 10 + sign * (str[idx_str] - '0');
		checker = checker * 10 + sign * (str[idx_str] - '0');
		++idx_str;
	}
	return (get_res(res, checker, sign));
}
