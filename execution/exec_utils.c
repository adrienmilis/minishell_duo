#include "execution.h"

int	exit_status(int new_status, int pid, int check_main)
{
	static int	status;
	static int	main_process;
	int			ret;

	if (check_main)
		return (main_process);
	if (pid)
		main_process = 1;
	else
		main_process = 0;
	ret = status;
	status = new_status;
	return (ret);
}

int	ctrl_execution(int new_value)
{
	static int	b;
	int			ret;

	ret = b;
	if (new_value != 1)
		b = new_value;
	return (ret);
}

void	handler_signal_execution(int sig)
{
	if (sig == SIGINT)
		ctrl_execution(130);
	else if (sig == SIGQUIT)
	{
		ctrl_execution(131);
		write(1, "Quit: 3", 7);
	}
}

void	canonical_mode(int set)
{
	struct termios	s_termios;

	if (tcgetattr(0, &s_termios) == -1)
		return ;
	if (set)
	{
		s_termios.c_lflag |= ICANON;
		s_termios.c_lflag |= ECHO;
	}
	else
	{
		s_termios.c_lflag &= ~(ICANON);
		s_termios.c_lflag &= ~(ECHO);
	}
	if (tcsetattr(0, 0, &s_termios) == -1)
		return ;
}
