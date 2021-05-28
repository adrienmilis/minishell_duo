#include "msh.h"

/* LEAKS ENCORE A GERER */

void	updown_event(int reset, char rd[3], t_command *begin_list, char **buffer)
{
	static t_command	*elem;
	char				*tmp;

	if (reset == 1)
		elem = NULL;
	if (begin_list->next == NULL)
		return ;
	if (rd[2] == 65) // up arrow
	{
		if (elem == NULL)
			elem = begin_list;
		else if (elem->next != NULL)
			elem = elem->next;
		while (ft_strlen(*buffer) > 0)
			del_char_buffer(buffer);
		if (elem->command)
		{
			tmp = *buffer;
			*buffer = elem->command;
			free(tmp);
			ft_putstr(elem->command);
		}
	}
	if (rd[2] == 66) // down arrow
	{
		if (elem == NULL || elem == begin_list)
			elem = NULL;
		else
			elem = elem->prev;
		while (ft_strlen(*buffer) > 0)
			del_char_buffer(buffer);
		if (elem)
		{
			tmp = *buffer;
			*buffer = elem->command;
			free(tmp);
			ft_putstr(elem->command);
		}
	}
}

int	enter_event(char **buffer, t_command **begin_list)
{
	t_pipe_cmd	*pipe_cmd;

	write(1, "\n", 1);
	pipe_cmd = parser(*buffer, 1);
	while (pipe_cmd && pipe_cmd->cmd)
	{
		exec_pipe_cmd(pipe_cmd);
		pipe_cmd = parser(*buffer, 0);
	}
	ft_lstadd_front(begin_list, new_elem_history(*buffer));
	free(*buffer);
	*buffer = NULL;
	write(1, "megashell> ", 11);
	return (1);
}

int	read_input(char **buffer, t_command **begin_list, int c, char *argv2)
{
	char		rd[4];
	int			ret;
	static int	reset;

	if (c)
		c_option(argv2);
	
	ret = read(0, &rd, 4);
	if (ret == -1)
		exit(ret);
	if (ft_isprint(rd[0]))
	{
		write(1, &rd[0], 1);
		*buffer = make_buffer(*buffer, rd[0]);
		if (*buffer == NULL)
			printf("RAJOUTER ERREUR");
	}
	else if (rd[0] == '\033')
	{
		updown_event(reset, rd, *begin_list, buffer);
		reset = 0;
	}
	else if (rd[0] == 127)
	{
		if (ft_strlen(*buffer) > 0)
			del_char_buffer(buffer);
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
		//write(1, "megashell> ", 11);
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
		exit(0);	// exit with free and/or msg ?
	l_begin = *begin_list;
	l_begin->next = NULL;
	l_begin->prev = NULL;
	l_begin->command = NULL;
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
		myenv = add_env_var_value(myenv, "PWD", getcwd(NULL, 0));
	shlvl();
	ret = main2(buffer, begin_list, c, argv[2]);
	if (buffer)
		free(buffer);
	free_list(begin_list);
	return (ret);
}
