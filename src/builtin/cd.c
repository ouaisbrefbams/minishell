/**
** \file   cd.c
** \brief  `cd` builtin
*/

#include "minishell.h"

int	builtin_cd(char **argv, t_env env)
{
	char	*path;

	path = argv[1];
	if (argv[1] == NULL)
		path = ft_htget(env, "HOME");
	if (path == NULL)
		return (1);
	if (chdir(path) == -1)
		return (1);
	return (0);
}
