#include "execution.h"

int	valid_env_name(char *var, int unset)
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

int	var_is_in_env(char **env, char *var)
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

int	var_has_value(char *var)
{
	if (var[strlen_env_var(var)] == '=')
		return (1);
	return (0);
}

int	modif_env_var(char **env, char *var)
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

int	modif_env_var_value(char **env, char *varname, char *varvalue)
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
