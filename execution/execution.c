#include "execution.h"
#include <stdio.h>
/*#include <stdlib.h>
#include <unistd.h>
#include "../ft_split.c"
#include "../ft_strjoin.c"
#include <termios.h>
#include <ctype.h>
#include <curses.h>
#include <term.h>
#include <string.h>
#include <fcntl.h>*/

int	free_exit_exec(int status, t_pipe_cmd *pipe_cmd, t_PATH *PATH)
{
	if (myenv)
		free_strtab(myenv);
	free_pipe_cmd(pipe_cmd);
	if (PATH && PATH->split)
		free_strtab(PATH->split);
	exit(status);
}

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

int	option_n(char *arg)
{
	size_t	i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
		if (arg[i++] != 'n')
			return (0);
	return (1);
}

int	builtin_echo(char **arg, int pid)
{
	int	nonl;
	int	option_string;

	nonl = 0;
	option_string = 1;
	if (pid == 0)
	{
		while (*(arg++))
		{
			if (option_string && !ft_strcmp("-", *(arg - 1)))
				option_string = 0;
			else if (option_string && option_n(*(arg - 1)))
				nonl = 1;
			else
			{
				option_string = 0;
				write(1, *(arg - 1), ft_strlen(*(arg - 1)));
				if (*arg)
					write(1, " ", 1);
			}
		}
		if (!nonl)
			write(1, "\n", 1);
	}
	return (1);
}

int	cd_oldpwd(void)
{
	if (!var_is_in_env(myenv, "OLDPWD"))
	{
		myenv = add_env_var_value(myenv, "OLDPWD", mygetenv(myenv, "PWD"));
		if (!myenv)
			return (0);
	}
	else
		if (!modif_env_var_value(myenv, "OLDPWD", mygetenv(myenv, "PWD")))
			return (0);
	return (1);
}

int	cd_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	modif_env_var_value(myenv, "PWD", cwd);
	free(cwd);
	return (1);
}

int	builtin_cd(char **arg, int pid, int pipes)
{
	if (*arg && arg[0][0] != '\0')
	{
		if ((pid != 0 && !pipes) || (pid == 0 && pipes))
		{
			if (chdir(*arg) < 0)
			{
				write(2, "minishell: cd: ", 15);
				write(2, *arg, ft_strlen(*arg));
				if (errno == ENOENT)
					write(2, ": No such file or directory\n", 28);
				if (errno == EACCES)
					write(2, ": Permission denied\n", 20);
			}
			else
				if (!cd_oldpwd() || !cd_pwd())
					return (0);
		}
		else if (pid == 0 && chdir(*arg) < 0)
			exit_status(1, pid, 0);
	}
	else
		if (!cd_oldpwd())
			return (0);
	return (1);
}

int	builtin_pwd(int pid)
{
	char	*pwd;

	if (pid == 0)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			return (0);
		write(1, pwd, ft_strlen(pwd));
		write(1, "\n", 1);
		free(pwd);
	}
	return (1);
}

void	export_print_env(char *var)
{
	size_t	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '$' || var[i] == '\"' || var[i] == '\\')
			write(1, "\\", 1);
		write(1, var + i, 1);
		i++;
	}
}

