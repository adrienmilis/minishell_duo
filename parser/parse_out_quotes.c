#include "parser.h"

char 	*get_redir_word(char *cmd, t_pars *p, t_pipe_cmd *p_begin, t_command *b_list)
{
	char	*new_word;
	char	*word;

	while (is_space(cmd[p->i]))
		p->i += 1;
	p->r = 1;
	if (cmd[p->i] == '\'')
		word = arg_simple_quotes(p, p_begin, cmd, b_list);
	else if (cmd[p->i] == '"')
		word = arg_double_quotes(p, p_begin, cmd, b_list);
	else
		word = get_next_word(cmd, p, p_begin, b_list);
	while (!is_r_space(&cmd[p->i], p->i) && !is_r_resvd_char(&cmd[p->i], p->i, 0) && cmd[p->i])
	{
		if (cmd[p->i] == '\'')
			word = ft_strjoin_w_ns(word, arg_simple_quotes(p, p_begin, cmd, b_list));
		else if (cmd[p->i] == '"')
			word = ft_strjoin_w_ns(word, arg_double_quotes(p, p_begin, cmd, b_list));
		else
		{
			new_word = get_next_word(cmd, p, p_begin, b_list);
			// printf("new word [%s]\n", new_word);
			word = ft_strjoin_w_ns(word, new_word);
			// printf("cat word [%s]\n", word);
		}
	}
	p->r = 0;
	return (word);
}

int	real_sign2(t_pars *p, t_pipe_cmd *p_begin, char *cmd, char sign, t_command *b_list)
{
	char		*word;
	t_pipe_cmd	*last;
	int			fd;

	last = ft_lstlast(p_begin);
	word = get_redir_word(cmd, p, p_begin, b_list);
	if ((!word && p->var_not_exist) || (word && space_middle_of_word(word) && p->word_from_variable))
	{
		free(word);
		set_exit_status("ambiguous redirect", 1);
		return (0);			
	}
	if (sign == '>')
	{
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
	}
	else if (sign == '<')
	{
		if (!word)
		{
			set_exit_status("syntax error near unexpected token `<'", 2);
			return (0);
		}
		if (last->input)
			free(last->input);
		last->input = word;
	}
	return (1);
}

int	real_sign(t_pars *p, t_pipe_cmd *p_cmd_start, char *cmd, t_command *b_list)
{
	t_pipe_cmd	*last;
	char		sign;

	sign = cmd[p->i];
	last = ft_lstlast(p_cmd_start);
	if (cmd[p->i] == '>')
	{
		if (cmd[p->i + 1] == '>')
		{
			p->i += 1;
			last->double_greater = 1;
		}
		else
			last->double_greater = 0;
	}
	p->i += 1;
	return (real_sign2(p, p_cmd_start, cmd, sign, b_list));
}

int	reserved_chars(t_pars *p, t_pipe_cmd *p_begin, char *cmd, t_command *b_list)
{
	t_pipe_cmd	*n_elem;

	if (cmd[p->i] == ';')
	{
		p->semicolon = 1;
		p->i += 1;
	}
	else if (cmd[p->i] == '>' || cmd[p->i] == '<')
		return (real_sign(p, p_begin, cmd, b_list));
	else if (cmd[p->i] == '|')
	{
		n_elem = new_elem();
		if (!n_elem)
			error_free_pars(cmd, b_list, p_begin);
		ft_lstadd_back(&p_begin, n_elem);
		p->i += 1;
	}
	return (1);
}

int	must_append(int i, char *cmd, t_pars *p)
{
	if (p->var_not_exist == 1)
	{
		p->var_not_exist = 0;
		return (0);
	}
	if (p->prev_var_w_space == 1)
	{
		p->prev_var_w_space = 0;
		return (0);
	}
	if (i != 0 && !is_r_space(&cmd[i - 1], i - 1) && !is_r_resvd_char(&cmd[i - 1], i - 1, 0))
		return (1);
	return (0);
}

char	*make_word(char	*word, t_pipe_cmd *p_begin, t_pars *p, char *cmd, t_command *b_list)
{
	char		*new_word;
	int			j;
	static int	i;

	while (is_space(word[i]) && word[i])	// !!!! tester une variable avec que des espaces ???
		i++;
	j = i;
	while (!is_space(word[i]) && word[i])
		i++;
	if (j == i)
	{
		if (is_space(word[i - 1]))
			p->prev_var_w_space = 1;
		i = 0;
		return (NULL);
	}
	new_word = ft_strdup_len(word + j, i - j);
	if (!new_word)
		error_free_pars(cmd, b_list, p_begin);
	return (new_word);
}

void	argument_w_spaces(char *word, int append, t_pipe_cmd *p_begin, t_pars *p, t_command *b_list, char *cmd)
{
	char	*new_word;
	int		i;

	i = 0;
	new_word = make_word(word, p_begin, p, cmd, b_list);
	while (new_word)
	{
		if (append && !is_space(word[0]))
		{
			if (!(append_arg(ft_lstlast(p_begin), new_word, NULL)))
				error_free_pars(cmd, b_list, p_begin);
			append = 0;
		}
		else
			add_argument(new_word, p_begin, cmd, b_list);
		new_word = make_word(word, p_begin, p, cmd, b_list);
	}
	free(word);
}

int	out_quotes(t_pars *p, t_pipe_cmd *p_begin, char *cmd, t_command *b_list)
{
	char	*word;

	if (is_r_resvd_char(&cmd[p->i], p->i, 0))
		return (reserved_chars(p, p_begin, cmd, b_list));
	else if (is_r_space(&cmd[p->i], p->i))
	{
		p->i += 1;
		return (out_quotes(p, p_begin, cmd, b_list));
	}
	else if (cmd[p->i] == '\'')
		p->in_s_quotes = 1;
	else if (cmd[p->i] == '"')
		p->in_d_quotes = 1;
	else if (must_append(p->i, cmd, p)/*p->i != 0 && !is_r_space(&cmd[p->i - 1], p->i - 1) && !is_r_resvd_char(&cmd[p->i - 1], p->i - 1, 0)*/)
	{
		word = get_next_word(cmd, p, p_begin, b_list);
		if (word)
		{
			if (p->word_from_variable && space_in_word(word))
				argument_w_spaces(word, 1, p_begin, p, b_list, cmd);
			else
				if (!append_arg(ft_lstlast(p_begin), word, NULL))
					error_free_pars(cmd, b_list, p_begin);
		}
		p->word_from_variable = 0;
	}
	else	// on est au debut d'un mot et pas de quote avant
	{
		word = get_next_word(cmd, p, p_begin, b_list);
		if (word)
		{
			if (p->word_from_variable && space_in_word(word))
				argument_w_spaces(word, 0, p_begin, p, b_list, cmd);
			else
				add_argument(word, p_begin, cmd, b_list);
		}
		p->word_from_variable = 0; // ici ?
	}
	return (1);
}
