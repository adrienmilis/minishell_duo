/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmesnard <hmesnard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 18:09:09 by hmesnard          #+#    #+#             */
/*   Updated: 2021/05/27 11:19:14 by hmesnard         ###   ########.fr       */
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

int		ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int		ft_atoi(const char *nptr)
{
	int					i;
	unsigned long long	nb;
	int					sign;

	i = 0;
	nb = 0;
	sign = 1;
	while ((9 <= nptr[i] && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
		if (nptr[i++] == '-')
			sign = -1;
	while ('0' <= nptr[i] && nptr[i] <= '9')
	{
		nb = nb * 10 + (nptr[i++] - 48);
		if (nb > 9223372036854775807)
		{
			if (sign == 1)
				return (-1);
			return (0);
		}
	}
	return (nb * sign);
}

char	*ft_strchr(const char *s, int c)
{
	int		i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return ((char*)s + i);
	if (s[i] == c)
		return ((char*)s + i);
	return (NULL);
}