int	builtin_export_no_arg(void)
{
	char	**copy_myenv;
	size_t	i;
	size_t	env_var_len;

	copy_myenv = copy_env(myenv + 1);
	if (!copy_myenv)
		return (0);
	sort_env(copy_myenv);
	i = 0;
	while (copy_myenv[i])
	{
		env_var_len = strlen_env_var(copy_myenv[i]);
		write(1, "declare -x ", 11);
		write(1, copy_myenv[i], env_var_len);
		if (var_has_value(copy_myenv[i]))
		{
			write(1, "=\"", 2);
			export_print_env(mygetenv(myenv, copy_myenv[i]));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		i++;
	}
	free_strtab(copy_myenv);
	return (1);
}

int	builtin_export_args(char **arg)
{
	while (*arg)
	{
		if (valid_env_name(*arg, 0))
		{
			if (var_is_in_env(myenv, *arg))
			{
				if (var_has_value(*arg) && !modif_env_var(myenv, *arg))
					return (0);
			}
			else
			{
				myenv = add_env_var(myenv, *arg);
				if (!myenv)
					return (0);
			}
		}
		else
		{
			write(2, "minishell: export: `", 20);
			write(2, *arg, ft_strlen(*arg));
			write(2, "\': not a valid identifier\n", 26);
		}
		arg++;
	}
	return (1);
}

int	builtin_export(char **arg, int pid, int pipes)
{
	if (pid == 0 && !*arg)
		return (builtin_export_no_arg());
	else if ((pid != 0 && !pipes) || (pid == 0 && pipes))
		return (builtin_export_args(arg));
	else if (pid == 0)
		while (*(arg++))
			if (!valid_env_name(*(arg - 1), 0))
				exit_status(1, pid, 0);
	return (1);
}

int	builtin_unset(char **arg, int pid, int pipes)
{
	if ((pid != 0 && !pipes) || (pid == 0 && pipes))
	{
		while (*arg)
		{
			if (valid_env_name(*arg, 1))
			{
				if (var_is_in_env(myenv, *arg))
				{
					myenv = rm_env_var(myenv, *arg);
					if (!myenv)
						return (0);
				}
			}
			else
			{
				write(2, "minishell: unset: `", 19);
				write(2, *arg, ft_strlen(*arg));
				write(2, "\': not a valid identifier\n", 26);
			}
			arg++;
		}
	}
	else if (pid == 0)
		while (*(arg++))
			if (!valid_env_name(*(arg - 1), 1))
				exit_status(1, pid, 0);
	return (1);
}

int	builtin_env(int pid)
{
	size_t	i;

	if (pid == 0)
	{
		i = 1;
		while (myenv[i])
		{
			if (var_has_value(myenv[i]))
			{
				write(1, myenv[i], ft_strlen(myenv[i]));
				write(1, "\n", 1);
			}
			i++;
		}
	}
	return (1);
}

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

int	do_builtin(char **cmd, int pid, int pipes)
{
	if (!ft_strcmp("echo", cmd[0]))
		return (builtin_echo(cmd + 1, pid));
	else if (!ft_strcmp("cd", cmd[0]))
		return (builtin_cd(cmd + 1, pid, pipes));
	else if (!ft_strcmp("pwd", cmd[0]))
		return (builtin_pwd(pid));
	else if (!ft_strcmp("export", cmd[0]))
		return (builtin_export(cmd + 1, pid, pipes));
	else if (!ft_strcmp("unset", cmd[0]))
		return (builtin_unset(cmd + 1, pid, pipes));
	else if (!ft_strcmp("env", cmd[0]))
		return (builtin_env(pid));
	else if (!ft_strcmp("exit", cmd[0]))
		return (builtin_exit(cmd + 1, pid, pipes));
	else
		return (0);
}

int	is_builtin(char **cmd)
{
	if (!ft_strcmp("echo", cmd[0]))
		return (1);
	else if (!ft_strcmp("cd", cmd[0]))
		return (1);
	else if (!ft_strcmp("pwd", cmd[0]))
		return (1);
	else if (!ft_strcmp("export", cmd[0]))
		return (1);
	else if (!ft_strcmp("unset", cmd[0]))
		return (1);
	else if (!ft_strcmp("env", cmd[0]))
		return (1);
	else if (!ft_strcmp("exit", cmd[0]))
		return (1);
	else
		return (0);
}

void	no_permission(char *cmd, t_pipe_cmd *begin, t_PATH *PATH)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": Permission denied\n", 20);
	free_exit_exec(126, begin, PATH);
}

void	stat_check(char *cmd, t_pipe_cmd *begin, t_PATH *PATH)
{
	struct stat	buf;

	ft_memset(&buf, 0, sizeof(struct stat));
	stat(cmd, &buf);
	if (S_ISDIR(buf.st_mode) && ft_strchr(cmd, '/'))
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": is a directory\n", 17);
		free_exit_exec(126, begin, PATH);
	}
	else if (S_ISREG(buf.st_mode) && ft_strchr(cmd, '/'))
	{
		if (!(buf.st_mode & S_IRUSR) || !(buf.st_mode & S_IXUSR))
			no_permission(cmd, begin, PATH);
		else
			free_exit_exec(0, begin, PATH);
	}
}

