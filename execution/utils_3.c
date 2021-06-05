#include "execution.h"

void	*ft_memset(void *s, int c, size_t n)
{
	while (n--)
		*((char *)s + n) = c;
	return (s);
}

int	ft_putchar(int c)
{
	write(1, &c, 1);
	return (1);
}
