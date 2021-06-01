#ifndef MSH_H
# define MSH_H

#include <termios.h>
#include <curses.h>
#include <term.h>
#include <string.h>
#include "execution/execution.h"

// int	in_history;

typedef struct s_command
{
	char				*command;
	struct s_command	*prev;
	struct s_command	*next;
}				t_command;

// parsing
void		free_pipe_elems(t_pipe_cmd *c);
void		free_pipe_cmd(t_pipe_cmd *begin_list);
t_pipe_cmd	*parser(char *cmd, int mew_command);	// ajouter a execution.h
int			ft_putchar(int c);

// minishell_utils.c
int			ft_isprint(int c);
void		error_free(char *buffer, t_command *begin_list);
int			init_termcap();
int			shlvl();
char		*make_buffer(char *buf, char c);
void		c_option(char *argv2);
void		del_char_buffer(char **buffer, t_command *begin_list);
void		ft_putstr(char *str);

// list_utils.c
int			pointer_in_history(char *buffer, t_command *begin_list);
t_command	*new_elem_history(char *str);
void		ft_lstadd_front(t_command **begin_list, t_command *new_elem);
void		print_list_history(t_command *begin_list);		// pas utile
void		free_list(t_command	*begin_list);

#endif