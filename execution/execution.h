#ifndef EXECUTION_H
# define EXECUTION_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

char	**g_myenv;

typedef struct s_pipe_cmd
{
	char				**cmd;
	char				*input;
	char				*output;
	int					double_greater;
	struct s_pipe_cmd	*next;
}						t_pipe_cmd;

typedef struct s_command
{
	char				*command;
	struct s_command	*prev;
	struct s_command	*next;
}				t_command;

typedef struct s_PATH
{
	char	*value;
	char	**split;
}				t_PATH;

typedef struct s_exec_data
{
	int			firstcmd;
	int			pipes;
	int			pid;
	int			status;
}				t_exec_data;

int		exec_pipe_cmd(t_pipe_cmd *pipe_cmd, int *backslash);

int		free_exit_exec(int status, t_pipe_cmd *pipe_cmd, t_PATH *PATH);
void	error_input(char *input, t_pipe_cmd *begin, int pipefd[2][2], \
	t_exec_data d);
int		error_exec(t_pipe_cmd *beg, int pipefd[2][2], t_exec_data d);

int		exit_status(int new_status, int pid, int check_main);
int		ctrl_execution(int new_value);
void	handler_signal_execution(int sig);
void	canonical_mode(int set);
void	dup2_and_close(int fd1, int fd2);

int		launch_executable(char **cmd, t_pipe_cmd *begin);

int		do_builtin(char **cmd, int pid, int pipes);
int		is_builtin(char **cmd);

int		builtin_echo(char **arg, int pid);
int		builtin_cd(char **arg, int pid, int pipes);
int		builtin_export(char **arg, int pid, int pipes);
int		builtin_pwd(int pid);
int		builtin_unset(char **arg, int pid, int pipes);
int		builtin_env(int pid);
int		builtin_exit(char **arg, int pid, int pipes);

size_t	strlen_env_var(char *var);
int		strcmp_env(const char *s1, const char *s2);
size_t	strtablen(char **strtab);
char	**free_strtab(char **strtab);
char	**new_env(char **env);
char	**copy_env(char **env);
void	sort_env(char **env);
char	**add_env_var(char **env, char *var);
char	**add_env_var_value(char **env, char *varname, char *varvalue);
char	**rm_env_var(char **env, char *var);
int		valid_env_name(char *var, int unset);
int		var_is_in_env(char **env, char *var);
int		var_has_value(char *var);
int		modif_env_var(char **env, char *var);
int		modif_env_var_value(char **env, char *varname, char *varvalue);
char	*mygetenv(char **env, char *var);
char	*itoa_env_var(char *prefix, int n);

char	**ft_split(const char *str, char c);

size_t	ft_strlen(const char *str);
char	*ft_strjoin(char const *s1, char const *s2, char c);
char	*ft_strdup(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_atoi(const char *nptr);
char	*ft_strchr(const char *s, int c);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	ft_putnbr_fd(int n, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memset(void *s, int c, size_t n);

void	free_pipe_cmd(t_pipe_cmd *begin_list);
void	free_list(t_command	*begin_list);

#endif
