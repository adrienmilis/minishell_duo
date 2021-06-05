#include "parser.h"

void	error_free_pars(char *buffer, t_command *b_list, t_pipe_cmd *p_begin)
{
	if (buffer)
		free(buffer);
	free_list(b_list);
	if (g_myenv)
		free_strtab(g_myenv);
	free_pipe_cmd(p_begin);
	ft_putstr("minishell: fatal error\n");
	exit(0);
}

void	set_exit_status(char *error, int status)
{
	ft_putstr("minishell: ");
	ft_putstr(error);
	ft_putstr("\n");
	free(g_myenv[0]);
	g_myenv[0] = itoa_env_var("?=", status);
}

void	free_pipe_elems(t_pipe_cmd *c)
{
	int	i;

	if (c->input)
		free(c->input);
	if (c->output)
		free(c->output);
	i = 0;
	if (c->cmd)
	{
		while (c->cmd[i])
			free(c->cmd[i++]);
		free(c->cmd);
	}
}

void	free_pipe_cmd(t_pipe_cmd *begin_list)
{
	t_pipe_cmd	*current;
	t_pipe_cmd	*prev;

	if (!begin_list)
		return ;
	prev = begin_list;
	current = prev->next;
	while (current)
	{
		if (prev)
		{
			free_pipe_elems(prev);
			free(prev);
		}
		prev = current;
		current = current->next;
	}
	if (prev)
	{
		free_pipe_elems(prev);
		free(prev);
	}
}
