#include "execution.h"

int	overflow(char *str, size_t i)
{
	unsigned int	len;
	int				sign;

	sign = 1;
	len = 0;
	if (str[len] == '-' || str[len] == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str == '0')
		str++;
	while ('0' <= str[len] && str[len] <= '9')
		len++;
	if (len > 19)
		return (1);
	else if (len == 19)
		if ((sign == 1 && ft_strncmp(str + i, "9223372036854775807", len) > 0)
			|| (sign == -1
				&& ft_strncmp(str + i, "9223372036854775808", len) > 0))
			return (1);
	return (0);
}

int	is_a_number(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] == ' ' || (9 <= str[i] && str[i] <= 13))
		i++;
	if (overflow(str + i, i))
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while ('0' <= str[i] && str[i] <= '9')
		i++;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i])
		return (0);
	return (1);
}

int	builtin_exit(char **arg, int pid, int pipes)
{
	if ((pid != 0 && !pipes) || (pid == 0 && pipes))
	{
		if (*arg)
		{
			if (!is_a_number(*arg))
			{
				write(2, "minishell: exit: ", 17);
				write(2, *arg, ft_strlen(*arg));
				write(2, ": numeric argument required\n", 28);
				exit_status(255, pid, 0);
			}
			else
			{
				if (*(arg + 1))
					write(2, "minishell: exit: too many arguments\n", 36);
				if (!*(arg + 1))
					exit_status(ft_atoi(*arg), pid, 0);
			}
		}
		else
			exit_status(ft_atoi(mygetenv(myenv, "?")), pid, 0);
	}
	else if (pid == 0 && *arg && is_a_number(*arg) && *(arg + 1))
		exit_status(1, pid, 0);
	return (1);
}
