#ifndef MSH_H
# define MSH_H

#include <termios.h>
#include <curses.h>
#include <term.h>
#include <string.h>
#include "execution/execution.h"

void		c_option(char *argv2);	// remove

// parsing
void		free_pipe_elems(t_pipe_cmd *c);
void		free_pipe_cmd(t_pipe_cmd *begin_list);
t_pipe_cmd	*parser(char *cmd, int new_command, t_command *b_list);	// ajouter a execution.h
int			ft_putchar(int c);

// new_megashell.c
void		write_hst_command(t_command *elem, char **buffer, int up, t_command *beg_list);
int			main2(char *buffer, t_command *begin_list, int c, char *argv2);
void		init_begin_list(t_command **begin_list);
void		first_pwd(t_command *begin_list);
int			main(int argc, char **argv, char **env);

// minishell_utils.c
int			init_termcap(void);
int			shlvl2(void);
int			shlvl(void);
char		*make_buffer(char *buf, char c, t_command *begin_list);
void		del_char_buffer(char **buffer, t_command *begin_list);

// minishell_utils2.c
int			ft_isprint(int c);
void		ft_putstr(char *str);
void		error_free(char *buffer, t_command *begin_list, int str);

// msh_misc.c
int			ctrlC(int new_value);
void		handler_sigint(int sig);
void		nothing_sigquit(int sig);
void		termcaps_del(void);

// msh_main_loop.c
void		updown_event(int *rst, char rd[3], t_command *beg_list, char **buffer);
int			enter_event_null_cmd(char **buffer, t_command **begin_list);
int			enter_event(char **buffer, t_command **begin_list, t_command *new_elem, t_pipe_cmd	*pipe_cmd);
void		print_and_add_char(char **buffer, char rd[4], t_command *begin_list);
int			read_input(char **buffer, t_command **begin_list, int c, char *argv2);

// list_utils.c
int			pointer_in_history(char *buffer, t_command *begin_list);
t_command	*new_elem_history(char *str);
void		ft_lstadd_front(t_command **begin_list, t_command *new_elem);
void		print_list_history(t_command *begin_list);		// pas utile
void		free_list(t_command	*begin_list);

#endif