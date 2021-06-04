#include "execution.h"

size_t	strlen_env_var(char *var)
{
	size_t	res;

	res = 0;
	if (!var)
		return (0);
	while (var[res] && var[res] != '=')
		res++;
	return (res);
}

int		strcmp_env(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	last_char_s1;
	unsigned char	last_char_s2;

	i = 0;
	while (s1[i] && s2[i] && s1[i] != '=' && s2[i] != '=')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	last_char_s1 = s1[i];
	last_char_s2 = s2[i];
	if (last_char_s1 == '=')
		last_char_s1 = '\0';
	if (last_char_s2 == '=')
		last_char_s2 = '\0';
	return (last_char_s1 - last_char_s2);
}

size_t	strtablen(char **strtab)
{
	size_t	res;

	res = 0;
	if (!strtab)
		return (0);
	while (strtab[res])
		res++;
	return (res);
}

char	**free_strtab(char **strtab)
{
	size_t	i;

	i = 0;
	while (strtab[i])
		free(strtab[i++]);
	free(strtab);
	return (NULL);
}

char	**new_env(char **env)
{
	char	**res;
	size_t	i;

	res = malloc((strtablen(env) + 2) * sizeof(char *));
	if (!res)
		return (NULL);
	res[0] = ft_strdup("?=0");
	if (!res[0])
		return (free_strtab(res));
	i = 0;
	while (env[i])
	{
		res[i + 1] = ft_strdup(env[i]);
		if (!res[i + 1])
			return (free_strtab(res));
		i++;
	}
	res[i + 1] = NULL;
	return (res);
}

char	**copy_env(char **env)
{
	char	**res;
	size_t	i;

	res = malloc((strtablen(env) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (env[i])
	{
		res[i] = ft_strdup(env[i]);
		if (!res[i])
			return (free_strtab(res));
		i++;
	}
	res[i] = NULL;
	return (res);
}

void	sort_env(char **env)
{
	size_t	tablen;
	size_t	i;
	char	*tmp;

	tablen = strtablen(env);
	i = 0;
	if (tablen > 0)
	{
		while (i < (tablen - 1))
		{
			if (strcmp_env(env[i], env[i + 1]) > 0)
			{
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
				if (i > 0)
					i--;
			}
			else
				i++;
		}
	}
}

char	**add_env_var(char **env, char *var)
{
	char	**res;
	size_t	i;

	res = malloc((strtablen(env) + 2) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (env[i])
	{
		res[i] = ft_strdup(env[i]);
		if (!res[i])
			return (free_strtab(res));
		i++;
	}
	res[i] = ft_strdup(var);
	if (!res[i])
		return (free_strtab(res));
	res[i + 1] = NULL;
	free_strtab(env);
	return (res);
}

char	**add_env_var_value(char **env, char *varname, char *varvalue)
{
	char	**res;
	size_t	i;

	res = malloc((strtablen(env) + 2) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (env[i])
	{
		res[i] = ft_strdup(env[i]);
		if (!res[i])
			return (free_strtab(res));
		i++;
	}
	res[i] = ft_strjoin(varname, varvalue, '=');
	if (!res[i])
		return (free_strtab(res));
	res[i + 1] = NULL;
	free_strtab(env);
	return (res);
}

char	**rm_env_var(char **env, char *var)
{
	char	**res;
	size_t	i;
	size_t	j;

	res = malloc(strtablen(env) * sizeof(char *)); // + 1 ici pour etre sur de pas segfault ?
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (strcmp_env(env[i], var))
		{
			res[j] = ft_strdup(env[i]);
			if (!res[j])
				return (free_strtab(res));
			j++;
		}
		i++;
	}
	res[j] = NULL;
	free_strtab(env);
	return (res);
}

int		valid_env_name(char *var, int unset) //remplacer par ft_is* si on met la libft
{
	size_t	i;

	if (!var)
		return (0);
	if (!('A' <= var[0] && var[0] <= 'Z')
			&& !('a' <= var[0] && var[0] <= 'z') && var[0] != '_')
		return (0);
	i = 1;
	while (var[i] && (var[i] != '=' || unset))
	{
		if (!('0' <= var[i] && var[i] <= '9')
				&& !('A' <= var[i] && var[i] <= 'Z')
				&& !('a' <= var[i] && var[i] <= 'z') && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int		var_is_in_env(char **env, char *var) // fusionner dans modif_env_var ? Probablement pas
{
	size_t	i;

	i = 1;
	if (!env || !env[0])
		return (0);
	while (env[i])
	{
		if (!strcmp_env(env[i], var))
			return (1);
		i++;
	}
	return (0);
}

int		var_has_value(char *var)
{
	if (var[strlen_env_var(var)] == '=')
		return (1);
	return (0);
}

int		modif_env_var(char **env, char *var)
{
	size_t	i;

	i = 1;
	if (!env || !env[0])
		return (0);
	while (env[i])
	{
		if (!strcmp_env(env[i], var))
		{
			free(env[i]);
			env[i] = ft_strdup(var);
			if (!env[i])
				return (0);
			return (1);
		}
		i++;
	}
	return (1);
}

int		modif_env_var_value(char **env, char *varname, char *varvalue)
{
	size_t	i;

	i = 1;
	if (!env || !env[0])
		return (0);
	while (env[i])
	{
		if (!strcmp_env(env[i], varname))
		{
			free(env[i]);
			env[i] = ft_strjoin(varname, varvalue, '=');
			if (!env[i])
				return (0);
			return (1);
		}
		i++;
	}
	return (1);
}

char	*mygetenv(char **env, char *var)
{
	size_t	i;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (!strcmp_env(env[i], var))
		{
			if (var_has_value(env[i]))
				return (env[i] + strlen_env_var(env[i]) + 1);
			else
				return (NULL);
		}
		i++;
	}
	return (NULL);
}

static int	nblen(unsigned int n)
{
	if (n < 10)
		return (1);
	return (1 + nblen(n / 10));
}

char		*itoa_env_var(char *prefix, int n)
{
	char			*res;
	int				len;
	unsigned int	posn;

	posn = n;
	if (n < 0)
		posn = -n;
	len = nblen(posn) + ft_strlen(prefix);
	if (n < 0)
		len++;
	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	res[len] = '\0';
	while ((unsigned int)--len >= ft_strlen(prefix))
	{
		res[len] = posn % 10 + 48;
		posn /= 10;
	}
	if (n < 0)
		res[ft_strlen(prefix)] = '-';
	ft_memmove(res, prefix, ft_strlen(prefix));
	return (res);
}
