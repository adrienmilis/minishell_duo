#include "execution.h"

int	builtin_pwd(int pid)
{
	char	*pwd;

	if (pid == 0)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			return (0);
		write(1, pwd, ft_strlen(pwd));
		write(1, "\n", 1);
		free(pwd);
	}
	return (1);
}

int	builtin_unset_args(char **arg)
{
	while (*arg)
	{
		if (valid_env_name(*arg, 1))
		{
			if (var_is_in_env(myenv, *arg))
			{
				myenv = rm_env_var(myenv, *arg);
				if (!myenv)
					return (0);
			}
		}
		else
		{
			write(2, "minishell: unset: `", 19);
			write(2, *arg, ft_strlen(*arg));
			write(2, "\': not a valid identifier\n", 26);
		}
		arg++;
	}
	return (1);
}

int	builtin_unset(char **arg, int pid, int pipes)
{
	if ((pid != 0 && !pipes) || (pid == 0 && pipes))
		return (builtin_unset_args(arg));
	else if (pid == 0)
		while (*(arg++))
			if (!valid_env_name(*(arg - 1), 1))
				exit_status(1, pid, 0);
	return (1);
}

int	builtin_env(int pid)
{
	size_t	i;

	if (pid == 0)
	{
		i = 1;
		while (myenv[i])
		{
			if (var_has_value(myenv[i]))
			{
				write(1, myenv[i], ft_strlen(myenv[i]));
				write(1, "\n", 1);
			}
			i++;
		}
	}
	return (1);
}
