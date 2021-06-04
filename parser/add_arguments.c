#include "parser.h"

void	add_argument2(char **new_args, int i, char *word, t_pipe_cmd *last)
{
	char	**tmp;

	new_args[i] = word;
	new_args[i + 1] = 0;
	tmp = last->cmd;
	last->cmd = new_args;
	free(tmp);
}

void	add_argument(char *word, t_pipe_cmd *p_begin, char *cmd, t_command *b_list)
{
	int			i;
	char		**new_args;
	t_pipe_cmd	*last;

	if (!word)
		return ;
	last = ft_lstlast(p_begin);
	i = 0;
	if (last->cmd)
		while (last->cmd[i])
			i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		error_free_pars(cmd, b_list, p_begin);
	i = 0;
	if (last->cmd)
	{
		while (last->cmd[i])
		{
			new_args[i] = last->cmd[i];
			i++;
		}
	}
	add_argument2(new_args, i, word, last);
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

char	*copy_next_word(char *cmd, t_pars *p, int wd_size, t_pipe_cmd *p_begin, t_command *b_list)
{
	char	*word;
	int		j;

	if (wd_size == 1)
		return (NULL);
	word = malloc(sizeof(char) * wd_size);
	if (!word)
		error_free_pars(cmd, b_list, p_begin);
	j = 0;
	while (!is_r_space(&cmd[p->i], p->i)
		&& !is_r_resvd_char(&cmd[p->i], p->i, 1)
		&& !is_r_quote(&cmd[p->i], p->i) && cmd[p->i])
	{
		if (!(cmd[p->i] == '\\' && !is_unesc_char(&cmd[p->i + 1], p->i + 1)))
			word[j++] = cmd[p->i];
		(p->i)++;
	}
	word[j] = 0;
	return (word);
}

char	*get_next_word(char *cmd, t_pars *p, t_pipe_cmd *p_cmd_start, t_command *b_list)
{
	char	*word;
	int		j;
	int		escaped;

	escaped = 0;
	while (is_r_space(&cmd[p->i], p->i))
		p->i += 1;
	if (cmd[p->i] == '$')
	{
		word = get_variable(p, cmd, p_cmd_start, b_list);
		p->word_from_variable = 1;
		return (word);
	}
	j = p->i;
	while (!is_r_space(&cmd[j], j)
		&& !is_r_resvd_char(&cmd[j], j, 1)
		&& !is_r_quote(&cmd[j], j) && cmd[j])
	{
		if (!is_unesc_char(&cmd[j], j))
			escaped++;
		j++;
	}
	word = copy_next_word(cmd, p, j - escaped - p->i + 1, p_cmd_start, b_list);
	return (word);
}
