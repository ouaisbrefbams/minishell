/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:20 by charles           #+#    #+#             */
/*   Updated: 2020/09/15 17:03:32 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   cd.c
** \brief  `cd` builtin
*/

#include "minishell.h"

/*
** \brief       Change directory
** \param argv  arguments
** \param env   environment
** \return      a status code or EVAL_FATAL on fatal error
*/

int	builtin_cd(char **argv, t_env env)
{
	char	buf[PATH_MAX + 1];
	char	*home;

	(void)env;
	if (argv[1] != NULL && argv[2] != NULL)
		return (errorf_ret(1, "cd: too many arguments\n"));
	if (argv[1] != NULL && argv[1][0] == '\0')
		return (0);
	if (argv[1] == NULL)
	{
		if ((home = env_search(env, "HOME", NULL)) == NULL)
			return (errorf_ret(1, "cd: HOME not set\n"));
		argv[1] = home;
	}
	errno = 0;
	if (chdir(argv[1]) == -1)
		return (errorf_ret(1, "cd: %s: %s\n", argv[1], strerror(errno)));
	if (getcwd(buf, PATH_MAX) == NULL)
		return (1);
	if (env_export(env, "PWD", buf) == NULL)
		return (EVAL_FATAL);
	return (0);
}
