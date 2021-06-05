#include "execution.h"

void	export_print_env(char *var)
{
	size_t	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '$' || var[i] == '\"' || var[i] == '\\')
			write(1, "\\", 1);
		write(1, var + i, 1);
		i++;
	}
}

int	builtin_export_no_arg(void)
{
	char	**copy_myenv;
	size_t	i;
	size_t	env_var_len;

	copy_myenv = copy_env(g_myenv + 1);
	if (!copy_myenv)
		return (0);
	sort_env(copy_myenv);
	i = 0;
	while (copy_myenv[i])
	{
		env_var_len = strlen_env_var(copy_myenv[i]);
		write(1, "declare -x ", 11);
		write(1, copy_myenv[i], env_var_len);
		if (var_has_value(copy_myenv[i]))
		{
			write(1, "=\"", 2);
			export_print_env(mygetenv(g_myenv, copy_myenv[i]));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		i++;
	}
	free_strtab(copy_myenv);
	return (1);
}

int	builtin_export_args(char **arg)
{
	while (*arg)
	{
		if (valid_env_name(*arg, 0))
		{
			if (var_is_in_env(g_myenv, *arg))
			{
				if (var_has_value(*arg) && !modif_env_var(g_myenv, *arg))
					return (0);
			}
			else
			{
				g_myenv = add_env_var(g_myenv, *arg);
				if (!g_myenv)
					return (0);
			}
		}
		else
		{
			write(2, "minishell: export: `", 20);
			write(2, *arg, ft_strlen(*arg));
			write(2, "\': not a valid identifier\n", 26);
		}
		arg++;
	}
	return (1);
}

int	builtin_export(char **arg, int pid, int pipes)
{
	if (pid == 0 && !*arg)
		return (builtin_export_no_arg());
	else if ((pid != 0 && !pipes) || (pid == 0 && pipes))
		return (builtin_export_args(arg));
	else if (pid == 0)
		while (*(arg++))
			if (!valid_env_name(*(arg - 1), 0))
				exit_status(1, pid, 0);
	return (1);
}
