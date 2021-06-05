#include "execution.h"

int	option_n(char *arg)
{
	size_t	i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
		if (arg[i++] != 'n')
			return (0);
	return (1);
}

int	builtin_echo(char **arg, int pid)
{
	int	nonl;
	int	option_string;

	nonl = 0;
	option_string = 1;
	if (pid == 0)
	{
		while (*(arg++))
		{
			if (option_string && !ft_strcmp("-", *(arg - 1)))
				option_string = 0;
			else if (option_string && option_n(*(arg - 1)))
				nonl = 1;
			else
			{
				option_string = 0;
				write(1, *(arg - 1), ft_strlen(*(arg - 1)));
				if (*arg)
					write(1, " ", 1);
			}
		}
		if (!nonl)
			write(1, "\n", 1);
	}
	return (1);
}

int	cd_oldpwd(void)
{
	if (!var_is_in_env(g_myenv, "OLDPWD"))
	{
		g_myenv = \
			add_env_var_value(g_myenv, "OLDPWD", mygetenv(g_myenv, "PWD"));
		if (!g_myenv)
			return (0);
	}
	else
		if (!modif_env_var_value(g_myenv, "OLDPWD", mygetenv(g_myenv, "PWD")))
			return (0);
	return (1);
}

int	cd_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	modif_env_var_value(g_myenv, "PWD", cwd);
	free(cwd);
	return (1);
}

int	builtin_cd(char **arg, int pid, int pipes)
{
	if (*arg && arg[0][0] != '\0')
	{
		if ((pid != 0 && !pipes) || (pid == 0 && pipes))
		{
			if (chdir(*arg) < 0)
			{
				write(2, "minishell: cd: ", 15);
				write(2, *arg, ft_strlen(*arg));
				if (errno == ENOENT)
					write(2, ": No such file or directory\n", 28);
				if (errno == EACCES)
					write(2, ": Permission denied\n", 20);
			}
			else
				if (!cd_oldpwd() || !cd_pwd())
					return (0);
		}
		else if (pid == 0 && chdir(*arg) < 0)
			exit_status(1, pid, 0);
	}
	else
		if (!cd_oldpwd())
			return (0);
	return (1);
}
