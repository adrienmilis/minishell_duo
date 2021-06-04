#include "parser.h"

void	doubleq_special(t_pars *p, char *cmd, t_pipe_cmd *p_begin, char **buff, t_command *b_list)
{
	// return un char et puis le join dans l'autre fct.
	char	*tmp;

	if (cmd[p->i] == '$')
	{
		*buff = ft_strjoin_w_ns(*buff, get_variable(p, cmd, p_begin, b_list));
	}
	else if (cmd[p->i] == '\\')
	{
		p->i += 1;
		if (cmd[p->i] != '"' && cmd[p->i] != '\\' && cmd[p->i] != '$')
		{
			*buff = ft_strjoin_w_ns(*buff, ft_strdup("\\"));
			if (!(*buff))
				error_exit("malloc error", p_begin);
		}
		tmp = malloc(2 * sizeof(char));
		tmp[0] = cmd[p->i];
		tmp[1] = 0;
		*buff = ft_strjoin_w_ns(*buff, tmp);
		if (!(*buff))
			error_exit("malloc error", p_begin);
		p->i += 1;
	}
}

char	*make_dquotes_arg(t_pars *p, char *cmd, t_pipe_cmd *p_begin, t_command *b_list)
{
	char	*buff;
	char	*tmp;
	int		start;

	buff = NULL;
	while (!(is_unesc_char(&cmd[p->i], p->i) && cmd[p->i] == '"'))
	{
		start = p->i;
		while (cmd[p->i] != '"' && cmd[p->i] != '\\' && cmd[p->i] != '$')
			p->i += 1;
		tmp = ft_strdup_len(cmd + start, p->i - start);
		if (!tmp)
			error_free_pars(cmd, b_list, p_begin);
		buff = ft_strjoin_w_ns(buff, tmp);
		if (!buff)
		{
			free(tmp);
			error_free_pars(cmd, b_list, p_begin);
		}
		if (!buff)
		{
			free(tmp);
			error_free_pars(cmd, b_list, p_begin);
		}
		doubleq_special(p, cmd, p_begin, &buff, b_list);
	}
	return (buff);
}

char	*arg_double_quotes(t_pars *p, t_pipe_cmd *p_begin, char *cmd, t_command *b_list)
{
	int		beg;
	char	*new_arg;

	beg = p->i;
	p->i += 1;
	new_arg = make_dquotes_arg(p, cmd, p_begin, b_list);
	if (p->r)
	{
		p->i += 1;
		return (new_arg);
	}
	if (must_append(beg, cmd, p))
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
	p->in_d_quotes = 0;
	return (NULL);
}

void	in_double_quotes(t_pars *p, t_pipe_cmd *p_begin, char *cmd, t_command *b_list)
{
	if (cmd[p->i + 1] == '"')
	{
		if (!must_append(p->i, cmd, p))
			add_argument(ft_strdup(""), p_begin, cmd, b_list);
		p->i += 2;
		p->in_d_quotes = 0;
	}
	else
		arg_double_quotes(p, p_begin, cmd, b_list);
}
