#include "msh.h"

int	ft_isprint(int c)
{
	if (c >= ' ' && c < 127)
		return (1);
	return (0);
}

void	ft_putstr(char *str)
{
	int	i;
	int	str_len;

	if (str == NULL)
		return ;
	i = 0;
	str_len = ft_strlen(str);
	write(1, str, str_len);
}

void	error_free(char *buffer, t_command *begin_list, int str)
{
	if (buffer)
		free(buffer);
	free_list(begin_list);
	if (myenv)
		free_strtab(myenv);
	if (str)
		ft_putstr("minishell: fatal error\n");
	else
		ft_putstr("exit\n");
	exit(0);
}
