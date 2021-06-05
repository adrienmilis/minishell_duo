#include "execution.h"

void	no_permission(char *cmd, t_pipe_cmd *begin, t_PATH *PATH)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": Permission denied\n", 20);
	free_exit_exec(126, begin, PATH);
}

void	stat_check(char *cmd, t_pipe_cmd *begin, t_PATH *PATH)
{
	struct stat	buf;

	ft_memset(&buf, 0, sizeof(struct stat));
	stat(cmd, &buf);
	if (S_ISDIR(buf.st_mode) && ft_strchr(cmd, '/'))
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": is a directory\n", 17);
		free_exit_exec(126, begin, PATH);
	}
	else if (S_ISREG(buf.st_mode) && ft_strchr(cmd, '/'))
	{
		if (!(buf.st_mode & S_IRUSR) || !(buf.st_mode & S_IXUSR))
			no_permission(cmd, begin, PATH);
		else
			free_exit_exec(0, begin, PATH);
	}
}

int	launch_executable_p(char **cmd, char *cmd0, t_PATH PATH, t_pipe_cmd *begin)
{
	int	i;

	i = 1;
	while (execve(cmd[0], cmd, g_myenv + 1) == -1 && PATH.split[i])
	{
		if (errno == EACCES && ft_strchr(cmd0, '/'))
			no_permission(cmd0, begin, &PATH);
		free(cmd[0]);
		cmd[0] = ft_strjoin(PATH.split[i++], cmd0, '/');
		if (!cmd[0])
			return (0);
	}
	if (errno == EACCES && ft_strchr(cmd[0], '/'))
		no_permission(cmd[0], begin, &PATH);
	return (1);
}

int	launch_executable_2(char **cmd, char *cmd0, t_PATH PATH, t_pipe_cmd *begin)
{
	if (ft_strchr(cmd[0], '/') || !(PATH.value && PATH.value[0]))
		execve(cmd[0], cmd, g_myenv + 1);
	if (errno == EACCES && ft_strchr(cmd[0], '/'))
		stat_check(cmd[0], begin, &PATH);
	cmd0 = cmd[0];
	if (PATH.value && PATH.value[0] && PATH.split[0])
	{
		cmd[0] = ft_strjoin(PATH.split[0], cmd0, '/');
		if (!cmd[0])
			return (0);
	}
	if (PATH.value && PATH.value[0])
		if (!launch_executable_p(cmd, cmd0, PATH, begin))
			return (0);
	stat_check(cmd0, begin, &PATH);
	write(2, "minishell: ", 11);
	write(2, cmd0, ft_strlen(cmd0));
	if (!ft_strchr(cmd0, '/') && PATH.value && PATH.value[0])
		write(2, ": command not found\n", 20);
	else
		write(2, ": No such file or directory\n", 28);
	free_exit_exec(127, begin, &PATH);
	return (1);
}

int	launch_executable(char **cmd, t_pipe_cmd *begin)
{
	char	*cmd0;
	t_PATH	PATH;

	if (!cmd[0])
		return (0);
	PATH.value = mygetenv(g_myenv, "PATH");
	PATH.split = NULL;
	if (PATH.value && PATH.value[0])
	{
		PATH.split = ft_split(PATH.value, ':');
		if (!PATH.split)
		{
			free(PATH.value);
			return (0);
		}
	}
	cmd0 = NULL;
	return (launch_executable_2(cmd, cmd0, PATH, begin));
}
