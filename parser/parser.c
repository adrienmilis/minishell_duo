#include "parser.h"

void	init_package(t_package *s, t_command *b_list, char *cmd)
{
	t_pipe_cmd	*p_begin;

	p_begin = init_pipe_list();
	if (!p_begin)
		error_free_pars(cmd, b_list, NULL);
	s->b_list = b_list;
	s->cmd = cmd;
	s->p_begin = p_begin;
}

void	*out_quotes_failed(t_pars *p, t_package s)
{
	free_pipe_cmd(s.p_begin);
	reset_pars_struct(p);
	return (NULL);
}

t_pipe_cmd	*parser(char *cmd, int new_command, t_command *b_list)
{
	t_package		s;
	static t_pars	p;

	if (!cmd || !cmd[0] || !check_syntax(cmd))
		return (NULL);
	init_pars_struct(&p, new_command, cmd[0]);
	if (!cmd[p.i])
		return (NULL);
	init_package(&s, b_list, cmd);
	while (!p.semicolon && cmd[p.i])
	{
		if (!p.in_d_quotes && !p.in_s_quotes)
		{
			if (!out_quotes(&p, &s))
				return (out_quotes_failed(&p, s));
		}
		else if (p.in_s_quotes)
			in_simple_quotes(&p, &s);
		else
			in_double_quotes(&p, &s);
	}
	reset_pars_struct(&p);
	return (s.p_begin);
}
