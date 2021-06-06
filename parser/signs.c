#include "parser.h"

int	real_greater_sign(char *word, t_pipe_cmd *last)
{
	int	fd;

	if (!word)
	{
		set_exit_status("syntax error near unexpected token `>'", 2);
		return (0);
	}
	if (last->output)
		free(last->output);
	if (!last->double_greater)
		fd = open(word, O_RDONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(word, O_RDONLY | O_CREAT, 0644);
	close(fd);
	last->output = word;
	return (1);
}

int	real_smaller_sign(char *word, t_pipe_cmd *last)
{
	if (!word)
	{
		set_exit_status("syntax error near unexpected token `<'", 2);
		return (0);
	}
	if (last->input)
		free(last->input);
	last->input = word;
	return (1);
}

int	do_sign(char sign, t_pipe_cmd *last, char *word)
{
	if (sign == '>')
	{
		if (!(real_greater_sign(word, last)))
			return (0);
	}
	else if (sign == '<')
	{
		if (!(real_smaller_sign(word, last)))
			return (0);
	}
	return (1);
}

int	real_sign2(t_pars *p, char sign, t_package *s)
{
	char		*word;
	t_pipe_cmd	*last;

	last = ft_lstlast(s->p_begin);
	word = get_redir_word(p, s, NULL, NULL);
	if ((!word && p->not_append) || (word && space_middle_of_word(word)
			&& p->word_from_variable))
	{
		free(word);
		set_exit_status("ambiguous redirect", 1);
		return (0);
	}
	if (!do_sign(sign, last, word))
		return (0);
	if (!word[0])
	{
		set_exit_status(": No such file or directory", 1);
		return (0);
	}
	return (1);
}

int	real_sign(t_pars *p, t_package *s)
{
	t_pipe_cmd	*last;
	char		sign;

	sign = s->cmd[p->i];
	last = ft_lstlast(s->p_begin);
	if (s->cmd[p->i] == '>')
	{
		if (s->cmd[p->i + 1] == '>')
		{
			p->i += 1;
			last->double_greater = 1;
		}
		else
			last->double_greater = 0;
	}
	p->i += 1;
	return (real_sign2(p, sign, s));
}
