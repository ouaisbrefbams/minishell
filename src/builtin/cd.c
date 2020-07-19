/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:20 by charles           #+#    #+#             */
/*   Updated: 2020/07/19 15:55:08 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   cd.c
** \brief  `cd` builtin
*/

#include "minishell.h"

int	builtin_cd(char **argv, t_env env)
{
	char	buf[PATH_MAX];
	char	*home;

	(void)env;
	if (argv[1] != NULL && argv[2] != NULL)
	{
		ft_putstr_fd(g_basename, STDERR_FILENO);
		ft_putendl_fd(": cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (argv[1] != NULL && argv[1][0] == '\0')
		return (0);
	if (argv[1] == NULL)
	{
		if ((home = env_search(env, "HOME")) == NULL)
		{
			ft_putstr_fd(g_basename, STDERR_FILENO);
			ft_putendl_fd(": cd: HOME not set", STDERR_FILENO);
			return (1);
		}
		argv[1] = home;
	}
	errno = 0;
	if (chdir(argv[1]) == -1)
	{
		ft_putstr_fd(g_basename, STDERR_FILENO);
		ft_putstr_fd(": cd: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (1);
	}
	if (!(getcwd(buf, PATH_MAX)))
		return (1);
	if (env_export(env, "PWD", buf) == NULL)
		return (2); // FIXME malloc error recognition in builtins and cmd
	return (0);
}
