#include "parser.h"

t_pipe_cmd	*parser(char *cmd, int new_command, t_command *b_list)
{
	t_pipe_cmd		*p_cmd_start;
	static t_pars	p;

	if (!cmd || !cmd[0] || !check_syntax(cmd))
		return (NULL);
	init_pars_struct(&p, new_command, cmd[0]);
	if (!cmd[p.i])
		return (NULL);
	p_cmd_start = init_pipe_list();
	if (!p_cmd_start)
		error_free_pars(cmd, b_list, NULL);	// est-ce qu'on garde comme ca ?
	while (!p.semicolon && cmd[p.i])
	{
		if (!p.in_d_quotes && !p.in_s_quotes)
		{
			if (!out_quotes(&p, p_cmd_start, cmd, b_list))
			{
				free_pipe_cmd(p_cmd_start);
				return (NULL);
			}
		}
		else if (p.in_s_quotes)
			in_simple_quotes(&p, p_cmd_start, cmd, b_list);
		else
			in_double_quotes(&p, p_cmd_start, cmd, b_list);
	}
	reset_pars_struct(&p);
	// print_list(p_cmd_start);
	return (p_cmd_start);
}
