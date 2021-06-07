#include "parser.h"

void	doubleq_special(t_pars *p, char **buff, t_package *s)
{
	char	*tmp;

	if (s->cmd[p->i] == '$')
	{
		*buff = ft_strjoin_w_ns(*buff, get_variable(p, s));
		if (!(*buff))
			error_free_pars(*buff, s->b_list, s->p_begin);
	}
	else if (s->cmd[p->i] == '\\')
	{
		p->i += 1;
		if (s->cmd[p->i] != '"' && s->cmd[p->i] != '\\' && s->cmd[p->i] != '$')
		{
			*buff = ft_strjoin_w_ns(*buff, ft_strdup("\\"));
			if (!(*buff))
				error_free_pars(s->cmd, s->b_list, s->p_begin);
		}
		tmp = malloc(2 * sizeof(char));
		tmp[0] = s->cmd[p->i];
		tmp[1] = 0;
		*buff = ft_strjoin_w_ns(*buff, tmp);
		if (!(*buff))
			error_free_pars(s->cmd, s->b_list, s->p_begin);
		p->i += 1;
	}
}

char	*make_dquotes_arg(t_pars *p, t_package *s)
{
	char	*buff;
	char	*tmp;
	int		start;

	buff = NULL;
	if (s->cmd[p->i] == '"')
		return (ft_strdup(""));
	while (!(is_unesc_char(&s->cmd[p->i], p->i) && s->cmd[p->i] == '"'))
	{
		start = p->i;
		while (s->cmd[p->i] != '"' && s->cmd[p->i] != '\\'
			&& s->cmd[p->i] != '$')
			p->i += 1;
		tmp = ft_strdup_len(s->cmd + start, p->i - start);
		if (!tmp)
		{
			free(buff);
			error_free_pars(s->cmd, s->b_list, s->p_begin);
		}
		buff = ft_strjoin_w_ns(buff, tmp);
		if (!buff)
			error_free_pars(s->cmd, s->b_list, s->p_begin);
		doubleq_special(p, &buff, s);
	}
	return (buff);
}

char	*arg_double_quotes(t_pars *p, t_package *s)
{
	int		beg;
	char	*new_arg;

	beg = p->i;
	p->i += 1;
	new_arg = make_dquotes_arg(p, s);
	if (p->r)
	{
		p->i += 1;
		return (new_arg);
	}
	if (must_append(beg, s->cmd, p))
	{
		if (!(append_arg(ft_lstlast(s->p_begin), new_arg, NULL)))
		{
			free(new_arg);
			error_free_pars(s->cmd, s->b_list, s->p_begin);
		}
	}
	else
		add_argument(new_arg, s, NULL, p);
	p->i += 1;
	p->in_d_quotes = 0;
	return (NULL);
}

void	in_double_quotes(t_pars *p, t_package *s)
{
	if (s->cmd[p->i + 1] == '"')
	{
		if (!must_append(p->i, s->cmd, p))
			add_argument(ft_strdup(""), s, NULL, p);
		p->i += 2;
		p->in_d_quotes = 0;
	}
	else
		arg_double_quotes(p, s);
}
