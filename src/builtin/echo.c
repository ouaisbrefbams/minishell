/**
** \file   echo.c
** \brief  `echo` builtin
*/

#include "minishell.h"

int	builtin_echo(char **argv, t_env env)
{
	bool	newline;

	(void)env;
	newline = ft_strcmp(argv[1], "-n") == 0;
	if (newline)
		argv++;
	while (*++argv != NULL)
	{
		ft_putstr(*argv);
		if (*(argv + 1) != NULL)
			ft_putchar(' ');
	}
	if (newline)
		ft_putchar('\n');
	return (0);
}
