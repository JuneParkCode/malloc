/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sungjpar <sungjpar@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 17:09:00 by sungjpar          #+#    #+#             */
/*   Updated: 2022/03/21 13:31:44 by sungjpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static size_t	get_size(const char *s, char c)
{
	size_t	res;
	size_t	idx_str;

	if (s[0] == 0)
		return (0);
	res = 0;
	idx_str = 0;
	while (s[idx_str] == c && s[idx_str])
		++idx_str;
	while (s[idx_str])
	{
		while (s[idx_str] != c && s[idx_str])
			++idx_str;
		++res;
		while (s[idx_str] == c && s[idx_str])
			++idx_str;
	}
	return (res);
}

static char	**do_free(char **res, size_t size)
{
	size_t	idx_res;

	if (res == NULL)
		return (NULL);
	idx_res = 0;
	while (idx_res < size)
	{
		free(res[idx_res]);
		++idx_res;
	}
	free(res);
	return (NULL);
}

static char	*get_word(const char *s, const int end, const int start)
{
	char	*word;

	word = malloc(sizeof(char) * (end - start + 1));
	if (word == NULL)
		return (NULL);
	ft_memset(word, 0, end - start + 1);
	ft_memcpy(word, &s[start], end - start);
	return (word);
}

/* Function		:	ft_split
 * Description	:	Allocates (with malloc(3)) and returns an array
 * 					of strings obtained by splitting ’s’ using the
 * 					character ’c’ as a delimiter. The array must be
 * 					ended by a NULL pointer.
 * Param		
 * 			str	:	string to convert
 * Return Value	:	splited strings with NULL at end
 * 					NULL: can not convert or number is zero
 */
char	**ft_split(char const *s, char c)
{
	char			**res;
	size_t			idx_res;
	size_t			idx_str;
	size_t			idx_start;

	res = malloc(sizeof(char *) * (get_size(s, c) + 1));
	if (res == NULL)
		return (NULL);
	idx_res = 0;
	idx_str = 0;
	while (s[idx_str] == c && s[idx_str])
		++idx_str;
	while (s[idx_str])
	{
		idx_start = idx_str;
		while (s[idx_str] != c && s[idx_str])
			++idx_str;
		res[idx_res] = get_word(s, idx_str, idx_start);
		if (res[idx_res++] == NULL)
			return (do_free(res, idx_res - 1));
		while (s[idx_str] == c && s[idx_str])
			++idx_str;
	}
	res[idx_res] = NULL;
	return (res);
}
