#ifndef PARSER_H
# define PARSER_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>

char	**g_myenv;

typedef struct s_pipe_cmd
{
	char				**cmd;
	char				*input;
	char				*output;
	int					double_greater;
	struct s_pipe_cmd	*next;
}				t_pipe_cmd;

typedef struct s_pars
{
	int	in_s_quotes;
	int	in_d_quotes;
	int	semicolon;
	int	i;
	int	prev_var_w_space;
	int	word_from_variable;
	int	not_append;
	int	r;
}				t_pars;

typedef struct s_command
{
	char				*command;
	struct s_command	*prev;
	struct s_command	*next;
}				t_command;

typedef struct s_package
{
	char		*cmd;
	t_pipe_cmd	*p_begin;
	t_command	*b_list;
}				t_package;

void		ft_putstr(char *str);
void		error_free_pars(char *buffer, t_command *b_lst, t_pipe_cmd *p_beg);
void		free_list(t_command	*begin_list);
char		**free_strtab(char **strtab);
char		*mygetenv(char **env, char *var);


// errors.c
void		error_free_pars(char *buffer, t_command *b_lst, t_pipe_cmd *p_beg);
void		set_exit_status(char *error, int status);
void		free_pipe_elems(t_pipe_cmd *c);
void		free_pipe_cmd(t_pipe_cmd *begin_list);

// parse_out_quotes.c
int			reserved_chars(t_pars *p, t_package *s);
void		argument_w_spaces(char *word, int append, t_pars *p, t_package *s);
void		do_append(t_pars *p, t_package *s);
void		do_add(t_pars *p, t_package *s);
int			out_quotes(t_pars *p, t_package *s);

// out_quotes_utils.c
int			must_append(int i, char *cmd, t_pars *p);
char		*make_word(char	*word, t_pars *p, t_package *s);

// double_quotes.c
void		doubleq_special(t_pars *p, char **buff, t_package *s);
char		*make_dquotes_arg(t_pars *p, t_package *s);
char		*arg_double_quotes(t_pars *p, t_package *s);
void		in_double_quotes(t_pars *p, t_package *s);

// parser.c
void		init_package(t_package *s, t_command *b_list, char *cmd);
t_pipe_cmd	*parser(char *cmd, int new_command, t_command *b_list);

// add_arguments.c
void		add_argument2(char **new_args, char *word, t_pipe_cmd *last);
void		add_argument(char *word, t_package *s, char *word_to_free, t_pars *p);
int			append_arg(t_pipe_cmd *last, char *word, char *tmp);
char		*copy_next_word(t_pars *p, int wd_size, t_package *s);
char		*get_next_word(t_pars *p, t_package *s);

// utils.c
t_pipe_cmd	*init_pipe_list(void);
void		init_pars_struct(t_pars *p, int new_command, char c);
void		reset_pars_struct(t_pars *p);
void		set_quotes(int i, char *cmd, t_pars *p);

// check_syntax.c
int			semicolons_valid(char *cmd);
int			pipe_is_last_char(char *cmd, int i);
int			pipes_valid(char *cmd);
int			are_quotes_closed(char *cmd);
int			check_syntax(char *cmd);

// check_chars.c
int			is_space(char c);
int			is_r_space(char *c, int i);
int			is_r_resvd_char(char *c, int i, int dollar);
int			is_quote(char c);
int			is_r_quote(char *c, int i);

// check_chars2.c
int			is_unesc_char(char *c, int i);
int			is_reserved_char(char c, int dollar);
int			is_number(char c);
int			is_letter(char c);
int			valid_var_char(char c);

// check_chars3.c
int			is_unesc_simple(char *c, int i);
int			is_unesc_double(char *c, int i);
int			space_in_word(char	*word);
int			space_middle_of_word(char *word);

// linked_list.c
t_pipe_cmd	*ft_lstlast(t_pipe_cmd *begin_list);
void		ft_lstadd_back(t_pipe_cmd **begin_list, t_pipe_cmd *new_elem);
t_pipe_cmd	*new_elem(void);
void		print_list(t_pipe_cmd *begin_list);

// libft.c
void		free_in_join(char *s1, char *s2);
char		*ft_strjoin_w_ns(char *s1, char *s2);
char		*ft_strdup_len(char *str, int len);
char		*get_env_var(int index);
// env_vars.c
char		*dup_dollar(t_package *s);
char		*unvalid_var_name(t_pars *p, t_package *s, char *wrd, char *tmp_w);
char		*valid_var_name(t_pars *p, t_package *s, char *var_name);
char		*get_variable(t_pars *p, t_package *s);

// simple_quotes.c
void		append_simple(char *new_arg, t_package *s);
char		*arg_simple_quotes(t_pars *p, t_package *s);
void		in_simple_quotes(t_pars *p, t_package *s);

// signs.c
char		*get_redir_word(t_pars *p, t_package *s, char *word, char *new_w);
int			real_greater_sign(char *word, t_pipe_cmd *last);
int			real_smaller_sign(char *word, t_pipe_cmd *last);
int			real_sign2(t_pars *p, char sign, t_package *s);
int			real_sign(t_pars *p, t_package *s);

// dupl_fcts.c
char		*ft_strdup(char *str);
int			ft_strlen(char *str);
int			strcmp_env(char *s1, char *s2);
char		**new_env(char **env);

// execution
char		*itoa_env_var(char *prefix, int n);

#endif
