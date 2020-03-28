#include "minishell.h"

void	st_print_env_variable(t_ftht_content *content)
{
	ft_putstr(content->key);
	ft_putchar('=');
	ft_putstr((char*)content->value);
	ft_putchar('\n');
}

int		ms_env(t_env env)
{
	ft_htiter(env, st_print_env_variable);
	return (0);
}
