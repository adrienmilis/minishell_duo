#include "msh.h"

int	ctrlC(int new_value)
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

void	termcaps_del(void)
{
	tputs(tgetstr("le", NULL), 1, ft_putchar);
	tputs(tgetstr("dc", NULL), 1, ft_putchar);
}
