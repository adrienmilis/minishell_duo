/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmesnard <hmesnard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 18:09:09 by hmesnard          #+#    #+#             */
/*   Updated: 2021/05/26 11:57:34 by hmesnard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

size_t	ft_strlen(const char *str)
{
	size_t	res;

	res = 0;
	if (!str)
		return (res);
	while (str[res])
		res++;
	return (res);
}

char	*ft_strjoin(char const *s1, char const *s2, char c)
{
	char	*res;
	int		i;
	int		lens1;

	if (!s1 || !s2)
		return (NULL);
	lens1 = ft_strlen(s1);
	if (!(res = malloc((lens1 + ft_strlen(s2) + 1 + 1) * sizeof(char))))
		return (NULL);
	i = -1;
	while (s1[++i])
		res[i] = s1[i];
	res[lens1] = c;
	i = -1;
	while (s2[++i])
		res[lens1 + i + 1] = s2[i];
	res[lens1 + i + 1] = '\0';
	return (res);
}

char	*ft_strdup(const char *s)
{
	char	*res;
	int		i;

	if (!(res = malloc((ft_strlen(s) + 1) * sizeof(char))))
		return (NULL);
	i = -1;
	while (s[++i])
		res[i] = s[i];
	res[i] = '\0';
	return (res);
}

int		strcmp_decaps(const char *s1, const char *s2)
{
	unsigned int	i;
	char			s2i;

	i = 0;
	s2i = s2[0];
	while (s1[i] && s2i)
	{
		if ('A' <= s2i && s2i <= 'Z')
			s2i += 32;
		if (s1[i] != s2i)
			return ((unsigned char)s1[i] - (unsigned char)s2i);
		i++;
		s2i = s2[i];
	}
	return ((unsigned char)s1[i] - (unsigned char)s2i);
}
