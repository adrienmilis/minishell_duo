#include "parser.h"

int	is_unesc_char(char *c, int i)
{
	int	count;

	count = 0;
	if (i == 0)
		return (1);
	i--;
	while (i >= 0 && *(c - 1 - count) == '\\')
	{
		i--;
		count++;
	}
	if (count % 2 == 0)
		return (1);
	return (0);
}

int	is_reserved_char(char c, int dollar)
{
	if (dollar == 0)
	{
		if (c == '|' || c == ';' || c == '>' || c == '<')
			return (1);
	}
	else
	{
		if (c == '|' || c == ';' || c == '>' || c == '<' || c == '$')
			return (1);
	}
	return (0);
}

int	is_number(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	is_letter(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	valid_var_char(char c)
{
	if (is_number(c) || is_letter(c) || c == '_' || c == '?')
		return (1);
	return (0);
}
