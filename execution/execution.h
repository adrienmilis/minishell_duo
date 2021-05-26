#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

char	**myenv;

typedef struct			s_pipe_cmd
{
	char				**cmd;
	char				*input;
	char				*output;
	int					double_greater; // booleen pour >>
	struct s_pipe_cmd	*next;
}						t_pipe_cmd;

/*typedef struct			s_command
{
	t_pipe_cmd			*pipe_cmd_start; // début de la liste chainee de pipe cmds
	struct s_command	*next;
}						t_command;*/

void	exec_pipe_cmd(t_pipe_cmd *pipe_cmd);

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
int		valid_env_name(char *var);
int		var_is_in_env(char **env, char *var);
int		var_has_value(char *var);
int		modif_env_var(char **env, char *var);
int		modif_env_var_value(char **env, char *varname, char *varvalue);
char	*mygetenv(char **env, char *var);
char		*itoa_env_var(int n);

char		**ft_split(const char *str, char c);

size_t	ft_strlen(const char *str);
char	*ft_strjoin(char const *s1, char const *s2, char c);
char	*ft_strdup(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_atoi(const char *nptr);

#endif
