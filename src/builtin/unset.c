#include "minishell.h"

int	ms_unset(t_state *state, char **argv)
{
	if (argv[1] == NULL)
		return (1);
	ft_htdelone(state->environment, argv[1], ms_ht_del_str_entry);
	return (0);
}
