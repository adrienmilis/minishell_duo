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

int	space_in_word(char	*word)
{
	int	i;

	i = 0;
	if (!word)
		return (0);
	while (word[i])
	{
		if (is_space(word[i]))
			return (1);
		i++;
	}
	return (0);
}

int	space_middle_of_word(char *word)
{
	int	i;

	i = 0;
	while (is_space(word[i]))
		i++;
	while (!is_space(word[i]) && word[i])
		i++;
	while (is_space(word[i]))
		i++;
	if (word[i])
		return (1);
	return (0);
}