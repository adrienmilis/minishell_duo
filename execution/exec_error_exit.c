#include "execution.h"

int	free_exit_exec(int status, t_pipe_cmd *pipe_cmd, t_PATH *PATH)
{
	if (g_myenv)
		free_strtab(g_myenv);
	free_pipe_cmd(pipe_cmd);
	if (PATH && PATH->split)
		free_strtab(PATH->split);
	exit(status);
}

void	error_input(char *input, t_pipe_cmd *begin, int pipefd[2][2], \
		t_exec_data d)
{
	write(2, "minishell: ", 11);
	write(2, input, ft_strlen(input));
	if (errno == EACCES)
		write(2, ": Permission denied\n", 20);
	else if (errno == ENOENT)
		write(2, ": No such file or directory\n", 28);
	if (!d.firstcmd)
	{
		close(pipefd[0][0]);
		close(pipefd[0][1]);
	}
	close(pipefd[1][0]);
	close(pipefd[1][1]);
	free_exit_exec(1, begin, NULL);
}

int	error_exec(t_pipe_cmd *beg, int pipefd[2][2], t_exec_data d)
{
	if (!d.firstcmd && d.pid < 0)
	{
		close(pipefd[0][0]);
		close(pipefd[0][1]);
	}
	if (d.pid == -1)
	{
		close(pipefd[1][0]);
		close(pipefd[1][1]);
	}
	else if (d.pid == 0)
		free_exit_exec(0, beg, NULL);
	return (0);
}
