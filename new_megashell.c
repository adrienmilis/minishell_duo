#include "msh.h"

int		ctrlC(int new_value)
{
	static int	b;
	int			ret;

	ret = b;
	b = new_value;
	return (ret);
}

void	handler_sigint(int sig)
{
	(void)sig;
	write(1, "\nmegashell> ", 12);
	ctrlC(1);
}

void	nothing_sigquit(int sig)
{
	(void)sig;
	return ;
}

void	c_option(char *argv2)
{
	t_pipe_cmd	*pipe_cmd;
	int			backslash;

	backslash = 1;
	pipe_cmd = parser(argv2, 1);
	while (pipe_cmd)
	{
		if (pipe_cmd->cmd || pipe_cmd->input)
			exec_pipe_cmd(pipe_cmd, &backslash);
		free_pipe_cmd(pipe_cmd);
		pipe_cmd = parser(argv2, 0);
	}
	free_pipe_cmd(pipe_cmd);
	int ret = ft_atoi(mygetenv(myenv, "?"));
	free_strtab(myenv);
	exit(ret);
}

void	func(void)
{
	tputs(tgetstr("le", NULL), 1, ft_putchar);
	tputs(tgetstr("dc", NULL), 1, ft_putchar);
}

void	write_history_command(t_command *elem, char **buffer, int up, t_command *beg_list)
{
	int len;
	
	len = ft_strlen(*buffer);
	while (len > 0)
	{
		func();
		len--;
	}
	if (!pointer_in_history(*buffer, beg_list) && *buffer)
	{
		free(*buffer);
		*buffer = NULL;
	}
	if ((!up && elem) || (up && elem->command))
	{
		*buffer = elem->command;
		ft_putstr(elem->command);
	}
	else
		*buffer = NULL;
}

void	updown_event(int *rst, char rd[3], t_command *beg_list, char **buffer)
{
	static t_command	*elem;

	if (*rst == 1)
		elem = NULL;
	*rst = 0;
	if (beg_list->next == NULL)
		return ;
	if (rd[2] == 65) // up arrow
	{
		if (elem == NULL)
			elem = beg_list;
		else if (elem->next != NULL)
			elem = elem->next;
		write_history_command(elem, buffer, 1, beg_list);
	}
	if (rd[2] == 66) // down arrow
	{
		if (elem == NULL || elem == beg_list)
			elem = NULL;
		else
			elem = elem->prev;
		write_history_command(elem, buffer, 0, beg_list);
	}
}

int	enter_event(char **buffer, t_command **begin_list)
{
	t_pipe_cmd	*pipe_cmd;
	t_command	*new_elem;
	int			backslash;

	backslash = 0;
	write(1, "\n", 1);
	pipe_cmd = parser(*buffer, 1);
	if (pipe_cmd == NULL)
	{
		write(1, "megashell> ", 11);
		free(*buffer);
		*buffer = NULL;
		return (1);
	}
	while (pipe_cmd)
	{
		if (pipe_cmd->cmd || pipe_cmd->input)
			exec_pipe_cmd(pipe_cmd, &backslash);	// free pipe_cmd on exit
		free_pipe_cmd(pipe_cmd);
		pipe_cmd = parser(*buffer, 0);
	}
	free_pipe_cmd(pipe_cmd);
	new_elem = new_elem_history(*buffer);
	if (!new_elem)
		error_free(*buffer, *begin_list);	// buffer if exists, t_command
	ft_lstadd_front(begin_list, new_elem);
	// free(*buffer);
	*buffer = NULL;
	if (backslash)
		write(1, "\n", 1);
	write(1, "megashell> ", 11);
	return (1);
}

int	read_input(char **buffer, t_command **begin_list, int c, char *argv2)	// plusieurs comportements pour free : 1er passage && suivants
{
	char		rd[4];
	int			ret;
	static int	reset;

	if (c)
		c_option(argv2);
	
	signal(SIGINT, &handler_sigint);
	signal(SIGQUIT, &nothing_sigquit);
	ret = read(0, &rd, 4);
	if (ret == -1)
		error_free(*buffer, *begin_list);	// free t_command and myenv (1st call) ; free t_command, myenv, buffer (2e call)
	if (ctrlC(0))
	{
		free(*buffer);
		*buffer = NULL;
	}
	if (ft_isprint(rd[0]))
	{
		write(1, &rd[0], 1);
		*buffer = make_buffer(*buffer, rd[0]);
		if (*buffer == NULL)
			error_free(*buffer, *begin_list);	// free t_command and myenv (old buffer is already freed)
	}
	else if (rd[0] == '\033')
		updown_event(&reset, rd, *begin_list, buffer);
	else if (rd[0] == 127)
	{
		if (ft_strlen(*buffer) > 0)
			del_char_buffer(buffer, *begin_list);
	}
	else if (rd[0] == 4 && ft_strlen(*buffer) == 0)
	{
		write(1, "exit\n", 5);
		//leaks ?
		exit(0);
	}
	else if (rd[0] == 10)
		reset = enter_event(buffer, begin_list);
	return (ret);
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
		error_free(NULL, begin_list);
	myenv = add_env_var_value(myenv, "PWD", cwd);
	if (!myenv)
		error_free(NULL, begin_list);
	free(cwd);
}

int	main(int argc, char **argv, char **env)
{
	char		*buffer;
	t_command	*begin_list;
	int			c;
	int			ret;

	c = 0;
	if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'c')
		c = 1;

	init_begin_list(&begin_list);
	buffer = NULL;
	myenv = new_env(env);
	if (!var_is_in_env(myenv, "PWD"))
		first_pwd(begin_list);
	if (!(shlvl()))
		error_free(buffer, begin_list);
	ret = main2(buffer, begin_list, c, argv[2]);	// on est ici
	if (buffer)
		free(buffer);
	free_list(begin_list);
	return (ret);
}
