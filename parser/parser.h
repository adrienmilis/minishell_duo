#ifndef PARSER_H
# define PARSER_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>

char	**myenv;

typedef struct s_pipe_cmd
{
	char				**cmd;
	char				*input;
	char				*output;
	int					double_greater;		// booleen pour >>
	struct s_pipe_cmd	*next;
}				t_pipe_cmd;

typedef struct s_pars
{
	int in_s_quotes;
	int	in_d_quotes;
	int	semicolon;
	int	i;
	int	prev_var_w_space;
	int	word_from_variable;
	int	var_not_exist;
}				t_pars;

// errors.c
void	free_pipe_elems(t_pipe_cmd *c);
void	free_pipe_cmd(t_pipe_cmd *begin_list);
void	error_exit(char *msg, t_pipe_cmd *begin_list);

// parse_out_quotes.c
char 	*get_redir_word(char *cmd, t_pars *p, t_pipe_cmd *p_begin);
int		real_sign2(t_pars *p, t_pipe_cmd *p_cmd_start, char *cmd, char sign);
int		real_sign(t_pars *p, t_pipe_cmd *p_cmd_start, char *cmd);
int		reserved_chars(t_pars *p, t_pipe_cmd *p_cmd_start, char *cmd);
int		must_append(int i, char *cmd, t_pars *p);
int		out_quotes(t_pars *p, t_pipe_cmd *p_cmd_start, char *cmd);

// double_quotes.c
void	doubleq_special(t_pars *p, char *cmd, t_pipe_cmd *p_begin, char **buff);
char	*make_dquotes_arg(t_pars *p, char *cmd, t_pipe_cmd *p_begin);
char	*arg_double_quotes(t_pars *p, t_pipe_cmd *p_begin, char *cmd, int r);
void	in_double_quotes(t_pars *p, t_pipe_cmd *p_begin, char *cmd);


// parser.c
t_pipe_cmd	*parser(char *cmd, int new_command);

// add_arguments.c
void		add_argument2(char **new_args, int i, char *word, t_pipe_cmd *last);
int			append_arg(t_pipe_cmd *last, char *word, char *tmp);
void		add_argument(char *word, t_pipe_cmd *p_begin);
char		*copy_next_word(char *cmd, t_pars *p, int word_size, t_pipe_cmd *p_cmd_start);
char		*get_next_word(char *cmd, t_pars *p, t_pipe_cmd *p_cmd_start);

// utils.c
t_pipe_cmd	*init_pipe_list();
void		init_pars_struct(t_pars *p, int new_command, char c);
void		reset_pars_struct(t_pars *p);

// check_syntax.c
void 	set_quotes(int i, char *cmd, t_pars *p);
int		semicolons_valid(char *cmd);
int		pipes_valid(char *cmd);
int		are_quotes_closed(char *cmd);
int		check_syntax(char *cmd);
void	set_exit_status(char *error, int status);

// check_chars.c
int		is_space(char c);
int		is_r_space(char *c, int i);
int		is_r_resvd_char(char *c, int i, int dollar);
int		is_quote(char c);
int		is_r_quote(char *c, int i);

// check_chars2.c
int		is_reserved_char(char c, int dollar);
int		is_unesc_char(char *c, int i);
int		is_number(char c);
int		is_letter(char c);
int		valid_var_char(char c);

// check_chars3.c
int		is_unesc_simple(char *c, int i);
int		is_unesc_double(char *c, int i);
int		space_in_word(char	*word);
int	space_middle_of_word(char *word);

// linked_list.c
t_pipe_cmd	*ft_lstlast(t_pipe_cmd *begin_list);
void		ft_lstadd_back(t_pipe_cmd **begin_list, t_pipe_cmd *new_elem);
t_pipe_cmd	*new_elem();
void		print_list(t_pipe_cmd *begin_list);

// libft.c
char	*ft_strjoin_w_ns(char *s1, char *s2);
char	*ft_strdup_len(char *str, int len);
int		is_number(char c);
int		is_letter(char c);
int		valid_var_char(char c);
int		ft_strcmp(char *s1, char *s2);

// env.c
char	*get_env_var(int index);

// env_vars.c
char	*gnw_double_quotes(t_pars *p, char *cmd, t_pipe_cmd *p_begin);
char	*dup_dollar(t_pipe_cmd *p_begin);
char 	*unvalid_var_name(t_pars *p, char *cmd, t_pipe_cmd *p_begin);
char	*valid_var_name(t_pars *p, char *cmd, t_pipe_cmd *p_cmd_start);
char	*get_variable(t_pars *p, char *cmd, t_pipe_cmd *p_begin);

// simple_quotes.c
char	*arg_simple_quotes(t_pars *p, t_pipe_cmd *p_cmd_start, char *cmd, int r);
void	in_simple_quotes(t_pars *p, t_pipe_cmd *p_cmd_start, char *cmd);

// dupl_fcts.c
char			*ft_strdup(char *str);
int				ft_strlen(char *str);
int				strcmp_env(char *s1, char *s2);
// static size_t	strtablen(char **tab);
// static char		**free_strtab(char **tab);
char			**new_env(char **env);

// execution
char		*itoa_env_var(char *prefix, int n);


#endif