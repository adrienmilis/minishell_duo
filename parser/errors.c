#include "parser.h"

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

void	error_exit(char *msg, t_pipe_cmd *begin_list)
{
	free_pipe_cmd(begin_list);
	printf("minishell: %s\n", msg);
	exit(0);
}
