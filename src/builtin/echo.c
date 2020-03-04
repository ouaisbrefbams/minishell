#include "minishell.h"

int	ms_echo(char **argv)
{
	bool	newline;

	newline = ft_strcmp(argv[1], "-n") == 0;
	while (*++argv != NULL)
	{
		ft_putstr(*argv);
		ft_putchar(' ');
	}
	if (newline)
		ft_putchar('\n');
	return (0);
}
