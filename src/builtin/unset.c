/**
** \file   unset.c
** \brief  `unset` builtin
*/

#include "minishell.h"

int	builtin_unset(char **argv, t_env env)
{
	if (argv[1] == NULL)
		return (1);
	ft_htdelone(env, argv[1], ht_del_str_entry);
	return (0);
}
