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

int	ft_putchar(int c)
{
	write(1, &c, 1);
	return (1);
}

void	print_strtab(char **env)
{
	int i = 0;
	while (env[i])
		printf("%s\n", env[i++]);
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
	//printf("HAHA\n");
	option_string = 1;
	if (pid == 0)
	{
		while (*arg)
		{
			if (option_string && !ft_strcmp("-", *arg))
				option_string = 0;
			else if (option_string && option_n(*arg))
				nonl = 1;
			else
			{
				option_string = 0;
				write(1, *arg, ft_strlen(*arg));
				if (*(arg + 1))
					write(1, " ", 1);
			}
			arg++;
		}
		if (!nonl)
			write(1, "\n", 1);
	}
	return (1);
}

void	cd_oldpwd()
{
	if (!var_is_in_env(myenv, "OLDPWD"))
		myenv = add_env_var_value(myenv, "OLDPWD", mygetenv(myenv, "PWD"));
	else
		modif_env_var_value(myenv, "OLDPWD", mygetenv(myenv, "PWD"));
}

void	cd_pwd()
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		exit(0); //quel exit status ?
	modif_env_var_value(myenv, "PWD", cwd);
	free(cwd);
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
			{
				cd_oldpwd();
				cd_pwd();
			}
		}
		else if (pid == 0 && chdir(*arg) < 0)
			exit(1);
	}
	else
		cd_oldpwd();
	return (1);
}

int	builtin_pwd(int pid)
{
	char	*pwd;

	if (pid == 0)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			exit(0); //
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

int	builtin_export(char **arg, int pid, int pipes)
{
	char	**copy_myenv;
	size_t	i;
	size_t	env_var_len;

	if (pid == 0 && !*arg)
	{
		copy_myenv = copy_env(myenv + 1);
		if (!copy_myenv)
			return (1); //changer les return gerer les erreurs
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
	}
	else if ((pid != 0 && !pipes) || (pid == 0 && pipes))
	{
		while (*arg)
		{
			if (valid_env_name(*arg))
			{
				if (var_is_in_env(myenv, *arg))
				{
					if (var_has_value(*arg))
						modif_env_var(myenv, *arg);
				}
				else
					myenv = add_env_var(myenv, *arg);
			}
			else
			{
				write(2, "minishell: export: `", 20);
				write(2, *arg, ft_strlen(*arg));
				write(2, "\': not a valid identifier\n", 26);
			}
			arg++;
		}
	}
	else if (pid == 0)
		while (*(arg++))
			if (!valid_env_name(*(arg - 1)))
				exit(1);
	return (1);
}

int	builtin_unset(char **arg, int pid, int pipes)
{
	if ((pid != 0 && !pipes) || (pid == 0 && pipes))
	{
		while (*arg)
		{
			if (valid_env_name(*arg))
			{
				if (var_is_in_env(myenv, *arg))
					myenv = rm_env_var(myenv, *arg);
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
			if (!valid_env_name(*(arg - 1)))
				exit(1);
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
				|| (sign == -1 && ft_strncmp(str + i, "9223372036854775808", len) > 0))
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
				exit(255);
			}
			else
			{
				if (*(arg + 1))
					write(2, "minishell: exit: too many arguments\n", 36);
				if (!*(arg + 1))
					exit(ft_atoi(*arg));
			}
		}
		else
			exit(ft_atoi(mygetenv(myenv, "?")));
	}
	else if (pid == 0 && *arg && is_a_number(*arg) && *(arg + 1))
		exit(1);
	return (1);
}

int	do_builtin(char **cmd, int pid, int pipes) // faire un strcmp qui decaps
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

int	no_permission(char *cmd)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": Permission denied\n", 20);
	exit(126);
}

void	stat_check(char *cmd)
{
	struct stat	buf;

	stat(cmd, &buf);
	if (S_ISDIR(buf.st_mode) && ft_strchr(cmd, '/'))
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": is a directory\n", 17);
		exit(126);
	}
	else if (S_ISREG(buf.st_mode) && ft_strchr(cmd, '/'))
	{
		if (!(buf.st_mode & S_IRUSR) || !(buf.st_mode & S_IXUSR))
			no_permission(cmd);
		else
			exit(0);
	}
}

