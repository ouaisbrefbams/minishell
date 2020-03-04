#include "minishell.h"

void	st_print_env_variable(t_ftht_content *content)
{
	ft_putstr(content->key);
	ft_putchar('=');
	ft_putstr((char*)content->value);
	ft_putchar('\n');
}

int		ms_env(t_state *state)
{
	ft_htiter(state->environment, st_print_env_variable);
	return (0);
}
