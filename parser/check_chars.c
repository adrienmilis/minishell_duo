#include "parser.h"

int	is_space(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	is_r_space(char *c, int i)
{
	if (is_space(*c) && is_unesc_char(c, i))
		return (1);
	return (0);
}

int	is_r_resvd_char(char *c, int i, int dollar)
{
	if (is_reserved_char(*c, dollar) && is_unesc_char(c, i))
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

int	is_r_quote(char *c, int i)
{
	if (is_quote(*c) && is_unesc_char(c, i))
		return (1);
	return (0);
}