void	launch_executable(char **cmd)
{
	int		i;
	char	*cmd0;
	char	*PATH_value;
	char	**PATH_split;

	if (!cmd[0])
		return ;
	PATH_value = mygetenv(myenv, "PATH");
	PATH_split = NULL;
	if (PATH_value)
	{
		PATH_split = ft_split(PATH_value, ':');
		if (!PATH_split)
			return ;
	}
	execve(cmd[0], cmd, myenv + 1);
	if (errno == EACCES && ft_strchr(cmd[0], '/'))
		stat_check(cmd[0]);
	cmd0 = cmd[0];
	if (PATH_value && PATH_split[0])
		cmd[0] = ft_strjoin(PATH_split[0], cmd0, '/');
	i = 1;
	if (PATH_value)
	{
		while (execve(cmd[0], cmd, myenv + 1) == -1 && PATH_split[i])
		{
			if (errno == EACCES && ft_strchr(cmd0, '/'))
				no_permission(cmd0);
			free(cmd[0]);
			cmd[0] = ft_strjoin(PATH_split[i++], cmd0, '/');
		}
		if (errno == EACCES && ft_strchr(cmd[0], '/'))
			no_permission(cmd[0]);
	}
	stat_check(cmd0);
	write(2, "minishell: ", 11);
	write(2, cmd0, ft_strlen(cmd0));
	if (!ft_strchr(cmd0, '/'))
		write(2, ": command not found\n", 20);
	else
		write(2, ": No such file or directory\n", 28);
	exit(127);
}

void	exec_pipe_cmd(t_pipe_cmd *pipe_cmd)
{
	int	pipefd[2][2];
	int	filefd[2];
	int	pid;
	int	firstcmd;
	int	pipes;
	int	status;

	firstcmd = 1;
	pipes = 0;
	if (pipe_cmd->next)
		pipes = 1;
	while (pipe_cmd)
	{
		//printf("----\n%s\n----\n", pipe_cmd->cmd[0]);
		pipefd[0][0] = pipefd[1][0];
		pipefd[0][1] = pipefd[1][1];
		pipe(pipefd[1]);
		pid = fork();
		if (pid == 0)
		{
			if (!firstcmd && !pipe_cmd->input)
				dup2(pipefd[0][0], STDIN_FILENO);
			else if (pipe_cmd->input)
			{
				filefd[0] = open(pipe_cmd->input, O_RDONLY);
				dup2(filefd[0], STDIN_FILENO);
				close(filefd[0]);
			}
			if (pipe_cmd->next && !pipe_cmd->output)
				dup2(pipefd[1][1], STDOUT_FILENO);
			else if (pipe_cmd->output)
			{
				if (!pipe_cmd->double_greater)
					filefd[1] = open(pipe_cmd->output, O_WRONLY | O_TRUNC);
				else
					filefd[1] = open(pipe_cmd->output, O_WRONLY | O_APPEND);
				dup2(filefd[1], STDOUT_FILENO);
				close(filefd[1]);
			}
			close(pipefd[1][0]);
			close(pipefd[1][1]);
		}
		if (!firstcmd)
		{
			close(pipefd[0][0]);
			close(pipefd[0][1]);
		}
		if (is_builtin(pipe_cmd->cmd))
			do_builtin(pipe_cmd->cmd, pid, pipes);
		else if (pid == 0)
			launch_executable(pipe_cmd->cmd);
		if (pid == 0)
			exit(0); // pid == 0 exit(0); ici ou dans les builtin ?
		waitpid(pid, &status, 0);
		pipe_cmd = pipe_cmd->next;
		firstcmd = 0;
	}
	if (WIFEXITED(status))
	{
		status = WEXITSTATUS(status);
		free(myenv[0]);
		myenv[0] = itoa_env_var("?=", status);
	}
	close(pipefd[1][0]);
	close(pipefd[1][1]);
}
