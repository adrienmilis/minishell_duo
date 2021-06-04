#include "parser.h"

int	reserved_chars(t_pars *p, t_package *s)
{
	t_pipe_cmd	*n_elem;

	if (s->cmd[p->i] == ';')
	{
		p->semicolon = 1;
		p->i += 1;
	}
	else if (s->cmd[p->i] == '>' || s->cmd[p->i] == '<')
		return (real_sign(p, s));
	else if (s->cmd[p->i] == '|')
	{
		n_elem = new_elem();
		if (!n_elem)
			error_free_pars(s->cmd, s->b_list, s->p_begin);
		ft_lstadd_back(&s->p_begin, n_elem);
		p->i += 1;
	}
	return (1);
}

void	argument_w_spaces(char *word, int append, t_pars *p, t_package *s)
{
	char	*new_word;
	int		i;

	i = 0;
	new_word = make_word(word, p, s);
	while (new_word)
	{
		if (append && !is_space(word[0]))
		{
			if (!(append_arg(ft_lstlast(s->p_begin), new_word, NULL)))
			{
				free(word);
				free(new_word);
				error_free_pars(s->cmd, s->b_list, s->p_begin);
			}
			append = 0;
		}
		else
			add_argument(new_word, s, word);
		new_word = make_word(word, p, s);
	}
	free(word);
}

void	do_append(t_pars *p, t_package *s)
{
	char		*word;
	t_pipe_cmd	*last;

	word = get_next_word(p, s);
	if (word)
	{
		if (p->word_from_variable && space_in_word(word))
			argument_w_spaces(word, 1, p, s);
		else
		{
			last = ft_lstlast(s->p_begin);
			if (!append_arg(last, word, NULL))
			{
				free(word);
				error_free_pars(s->cmd, s->b_list, s->p_begin);
			}
		}
	}
	p->word_from_variable = 0;
}

void	do_add(t_pars *p, t_package *s)
{
	char	*word;

	word = get_next_word(p, s);
	if (word)
	{
		if (p->word_from_variable && space_in_word(word))
			argument_w_spaces(word, 0, p, s);
		else
			add_argument(word, s, NULL);
	}
	p->word_from_variable = 0;
}

int	out_quotes(t_pars *p, t_package *s)
{
	if (is_r_resvd_char(&s->cmd[p->i], p->i, 0))
		return (reserved_chars(p, s));
	else if (is_r_space(&s->cmd[p->i], p->i))
	{
		p->i += 1;
		return (out_quotes(p, s));
	}
	else if (s->cmd[p->i] == '\'')
		p->in_s_quotes = 1;
	else if (s->cmd[p->i] == '"')
		p->in_d_quotes = 1;
	else if (must_append(p->i, s->cmd, p))
		do_append(p, s);
	else
		do_add(p, s);
	return (1);
}
