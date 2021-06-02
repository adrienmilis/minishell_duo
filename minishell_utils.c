#include "msh.h"

int	ft_isprint(int c)
{
	if (c >= ' ' && c < 127)
		return (1);
	return (0);
}

void	ft_putstr(char *str)
{
	int i;
	int str_len;

	if (str == NULL)
		return ;
	i = 0;
	str_len = ft_strlen(str);
	write(1, str, str_len);
}

void	error_free(char *buffer, t_command *begin_list)
{
	if (buffer)
		free(buffer);
	free_list(begin_list);
	if (myenv)
		free_strtab(myenv);
	ft_putstr("minishell: fatal error\n");
	exit(0);
}

int	init_termcap()
{
	int		ret;
	char	*term_type;

	term_type = getenv("TERM");
	if (!term_type)
		return (-1);
	ret = tgetent(NULL, term_type);
	if (ret == -1 || ret == 0)
		return (-1);
	return (0);
}

int	shlvl()
{
	int		shlvl;
	size_t	i;

	if (!var_is_in_env(myenv, "SHLVL"))
	{
		myenv = add_env_var_value(myenv, "SHLVL", "1");
		if (!myenv)
			return (0);	// free t_command, myenv;
	}
	else
	{
		i = 0;
		while (strcmp_env(myenv[i], "SHLVL"))
			i++;
		shlvl = ft_atoi(mygetenv(myenv, "SHLVL")) + 1; // proteger ft_atoi
		if (shlvl > 1000)
		{
			write(2, "minishell: warning: shell level (", 33);
			ft_putnbr_fd(shlvl, 2);
			write(2, ") too high, resetting to 1\n", 27);
			shlvl = 1;
		}
		else if (shlvl < 0)
			shlvl = 0;
		free(myenv[i]);
		myenv[i] = itoa_env_var("SHLVL=", shlvl);
		if (!myenv[i])
			return (0); // free t_command, myenv
	}
	return (1);
}

char	*make_buffer(char *buf, char c, t_command *begin_list)
{
	char	*buf_cpy;
	int		buf_len;
	int		i;

	i = 0;
	buf_len = ft_strlen(buf);
	buf_cpy = malloc((buf_len + 2) * sizeof(char));
	if (buf_cpy == NULL)
	{
		free(buf);
		return (NULL);
	}
	while (i < buf_len)
	{
		buf_cpy[i] = buf[i];
		i++;
	}
	buf_cpy[i] = c;
	buf_cpy[i + 1] = '\0';
	if (!pointer_in_history(buf, begin_list))
	free(buf);
	return (buf_cpy);
}

void	del_char_buffer(char **buffer, t_command *begin_list)
{
	char	*new_buffer;
	int		i;
	char	*tmp;
	char	*old_buf;

	if (pointer_in_history(*buffer, begin_list))
		old_buf = ft_strdup(*buffer);
	else
		old_buf = *buffer;
	i = 1;
	new_buffer = malloc(ft_strlen(old_buf) * sizeof(char));
	if (!new_buffer)
		error_free(*buffer, begin_list);	// free t_command, myenv, buffer
	while (old_buf[i])
	{
		new_buffer[i - 1] = old_buf[i - 1];
		i++;
	}
	new_buffer[i - 1] = 0;
	tmp = old_buf;
	*buffer = new_buffer;
	if (tmp)
		free(tmp);
	tputs(tgetstr("le", NULL), 1, ft_putchar);
	tputs(tgetstr("dc", NULL), 1, ft_putchar);
}
