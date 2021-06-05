#include "execution.h"

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
		return (free_strtab(env));
	i = 0;
	while (env[i])
	{
		res[i] = ft_strdup(env[i]);
		if (!res[i++])
		{
			free_strtab(env);
			return (free_strtab(res));
		}
	}
	res[i] = ft_strdup(var);
	if (!res[i])
	{
		free_strtab(env);
		return (free_strtab(res));
	}
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
		return (free_strtab(env));
	i = 0;
	while (env[i])
	{
		res[i] = ft_strdup(env[i]);
		if (!res[i++])
		{
			free_strtab(env);
			return (free_strtab(res));
		}
	}
	res[i] = ft_strjoin(varname, varvalue, '=');
	if (!res[i])
	{
		free_strtab(env);
		return (free_strtab(res));
	}
	res[i + 1] = NULL;
	free_strtab(env);
	return (res);
}

char	**rm_env_var(char **env, char *var)
{
	char	**res;
	size_t	i;
	size_t	j;

	res = malloc((strtablen(env) + 1) * sizeof(char *));
	if (!res)
		return (free_strtab(env));
	i = 0;
	j = 0;
	while (env[i])
	{
		if (strcmp_env(env[i], var))
		{
			res[j] = ft_strdup(env[i]);
			if (!res[j++])
			{
				free_strtab(env);
				return (free_strtab(res));
			}
		}
		i++;
	}
	res[j] = NULL;
	free_strtab(env);
	return (res);
}
