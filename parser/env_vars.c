#include "parser.h"

char	*gnw_double_quotes(t_pars *p, char *cmd, t_pipe_cmd *p_begin)
{
	char	*word;
	int		start;

	start = p->i;
	while (!is_space(cmd[p->i]) && cmd[p->i] != '"')
		p->i++;
	word = ft_strdup_len(cmd + start, p->i - start);
	if (!word)
		error_exit("malloc error", p_begin);
	return (word);
}

char	*dup_dollar(t_pipe_cmd *p_begin)
{
	char	*word;

	word = ft_strdup("$");
	if (!word)
		error_exit("malloc error", p_begin);
	return (word);
}

char 	*unvalid_var_name(t_pars *p, char *cmd, t_pipe_cmd *p_begin)
{
	char	*word;
	int		i;
	int		j;
	char	*tmp_word;

	if (is_space(cmd[p->i]) || cmd[p->i] == 0)
		return (dup_dollar(p_begin));
	/*if (!d_quo)
		tmp_word = get_next_word(cmd, p, p_begin);
	else
		tmp_word = gnw_double_quotes(p, cmd, p_begin);*/
	tmp_word = get_redir_word(cmd, p, p_begin);
	word = malloc(sizeof(char) * (ft_strlen(tmp_word) + 2));
	if (!word)
		error_exit("malloc error", p_begin);
	i = 1;
	j = 0;
	word[0] = '$';
	while (tmp_word[i - 1])
		word[i++] = tmp_word[j++];
	word[i] = 0;
	free(tmp_word);
	return (word);
}

char	*valid_var_name(t_pars *p, char *cmd, t_pipe_cmd *p_cmd_start)
{
	char	*var_name;
	int		start;
	int		j;
	char	*var_value;

	j = 0;
	start = p->i;
	while (valid_var_char(cmd[p->i]))
		p->i += 1;
	var_name = ft_strdup_len(cmd + start, p->i - start);
	if (!var_name)
		error_exit("malloc error", p_cmd_start);
	while (myenv[j] && strcmp_env(var_name, myenv[j]))
		j++;
	free(var_name);
	if (myenv[j] == 0)
		return (NULL);
	var_value = get_env_var(j);
	if (!var_value)
		error_exit("malloc error", p_cmd_start);
	return (var_value);
}

char	*get_variable(t_pars *p, char *cmd, t_pipe_cmd *p_begin)
{
	char	*word;

	p->i += 1;
	if (is_number(cmd[p->i]))
	{
		p->i += 1;
		return (NULL);
	}
	if (!valid_var_char(cmd[p->i]))
	{
		word = unvalid_var_name(p, cmd, p_begin);
	}
	else
		word = valid_var_name(p, cmd, p_begin);
	return (word);
}
