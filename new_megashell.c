#include "msh.h"

void	write_hst_command(t_command *elem, char **b, int up, t_command *b_list)
{
	int	len;

	len = ft_strlen(*b);
	while (len > 0)
	{
		termcaps_del();
		len--;
	}
	if (!pointer_in_history(*b, b_list) && *b)
	{
		free(*b);
		*b = NULL;
	}
	if ((!up && elem) || (up && elem->command))
	{
		*b = ft_strdup(elem->command);
		ft_putstr(elem->command);
	}
	else
		*b = NULL;
}

int	main2(char *buffer, t_command *begin_list)
{
	struct termios	s_termios;
	struct termios	s_termios_backup;
	int				ret;

	ret = 1;
	if (init_termcap() == 0)
	{
		if (tcgetattr(0, &s_termios) == -1)
			return (-1);
		if (tcgetattr(0, &s_termios_backup) == -1)
			return (-1);
		s_termios.c_lflag &= ~(ICANON);
		s_termios.c_lflag &= ~(ECHO);
		if (tcsetattr(0, 0, &s_termios) == -1)
			return (-1);
		write(1, "megashell> ", 11);
		while (ret > 0)
			ret = read_input(&buffer, &begin_list);
		if (tcsetattr(0, 0, &s_termios_backup) == -1)
			return (-1);
	}
	return (0);
}

void	init_begin_list(t_command **begin_list)
{
	t_command	*l_begin;

	*begin_list = malloc(sizeof(t_command));
	if (begin_list == NULL)
	{
		printf("minishell: malloc error\n");
		exit(0);
	}
	l_begin = *begin_list;
	l_begin->next = NULL;
	l_begin->prev = NULL;
	l_begin->command = NULL;
}

void	first_pwd(t_command *begin_list)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		error_free(NULL, begin_list, 1);
	g_myenv = add_env_var_value(g_myenv, "PWD", cwd);
	if (!g_myenv)
		error_free(NULL, begin_list, 1);
	free(cwd);
}

int	main(int argc, char **argv, char **env)
{
	char		*buffer;
	t_command	*begin_list;
	int			ret;

	(void)argc;
	(void)argv;
	init_begin_list(&begin_list);
	buffer = NULL;
	g_myenv = new_env(env);
	if (!g_myenv)
		return (1);
	if (!var_is_in_env(g_myenv, "PWD"))
		first_pwd(begin_list);
	if (!(shlvl()))
		error_free(buffer, begin_list, 1);
	ret = main2(buffer, begin_list);
	if (buffer)
		free(buffer);
	free_list(begin_list);
	return (ret);
}
