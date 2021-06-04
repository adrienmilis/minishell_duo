#include "parser.h"

char	*arg_simple_quotes(t_pars *p, t_pipe_cmd *p_begin, char *cmd, t_command *b_list)
{
	int		beg;
	int		start;
	char	*new_arg;

	beg = p->i;
	p->i += 1;
	start = p->i;
	while (cmd[p->i] != '\'')
		p->i += 1;
	new_arg = ft_strdup_len(cmd + start, p->i - start);
	if (!new_arg)
		error_free_pars(cmd, b_list, p_begin);
	if (p->r)
	{
		p->i += 1;
		return (new_arg);
	}
	if (must_append(beg, cmd, p)/*beg != 0 && !is_r_space(&cmd[beg - 1], beg - 1)*/)
	{
		if (!(append_arg(ft_lstlast(p_begin), new_arg, NULL)))
		{
			free(new_arg);
			error_free_pars(cmd, b_list, p_begin);
		}
	}
	else
		add_argument(new_arg, p_begin, cmd, b_list);
	p->i += 1;
	p->in_s_quotes = 0;
	return (NULL);
}

void	in_simple_quotes(t_pars *p, t_pipe_cmd *p_begin, char *cmd, t_command *b_list)
{
	if (cmd[p->i + 1] == '\'')
	{
		if (!must_append(p->i, cmd, p))
			add_argument(ft_strdup(""), p_begin, cmd, b_list);
		p->i += 2;
		p->in_s_quotes = 0;
	}
	else
		arg_simple_quotes(p, p_begin, cmd, b_list);
}