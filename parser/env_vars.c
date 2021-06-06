#include "parser.h"

char	*dup_dollar(t_package *s)
{
	char	*word;

	word = ft_strdup("$");
	if (!word)
		error_free_pars(s->cmd, s->b_list, s->p_begin);
	return (word);
}

char 	*unvalid_var_name(t_pars *p, t_package *s, char *word, char *tmp_word)
{
	int		i;
	int		j;

	if (is_space(s->cmd[p->i]) || s->cmd[p->i] == 0 || s->cmd[p->i] == '"')
		return (dup_dollar(s));
	j = p->i;
	while (!is_unesc_char(&s->cmd[p->i], p->i))
		p->i += 1;
	tmp_word = ft_strdup_len(s->cmd + j, p->i - j);
	if (!tmp_word)
		error_free_pars(s->cmd, s->b_list, s->p_begin);
	word = malloc(sizeof(char) * (ft_strlen(tmp_word) + 2));
	if (!word)
	{
		free(tmp_word);
		error_free_pars(s->cmd, s->b_list, s->p_begin);
	}
	i = 1;
	j = 0;
	word[0] = '$';
	while (tmp_word[i - 1])
		word[i++] = tmp_word[j++];
	word[i] = 0;
	free(tmp_word);
	return (word);
}

char	*valid_var_name(t_pars *p, t_package *s)
{
	char	*var_name;
	int		start;
	int		j;
	char	*var_value;

	j = 0;
	start = p->i;
	while (valid_var_char(s->cmd[p->i]))
		p->i += 1;
	var_name = ft_strdup_len(s->cmd + start, p->i - start);
	if (!var_name)
		error_free_pars(s->cmd, s->b_list, s->p_begin);
	while (g_myenv[j] && strcmp_env(var_name, g_myenv[j]))
		j++;
	free(var_name);
	if (g_myenv[j] == 0)
		return (NULL);
	var_value = get_env_var(j);
	if (!var_value)
		error_free_pars(s->cmd, s->b_list, s->p_begin);
	return (var_value);
}

char	*get_variable(t_pars *p, t_package *s)
{
	char	*word;
	int		r_space;

	r_space = 0;
	if (is_r_space(&s->cmd[p->i - 1], p->i - 1))
		r_space = 1;
	p->i += 1;
	if (is_number(s->cmd[p->i]))
	{
		p->i += 1;
		return (NULL);
	}
	if (!valid_var_char(s->cmd[p->i]))
		word = unvalid_var_name(p, s, NULL, NULL);
	else
		word = valid_var_name(p, s);
	if (word == NULL && r_space)
		p->not_append = 1;
	return (word);
}
