/**
** \file   pwd.c
** \brief  `pwd` builtin
*/

#include "minishell.h"

int	builtin_pwd(void)
{
	char	buf[PATH_MAX];

	ft_bzero(buf, PATH_MAX);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (1);
	ft_putendl(buf);
	return (0);
}
