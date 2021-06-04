#include "msh.h"

int	pointer_in_history(char *buffer, t_command *begin_list)
{
	t_command	*elem;

	if (!begin_list)
		return (0);
	if (!buffer)
		return (0);
	elem = begin_list;
	while (elem)
	{
		if (buffer == elem->command)
			return (1);
		elem = elem->next;
	}
	return (0);
}

t_command	*new_elem_history(char *str)
{
	t_command	*new_elem;

	new_elem = malloc(sizeof(t_command));
	if (!new_elem)
		return (NULL);
	new_elem->next = NULL;
	new_elem->prev = NULL;
	new_elem->command = str;
	return (new_elem);
}

void	ft_lstadd_front(t_command **begin_list, t_command *new_elem)
{
	t_command	*bg_list;

	new_elem->next = *begin_list;
	bg_list = *begin_list;
	bg_list->prev = new_elem;
	*begin_list = new_elem;
}

void	print_list_history(t_command *begin_list)
{
	t_command	*elem;
	int			i;

	i = 0;
	elem = begin_list;
	while (elem->next)
	{
		printf("[%d] : %s\n", i, elem->command);
		elem = elem->next;
		i++;
	}
}

void	free_list(t_command	*begin_list)
{
	t_command	*prev;
	t_command	*current;

	if (!begin_list)
		return ;
	prev = begin_list;
	current = prev->next;
	while (current)
	{
		if (prev)
		{
			if (prev->command)
				free(prev->command);
			free(prev);
		}
		prev = current;
		current = current->next;
	}
	if (prev)
	{
		if (prev->command)
			free(prev->command);
		free(prev);
	}
}
