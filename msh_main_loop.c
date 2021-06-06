#include "msh.h"

void	updown_event(int *rst, char rd[3], t_command *beg_list, char **buffer)
{
	static t_command	*elem;

	if (*rst == 1)
		elem = NULL;
	*rst = 0;
	if (beg_list->next == NULL)
		return ;
	if (rd[2] == 65)
	{
		if (elem == NULL)
			elem = beg_list;
		else if (elem->next->next != NULL)
			elem = elem->next;
		write_hst_command(elem, buffer, 1, beg_list);
	}
	if (rd[2] == 66)
	{
		if (elem == NULL || elem == beg_list)
			elem = NULL;
		else
			elem = elem->prev;
		write_hst_command(elem, buffer, 0, beg_list);
	}
}

int	enter_event_null_cmd(char **buffer, t_command **begin_list)
{
	t_command	*new_elem;

	if (*buffer && (*buffer)[0])
	{
		new_elem = new_elem_history(*buffer);
		if (!new_elem)
			error_free(*buffer, *begin_list, 1);
		ft_lstadd_front(begin_list, new_elem);
	}
	*begin_list = *begin_list;
	write(1, "megashell> ", 11);
	*buffer = NULL;
	return (1);
}

int	enter_event(char **b, t_command **b_list, t_command *n_lm, t_pipe_cmd *p_c)
{
	int	backslash;

	backslash = 0;
	write(1, "\n", 1);
	p_c = parser(*b, 1, *b_list);
	if (p_c == NULL)
		return (enter_event_null_cmd(b, b_list));
	while (p_c)
	{
		if (p_c->cmd || p_c->input)
			if (!exec_pipe_cmd(p_c, &backslash))
				error_free(*b, *b_list, 1);
		free_pipe_cmd(p_c);
		p_c = parser(*b, 0, *b_list);
	}
	free_pipe_cmd(p_c);
	n_lm = new_elem_history(*b);
	if (!n_lm)
		error_free(*b, *b_list, 1);
	ft_lstadd_front(b_list, n_lm);
	*b = NULL;
	if (backslash)
		write(1, "\n", 1);
	write(1, "megashell> ", 11);
	return (1);
}

void	print_and_add_char(char **buffer, char rd[4], t_command *begin_list)
{
	write(1, &rd[0], 1);
	*buffer = make_buffer(*buffer, rd[0], begin_list);
	if (*buffer == NULL)
		error_free(*buffer, begin_list, 1);
}

int	read_input(char **buffer, t_command **begin_list, int c, char *argv2)
{
	char		rd[4];
	int			ret;
	static int	reset;

	if (c)						// remove
		c_option(argv2);

	signal(SIGINT, &handler_sigint);
	signal(SIGQUIT, &nothing_sigquit);
	ret = read(0, rd, 1);
	if (ret == -1)
		error_free(*buffer, *begin_list, 1);
	if (ctrlC(0))
	{
		free(*buffer);
		*buffer = NULL;
	}
	if (ft_isprint(rd[0]))
		print_and_add_char(buffer, rd, *begin_list);
	else if (rd[0] == '\033')
	{
		ret = read(0, rd + 1, 3);
		updown_event(&reset, rd, *begin_list, buffer);
	}
	else if (rd[0] == 127 && ft_strlen(*buffer) > 0)
		del_char_buffer(buffer, *begin_list);
	else if (rd[0] == 4 && ft_strlen(*buffer) == 0)
		error_free(*buffer, *begin_list, 0);
	else if (rd[0] == 10)
		reset = enter_event(buffer, begin_list, NULL, NULL);
	return (ret);
}
