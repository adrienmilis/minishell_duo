#include "execution.h"

void	redirections(t_pipe_cmd *pipe_cmd, t_pipe_cmd *begin, int fd[3][2], \
		t_exec_data d)
{
	if (!d.firstcmd && !pipe_cmd->input)
		dup2(fd[0][0], STDIN_FILENO);
	else if (pipe_cmd->input)
	{
		fd[2][0] = open(pipe_cmd->input, O_RDONLY);
		if (fd[2][0] == -1)
			error_input(pipe_cmd->input, begin, fd, d);
		dup2_and_close(fd[2][0], STDIN_FILENO);
	}
	if (pipe_cmd->next && !pipe_cmd->output)
		dup2(fd[1][1], STDOUT_FILENO);
	else if (pipe_cmd->output)
	{
		if (!pipe_cmd->double_greater)
			fd[2][1] = open(pipe_cmd->output, O_WRONLY | O_TRUNC);
		else
			fd[2][1] = open(pipe_cmd->output, O_WRONLY | O_APPEND);
		if (fd[2][1] >= 0)
			dup2_and_close(fd[2][1], STDOUT_FILENO);
	}
	close(fd[1][0]);
	close(fd[1][1]);
}

int	end_execution(int *backslash, int fd[3][2], int status)
{
	if (WIFEXITED(status))
	{
		status = WEXITSTATUS(status);
		free(g_myenv[0]);
		g_myenv[0] = itoa_env_var("?=", status);
		if (!g_myenv[0])
			return (0);
	}
	if (ctrl_execution(1))
	{
		*backslash = 1;
		free(g_myenv[0]);
		g_myenv[0] = itoa_env_var("?=", ctrl_execution(0));
		if (!g_myenv[0])
			return (0);
	}
	close(fd[1][0]);
	close(fd[1][1]);
	canonical_mode(0);
	return (1);
}

int	exec_simple_cmd_2(t_pipe_cmd **pipe_cmd, t_pipe_cmd *begin, int fd[3][2], \
		t_exec_data *d)
{
	if ((*pipe_cmd)->cmd)
	{
		if (is_builtin((*pipe_cmd)->cmd))
		{
			if (!do_builtin((*pipe_cmd)->cmd, d->pid, d->pipes))
				return (error_exec(begin, fd, *d));
		}
		else if (d->pid == 0)
			if (!launch_executable((*pipe_cmd)->cmd, begin))
				return (error_exec(begin, fd, *d));
	}
	if (d->pid == 0)
		free_exit_exec(exit_status(0, d->pid, 0), begin, NULL);
	waitpid(d->pid, &d->status, 0);
	if (exit_status(0, 0, 1))
		free_exit_exec(exit_status(0, d->pid, 0), begin, NULL);
	*pipe_cmd = (*pipe_cmd)->next;
	d->firstcmd = 0;
	return (1);
}

int	exec_simple_cmd(t_pipe_cmd **pipe_cmd, t_pipe_cmd *begin, int fd[3][2], \
		t_exec_data *d)
{
	fd[0][0] = fd[1][0];
	fd[0][1] = fd[1][1];
	if (pipe(fd[1]) == -1)
		return (error_exec(begin, fd, *d));
	signal(SIGINT, &handler_signal_execution);
	signal(SIGQUIT, &handler_signal_execution);
	d->pid = fork();
	if (d->pid == -1)
		return (error_exec(begin, fd, *d));
	if (d->pid == 0)
		redirections(*pipe_cmd, begin, fd, *d);
	if (!d->firstcmd)
	{
		close(fd[0][0]);
		close(fd[0][1]);
	}
	return (exec_simple_cmd_2(pipe_cmd, begin, fd, d));
}

int	exec_pipe_cmd(t_pipe_cmd *pipe_cmd, int *backslash)
{
	t_pipe_cmd	*begin;
	int			fd[3][2];
	t_exec_data	d;

	begin = pipe_cmd;
	*backslash = 0;
	d.firstcmd = 1;
	d.pipes = 0;
	d.pid = -2;
	if (pipe_cmd->next)
		d.pipes = 1;
	canonical_mode(1);
	while (pipe_cmd && !ctrl_execution(1) && (pipe_cmd->cmd || pipe_cmd->input))
		if (!exec_simple_cmd(&pipe_cmd, begin, fd, &d))
			return (0);
	return (end_execution(backslash, fd, d.status));
}
