#include "minishell.h"

int	ms_pwd(void)
{
	char	buf[PATH_MAX];

	ft_bzero(buf, PATH_MAX);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (1);
	ft_putendl(buf);
	return (0);
}
