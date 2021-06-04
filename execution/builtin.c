#include "execution.h"

int	do_builtin(char **cmd, int pid, int pipes)
{
	if (!ft_strcmp("echo", cmd[0]))
		return (builtin_echo(cmd + 1, pid));
	else if (!ft_strcmp("cd", cmd[0]))
		return (builtin_cd(cmd + 1, pid, pipes));
	else if (!ft_strcmp("pwd", cmd[0]))
		return (builtin_pwd(pid));
	else if (!ft_strcmp("export", cmd[0]))
		return (builtin_export(cmd + 1, pid, pipes));
	else if (!ft_strcmp("unset", cmd[0]))
		return (builtin_unset(cmd + 1, pid, pipes));
	else if (!ft_strcmp("env", cmd[0]))
		return (builtin_env(pid));
	else if (!ft_strcmp("exit", cmd[0]))
		return (builtin_exit(cmd + 1, pid, pipes));
	else
		return (0);
}

int	is_builtin(char **cmd)
{
	if (!ft_strcmp("echo", cmd[0]))
		return (1);
	else if (!ft_strcmp("cd", cmd[0]))
		return (1);
	else if (!ft_strcmp("pwd", cmd[0]))
		return (1);
	else if (!ft_strcmp("export", cmd[0]))
		return (1);
	else if (!ft_strcmp("unset", cmd[0]))
		return (1);
	else if (!ft_strcmp("env", cmd[0]))
		return (1);
	else if (!ft_strcmp("exit", cmd[0]))
		return (1);
	else
		return (0);
}
