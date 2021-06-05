#include "execution.h"

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

char	*itoa_env_var(char *prefix, int n)
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
