#include "minishell.h"

int	ms_pwd(t_state *state)
{
	char	buf[PATH_MAX];

	(void)state;
	ft_bzero(buf, PATH_MAX);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (1);
	ft_putendl(buf);
	return (0);
}
