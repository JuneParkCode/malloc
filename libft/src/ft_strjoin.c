/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 16:31:46 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:34:22 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/* Function		:	ft_strjoin
 * Description	:	Allocates and returns a new string, which is the result of
 * 					the concatenation of ’s1’ and ’s2’.
 * Param		
 * 			s1	:	The prefix string.
 * 			s2	:	The suffix string.
 * Return Value	:	address of joined string
 * 					NULL : Memory allocation error
 */
char	*ft_strjoin(char const *s1, char const *s2)
{
	const size_t	len_s1 = ft_strlen(s1);
	const size_t	len_s2 = ft_strlen(s2);
	char			*res;

	res = malloc(sizeof(char) * (len_s1 + len_s2 + 1));
	if (res == NULL)
		return (NULL);
	ft_strlcpy(res, s1, len_s1 + 1);
	ft_strlcpy(res + len_s1, s2, len_s2 + 1);
	return (res);
}
