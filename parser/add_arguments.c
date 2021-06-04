#include "parser.h"

void	add_argument2(char **new_args, char *word, t_pipe_cmd *last)
{
	char	**tmp;
	int		j;

	j = 0;
	if (last->cmd)
	{
		while (last->cmd[j])
		{
			new_args[j] = last->cmd[j];
			j++;
		}
	}
	new_args[j] = word;
	new_args[j + 1] = 0;
	tmp = last->cmd;
	last->cmd = new_args;
	free(tmp);
}

void	add_argument(char *word, t_package *s, char *word_to_free)
{
	int			i;
	char		**new_args;
	t_pipe_cmd	*last;

	if (!word)
		return ;
	last = ft_lstlast(s->p_begin);
	i = 0;
	if (last->cmd)
		while (last->cmd[i])
			i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
	{
		free(word);
		free(word_to_free);
		error_free_pars(s->cmd, s->b_list, s->p_begin);
	}
	add_argument2(new_args, word, last);
}

int	append_arg(t_pipe_cmd *last, char *word, char *tmp)
{
	char	*cat_arg;
	int		i;
	int		j;
	int		k;

	i = 0;
	while (last->cmd[i])
		i++;
	cat_arg = malloc((ft_strlen(last->cmd[i - 1])
				+ ft_strlen(word) + 1) * sizeof(char));
	if (!cat_arg)
		return (0);
	j = 0;
	k = 0;
	while (last->cmd[i - 1][j])
		cat_arg[k++] = last->cmd[i - 1][j++];
	j = 0;
	while (word[j])
		cat_arg[k++] = word[j++];
	cat_arg[k] = 0;
	tmp = last->cmd[i - 1];
	last->cmd[i - 1] = cat_arg;
	free(tmp);
	free(word);
	return (1);
}

char	*copy_next_word(t_pars *p, int wd_size, t_package *s)
{
	char	*word;
	int		j;

	if (wd_size == 1)
		return (NULL);
	word = malloc(sizeof(char) * wd_size);
	if (!word)
		error_free_pars(s->cmd, s->b_list, s->p_begin);
	j = 0;
	while (!is_r_space(&s->cmd[p->i], p->i)
		&& !is_r_resvd_char(&s->cmd[p->i], p->i, 1)
		&& !is_r_quote(&s->cmd[p->i], p->i) && s->cmd[p->i])
	{
		if (!(s->cmd[p->i] == '\\'
				&& !is_unesc_char(&s->cmd[p->i + 1], p->i + 1)))
			word[j++] = s->cmd[p->i];
		(p->i)++;
	}
	word[j] = 0;
	return (word);
}

char	*get_next_word(t_pars *p, t_package *s)
{
	char	*word;
	int		j;
	int		escaped;

	escaped = 0;
	while (is_r_space(&s->cmd[p->i], p->i))
		p->i += 1;
	if (s->cmd[p->i] == '$')
	{
		word = get_variable(p, s);
		p->word_from_variable = 1;
		return (word);
	}
	j = p->i;
	while (!is_r_space(&s->cmd[j], j)
		&& !is_r_resvd_char(&s->cmd[j], j, 1)
		&& !is_r_quote(&s->cmd[j], j) && s->cmd[j])
	{
		if (!is_unesc_char(&s->cmd[j], j))
			escaped++;
		j++;
	}
	word = copy_next_word(p, j - escaped - p->i + 1, s);
	return (word);
}
