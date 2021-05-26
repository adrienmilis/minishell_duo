/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmesnard <hmesnard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 18:08:43 by hmesnard          #+#    #+#             */
/*   Updated: 2021/05/26 11:55:37 by hmesnard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	nb_mots(const char *str, char c)
{
	int i;
	int nbmots;

	i = 0;
	nbmots = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (!(str[i] == c) && str[i] != 0)
		{
			while (!(str[i] == c) && str[i] != 0)
				i++;
			nbmots++;
		}
	}
	return (nbmots);
}

static int	malloc_mots(const char *str, char c, char **res)
{
	int i;
	int mot;
	int nblettres;

	i = 0;
	mot = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (!(str[i] == c) && str[i] != 0)
		{
			nblettres = 0;
			while (!(str[i] == c) && str[i] != 0)
			{
				nblettres++;
				i++;
			}
			if (!(res[mot] = malloc((nblettres + 1) * sizeof(char))))
				return (0);
			mot++;
		}
	}
	return (1);
}

static void	fill_res(const char *str, char c, char **res)
{
	int i;
	int mot;
	int lettre;

	i = 0;
	mot = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (!(str[i] == c) && str[i] != 0)
		{
			lettre = 0;
			while (!(str[i] == c) && str[i] != 0)
			{
				res[mot][lettre] = str[i];
				lettre++;
				i++;
			}
			res[mot][lettre] = 0;
			mot++;
		}
	}
}

static void	free_res(char **res)
{
	int mot;

	mot = 0;
	while (res[mot])
		free(res[mot++]);
	free(res);
}

char		**ft_split(const char *str, char c)
{
	char	**res;
	int		nb;

	if (!str)
		return (NULL);
	nb = nb_mots(str, c);
	if (!(res = malloc((nb + 1) * sizeof(char*))))
		return (NULL);
	res[nb] = 0;
	if (!malloc_mots(str, c, res))
	{
		free_res(res);
		return (NULL);
	}
	fill_res(str, c, res);
	return (res);
}
