/**
** \file   unset.c
** \brief  `unset` builtin
*/

#include "minishell.h"

int	ms_unset(t_env env, char **argv)
{
	if (argv[1] == NULL)
		return (1);
	ft_htdelone(env, argv[1], ms_ht_del_str_entry);
	return (0);
}
