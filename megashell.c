#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <curses.h>
#include <term.h>
#include <string.h>
#include "execution/execution.h"

t_pipe_cmd	*parser(char *cmd, int mew_command);

char	**myenv;

int	ft_putchar(int c);
/*int	ft_putchar(int c)
{
	write(1, &c, 1);
	return (1);
}*/

void	printenv(char **env)
{
	int i = 0;
	while (env[i])
		printf("%s\n", env[i++]);
}

void	exec_pipe_cmd(t_pipe_cmd *pipe_cmd);

int	input(char *buffer, char **path, char **env, int c, char *argv2)
{
	(void)path;
	(void)env;
	char	buf[3];
	int		ret;
	t_pipe_cmd	*pipe_cmd;

	if (c)
	{	
		//printf("1\n");
		pipe_cmd = parser(argv2, 1);
		//printf("2\n");
		while (pipe_cmd && pipe_cmd->cmd)
		{
		//printf("lol\n");
			exec_pipe_cmd(pipe_cmd);
		//printf("3\n");
			pipe_cmd = parser(argv2, 0);
		//printf("4\n");
		}
		exit(0);
	}
	ret = read(0, &buf, 3);
	if (ret == -1)
		return (ret);
	if (isprint(buf[0]))
	{
		write(1, &buf[0], 1);
		buffer[ft_strlen(buffer) + 1] = '\0';
		buffer[ft_strlen(buffer)] = buf[0];
	}
	else if (buf[0] == '\033')
	{
		if (buf[2] == 65)
			/*printf("UP ARROW\n")*/;
		if (buf[2] == 66)
			/*printf("DOWN ARROW\n")*/;
		if (buf[2] == 67)
			/*tputs(tgetstr("nd", NULL), 1, ft_putchar)*/;
		if (buf[2] == 68)
			/*tputs(tgetstr("le", NULL), 1, ft_putchar)*/;
	}
	else if (buf[0] == 127)
	{
		if (ft_strlen(buffer) > 0)
		{
			buffer[ft_strlen(buffer) - 1] = '\0';
			tputs(tgetstr("le", NULL), 1, ft_putchar);
			tputs(tgetstr("dc", NULL), 1, ft_putchar);
		}
	}
	else if (buf[0] == 10)
	{
		write(1, "\n", 1);
		//printf("1\n");
		pipe_cmd = parser(buffer, 1);
		//printf("2\n");
		while (pipe_cmd && pipe_cmd->cmd)
		{
			exec_pipe_cmd(pipe_cmd);
		//printf("3\n");
			pipe_cmd = parser(buffer, 0);
		}
		buffer[0] = '\0';
		write(1, "megashell> ", 11);
	}
	else
		printf("Other %d character!\n", buf[0]);
	return (ret);
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

char	**new_env(char **env);

int	main(int argc, char **argv, char **env)
{
	char	*buffer = NULL;
	size_t	buf_size = 2048;
	char **path = ft_split(getenv("PATH"), ':');

	myenv = new_env(env);
	if (!var_is_in_env(myenv, "PWD"))
		myenv = add_env_var_value(myenv, "PWD", getcwd(NULL, 0));
	if (!var_is_in_env(myenv, "OLDPWD"))
		myenv = add_env_var_value(myenv, "OLDPWD", "");

	buffer = (char *)calloc(sizeof(char), buf_size);
	if (buffer == NULL) {
		perror("Malloc failure");
		return (EXIT_FAILURE);
	}
	buffer[0] = '\0';

	struct termios s_termios;
	struct termios s_termios_backup;
	int ret = 1;

	(void)env;

	if (init_termcap() == 0) /* Fonction que vous aurez créé avec un tgetent dedans. */
	{
		if (tcgetattr(0, &s_termios) == -1)
			return (-1);

		if (tcgetattr(0, &s_termios_backup) == -1)
			return (-1);

		s_termios.c_lflag &= ~(ICANON); /* Met le terminal en mode non canonique. La fonction read recevra les entrées clavier en direct sans attendre qu'on appuie sur Enter */
		s_termios.c_lflag &= ~(ECHO); /* Les touches tapées au clavier ne s'affficheront plus dans le terminal */

		if (tcsetattr(0, 0, &s_termios) == -1)
			return (-1);

		//write(1, "megashell> ", 11);
		while (ret > 0)
			ret = input(buffer, path, env, (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'c'), argv[2]);

		if (tcsetattr(0, 0, &s_termios_backup) == -1)
			return (-1);
	}	

		printf("Bye \n");
	free(buffer);
}
