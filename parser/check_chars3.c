#include "parser.h"

int	is_unesc_simple(char *c, int i)
{
	if (is_unesc_char(c, i) && *c == '\'')
		return (1);
	return (0);
}

int	is_unesc_double(char *c, int i)
{
	if (is_unesc_char(c, i) && *c == '"')
		return (1);
	return (0);
}