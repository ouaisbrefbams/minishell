#include "minishell.h"

int	ms_cd(t_state *state, char **argv)
{
	char	*path;

	path = argv[1];
	if (argv[1] == NULL)
		path = ft_htget(state->environment, "HOME");
	if (path == NULL)
		return (1);
	if (chdir(path) == -1)
		return (1);
	return (0);
}
