/**
** \file   env.c
** \brief  `env` builtin
*/

#include "minishell.h"

void	st_print_env_variable(t_ftht_entry *entry)
{
	ft_putstr(entry->key);
	ft_putchar('=');
	ft_putstr((char*)entry->value);
	ft_putchar('\n');
}

int		builtin_env(char **argv, t_env env)
{
	(void)argv;
	ft_htiter(env, st_print_env_variable);
	return (0);
}
