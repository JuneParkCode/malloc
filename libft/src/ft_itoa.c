/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 15:54:57 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:27:19 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	get_size(int n)
{
	int				res;
	unsigned int	nb;

	res = n < 0 + 1;
	if (n == 0)
		return (res);
	if (n < 0)
		nb = -n;
	else
		nb = n;
	while (nb > 0)
	{
		res++;
		nb /= 10;
	}
	return (res);
}

void	get_string(char *res, int n, int size)
{
	unsigned int	nb;
	unsigned int	idx_res;

	idx_res = 0;
	if (n == 0)
	{
		res[0] = '0';
		return ;
	}
	if (n < 0)
	{
		nb = -n;
		res[idx_res++] = '-';
	}
	else
		nb = n;
	while (nb > 0)
	{
		res[--size] = nb % 10 + '0';
		nb /= 10;
	}
}

/* Function		:	ft_itoa
 * Description	:	Allocates (with malloc(3)) and returns a string
 * 					representing the integer received as an argument.
 * 					Negative numbers must be handled.
 * Param		
 * 			n	:	the integar to convert
 * Return Value	:	converted string
 * 					NULL 	: memory alloctaion error
 */
char	*ft_itoa(int n)
{
	const int		size = get_size(n);
	char			*res;

	res = malloc(size + 1);
	if (res == NULL)
		return (NULL);
	get_string(res, n, size);
	res[size] = 0;
	return (res);
}
