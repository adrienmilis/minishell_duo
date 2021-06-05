#include "execution.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return ((char *)s + i);
	if (s[i] == c)
		return ((char *)s + i);
	return (NULL);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned int	i;

	i = 0;
	if ((dest || src) && (dest != src))
	{
		if (src < dest)
			while (n--)
				*((char *)dest + n) = *((char *)src + n);
		else
		{
			while (i < n)
			{
				*((char *)dest + i) = *((char *)src + i);
				i++;
			}
		}
	}
	return (dest);
}

static void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	posn;

	posn = n;
	if (n < 0)
		posn = -n;
	if (n < 0)
		ft_putchar_fd('-', fd);
	if (posn >= 10)
		ft_putnbr_fd(posn / 10, fd);
	ft_putchar_fd(posn % 10 + 48, fd);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n < 1)
		return (0);
	while (s1[i] && s2[i] && i < n - 1)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
