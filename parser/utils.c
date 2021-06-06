#include "parser.h"

t_pipe_cmd	*init_pipe_list(void)
{
	t_pipe_cmd	*p_cmd_start;

	p_cmd_start = malloc(sizeof(t_pipe_cmd));
	if (!p_cmd_start)
		return (NULL);
	p_cmd_start->cmd = NULL;
	p_cmd_start->double_greater = 0;
	p_cmd_start->input = NULL;
	p_cmd_start->next = NULL;
	p_cmd_start->output = NULL;
	return (p_cmd_start);
}

void	init_pars_struct(t_pars *p, int new_command, char c)
{
	p->semicolon = 0;
	p->prev_var_w_space = 0;
	p->word_from_variable = 0;
	p->not_append = 0;
	p->r = 0;
	if (new_command)
		p->i = 0;
	if (c == '"')
	{
		p->in_d_quotes = 1;
		p->in_s_quotes = 0;
	}
	else if (c == '\'')
	{
		p->in_d_quotes = 0;
		p->in_s_quotes = 1;
	}
	else
	{
		p->in_d_quotes = 0;
		p->in_s_quotes = 0;
	}
}

void	reset_pars_struct(t_pars *p)
{
	p->in_d_quotes = 0;
	p->in_s_quotes = 0;
	p->semicolon = 0;
	p->prev_var_w_space = 0;
	p->word_from_variable = 0;
	p->not_append = 0;
	p->r = 0;
}

void	set_quotes(int i, char *cmd, t_pars *p)
{
	if (!p->in_s_quotes && !p->in_d_quotes)
	{
		if (is_unesc_simple(&cmd[i], i))
			p->in_s_quotes = 1;
		else if (is_unesc_double(&cmd[i], i))
			p->in_d_quotes = 1;
	}
	else if (p->in_s_quotes && !p->in_d_quotes)
	{
		if (cmd[i] == '\'')
			p->in_s_quotes = 0;
	}
	else if (!p->in_s_quotes && p->in_d_quotes)
		if (is_unesc_double(&cmd[i], i))
			p->in_d_quotes = 0;
}
