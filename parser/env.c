#include "parser.h"

char	*get_env_var(int index)
{
	char	*var_value;
	int		j;

	j = 0;
	while (myenv[index][j] != '=' && myenv[index][j])
		j++;
	var_value = ft_strdup(myenv[index] + j + 1);
	if (!var_value)
		return (NULL);
	return (var_value);
}