int	launch_executable(char **cmd, t_pipe_cmd *begin)
{
	int		i;
	char	*cmd0;
	t_PATH	PATH;

	if (!cmd[0])
		return (0);
	PATH.value = mygetenv(myenv, "PATH");
	PATH.split = NULL;
	if (PATH.value && PATH.value[0])
	{
		PATH.split = ft_split(PATH.value, ':');
		if (!PATH.split)
		{
			free(PATH.value);
			return (0);
		}
	}
	if (ft_strchr(cmd[0], '/') || !(PATH.value && PATH.value[0]))
		execve(cmd[0], cmd, myenv + 1);
	if (errno == EACCES && ft_strchr(cmd[0], '/'))
		stat_check(cmd[0], begin, &PATH);
	cmd0 = cmd[0];
	if (PATH.value && PATH.value[0] && PATH.split[0])
	{
		cmd[0] = ft_strjoin(PATH.split[0], cmd0, '/');
		if (!cmd[0])
			return (0);
	}
	i = 1;
	if (PATH.value && PATH.value[0])
	{
		while (execve(cmd[0], cmd, myenv + 1) == -1 && PATH.split[i])
		{
			if (errno == EACCES && ft_strchr(cmd0, '/'))
				no_permission(cmd0, begin, &PATH);
			free(cmd[0]);
			cmd[0] = ft_strjoin(PATH.split[i++], cmd0, '/');
			if (!cmd[0])
				return (0);
		}
		if (errno == EACCES && ft_strchr(cmd[0], '/'))
			no_permission(cmd[0], begin, &PATH);
	}
	stat_check(cmd0, begin, &PATH);
	write(2, "minishell: ", 11);
	write(2, cmd0, ft_strlen(cmd0));
	if (!ft_strchr(cmd0, '/') && PATH.value && PATH.value[0])
		write(2, ": command not found\n", 20);
	else
		write(2, ": No such file or directory\n", 28);
	free_exit_exec(127, begin, &PATH);
	return (1);
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

void	error_input(char *input, t_pipe_cmd *begin, int pipefd[2][2], int *fst)
{
	write(2, "minishell: ", 11);
	write(2, input, ft_strlen(input));
	if (errno == EACCES)
		write(2, ": Permission denied\n", 20);
	else if (errno == ENOENT)
		write(2, ": No such file or directory\n", 28);
	if (!fst[0])
	{
		close(pipefd[0][0]);
		close(pipefd[0][1]);
	}
	close(pipefd[1][0]);
	close(pipefd[1][1]);
	free_exit_exec(1, begin, NULL);
}

int	error_exec(t_pipe_cmd *beg, int pipefd[2][2], int *firstcmd_pipes, int pid)
{
	if (!firstcmd_pipes[0] && pid < 0)
	{
		close(pipefd[0][0]);
		close(pipefd[0][1]);
	}
	if (pid == -1)
	{
		close(pipefd[1][0]);
		close(pipefd[1][1]);
	}
	else if (pid == 0)
		free_exit_exec(0, beg, NULL);
	return (0);
}

int	exec_pipe_cmd(t_pipe_cmd *pipe_cmd, int *backslash)
{
	t_pipe_cmd	*begin;
	int			fd[3][2];
	int			firstcmd_pipes[2];
	int			pid;
	int			status;

	begin = pipe_cmd;
	*backslash = 0;
	firstcmd_pipes[0] = 1;
	firstcmd_pipes[1] = 0;
	if (pipe_cmd->next)
		firstcmd_pipes[1] = 1;
	canonical_mode(1);
	while (pipe_cmd && !ctrl_execution(1) && (pipe_cmd->cmd || pipe_cmd->input))
	{
		fd[0][0] = fd[1][0];
		fd[0][1] = fd[1][1];
		if (pipe(fd[1]) == -1)
			return (error_exec(begin, fd, firstcmd_pipes, -2));
		signal(SIGINT, &handler_signal_execution);
		signal(SIGQUIT, &handler_signal_execution);
		pid = fork();
		if (pid == -1)
			return (error_exec(begin, fd, firstcmd_pipes, pid));
		if (pid == 0)
		{
			if (!firstcmd_pipes[0] && !pipe_cmd->input)
				dup2(fd[0][0], STDIN_FILENO);
			else if (pipe_cmd->input)
			{
				fd[2][0] = open(pipe_cmd->input, O_RDONLY);
				if (fd[2][0] == -1)
					error_input(pipe_cmd->input, begin, fd, firstcmd_pipes);
				dup2(fd[2][0], STDIN_FILENO);
				close(fd[2][0]);
			}
			if (pipe_cmd->next && !pipe_cmd->output)
				dup2(fd[1][1], STDOUT_FILENO);
			else if (pipe_cmd->output)
			{
				if (!pipe_cmd->double_greater)
					fd[2][1] = open(pipe_cmd->output, O_WRONLY | O_TRUNC);
				else
					fd[2][1] = open(pipe_cmd->output, O_WRONLY | O_APPEND);
				dup2(fd[2][1], STDOUT_FILENO);
				close(fd[2][1]);
			}
			close(fd[1][0]);
			close(fd[1][1]);
		}
		if (!firstcmd_pipes[0])
		{
			close(fd[0][0]);
			close(fd[0][1]);
		}
		if (pipe_cmd->cmd)
		{
			if (is_builtin(pipe_cmd->cmd))
			{
				if (!do_builtin(pipe_cmd->cmd, pid, firstcmd_pipes[1]))
					return (error_exec(begin, fd, firstcmd_pipes, pid));
			}
			else if (pid == 0)
				if (!launch_executable(pipe_cmd->cmd, begin))
					return (error_exec(begin, fd, firstcmd_pipes, pid));
		}
		if (pid == 0)
			free_exit_exec(exit_status(0, pid, 0), begin, NULL);
		waitpid(pid, &status, 0);
		if (exit_status(0, 0, 1))
			free_exit_exec(exit_status(0, pid, 0), begin, NULL);
		pipe_cmd = pipe_cmd->next;
		firstcmd_pipes[0] = 0;
	}
	if (WIFEXITED(status))
	{
		status = WEXITSTATUS(status);
		free(myenv[0]);
		myenv[0] = itoa_env_var("?=", status);
		if (myenv[0])
			return (0);
	}
	if (ctrl_execution(1))
	{
		*backslash = 1;
		free(myenv[0]);
		myenv[0] = itoa_env_var("?=", ctrl_execution(0));
		if (myenv[0])
			return (0);
	}
	close(fd[1][0]);
	close(fd[1][1]);
	canonical_mode(0);
	return (1);
}
