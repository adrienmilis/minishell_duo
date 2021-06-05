#include "msh.h"

void	c_option(char *argv2)	// remove
{
	t_pipe_cmd	*pipe_cmd;
	int			backslash;

	backslash = 1;
	pipe_cmd = parser(argv2, 1, NULL);
	while (pipe_cmd)
	{
		if (pipe_cmd->cmd || pipe_cmd->input)
			exec_pipe_cmd(pipe_cmd, &backslash);
		free_pipe_cmd(pipe_cmd);
		pipe_cmd = parser(argv2, 0, NULL);
	}
	free_pipe_cmd(pipe_cmd);
	int ret = ft_atoi(mygetenv(myenv, "?"));
	free_strtab(myenv);
	exit(ret);
}

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
		*b = elem->command;
		ft_putstr(elem->command);
	}
	else
		*b = NULL;
}

int	main2(char *buffer, t_command *begin_list, int c, char *argv2)
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
		if (!c)
			write(1, "megashell> ", 11);
		while (ret > 0)
			ret = read_input(&buffer, &begin_list, c, argv2);
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
	myenv = add_env_var_value(myenv, "PWD", cwd);
	if (!myenv)
		error_free(NULL, begin_list, 1);
	free(cwd);
}

int	main(int argc, char **argv, char **env)
{
	char		*buffer;
	t_command	*begin_list;
	int			c;
	int			ret;

	c = 0;	// remove
	if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'c')	// remove
		c = 1;	// remove
	init_begin_list(&begin_list);
	buffer = NULL;
	myenv = new_env(env);
	if (!myenv)
		return (1);
	if (!var_is_in_env(myenv, "PWD"))
		first_pwd(begin_list);
	if (!(shlvl()))
		error_free(buffer, begin_list, 1);
	ret = main2(buffer, begin_list, c, argv[2]);
	if (buffer)
		free(buffer);
	free_list(begin_list);
	return (ret);
}
