#include "parser.h"

t_pipe_cmd	*parser(char *cmd, int new_command)
{
	t_pipe_cmd		*p_cmd_start;
	static t_pars	p;

	if (!cmd || !cmd[0])
		return (NULL);
	check_syntax(cmd);
	init_pars_struct(&p, new_command, cmd[0]);
	p_cmd_start = init_pipe_list();
	while (!p.semicolon && cmd[p.i])
	{
		if (!p.in_d_quotes && !p.in_s_quotes)
			out_quotes(&p, p_cmd_start, cmd);
		else if (p.in_s_quotes)
			in_simple_quotes(&p, p_cmd_start, cmd);
		else
			in_double_quotes(&p, p_cmd_start, cmd);
	}
	reset_pars_struct(&p);
	// print_list(p_cmd_start);
	return (p_cmd_start);
}
