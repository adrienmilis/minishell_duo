#include "parser.h"

void	append_simple(char *new_arg, t_package *s)
{
	if (!(append_arg(ft_lstlast(s->p_begin), new_arg, NULL)))
	{
		free(new_arg);
		error_free_pars(s->cmd, s->b_list, s->p_begin);
	}
}

char	*arg_simple_quotes(t_pars *p, t_package *s)
{
	int		beg;
	int		start;
	char	*new_arg;

	beg = p->i;
	p->i += 1;
	start = p->i;
	while (s->cmd[p->i] != '\'')
		p->i += 1;
	new_arg = ft_strdup_len(s->cmd + start, p->i - start);
	if (!new_arg)
		error_free_pars(s->cmd, s->b_list, s->p_begin);
	if (p->r)
	{
		p->i += 1;
		return (new_arg);
	}
	if (must_append(beg, s->cmd, p))
		append_simple(new_arg, s);
	else
		add_argument(new_arg, s, NULL);
	p->i += 1;
	p->in_s_quotes = 0;
	return (NULL);
}

void	in_simple_quotes(t_pars *p, t_package *s)
{
	if (s->cmd[p->i + 1] == '\'')
	{
		if (!must_append(p->i, s->cmd, p))
			add_argument(ft_strdup(""), s, NULL);
		p->i += 2;
		p->in_s_quotes = 0;
	}
	else
		arg_simple_quotes(p, s);
}
