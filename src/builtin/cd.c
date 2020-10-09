/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:20 by charles           #+#    #+#             */
/*   Updated: 2020/10/09 12:44:55 by cacharle         ###   ########.fr       */
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
	char	cwd[PATH_MAX + 1];
	char	*oldwd;
	char	*home;

	(void)env;
	oldwd = env_search(env, "PWD", NULL);
	if (oldwd == NULL)
		oldwd = "";
	if (argv[1] != NULL && argv[1][0] == '\0')
		return (env_export(env, "OLDPWD", oldwd) == NULL ? EVAL_FATAL : 0);
	if (argv[1] == NULL)
	{
		if ((home = env_search(env, "HOME", NULL)) == NULL)
			return (errorf_ret(1, "cd: HOME not set\n"));
		argv[1] = home;
	}
	if (chdir(argv[1]) == -1)
		return (errorf_ret(1, "cd: %s: %s\n", argv[1], strerror(errno)));
	if (env_export(env, "OLDPWD", oldwd) == NULL)
		return (EVAL_FATAL);
	if (getcwd(cwd, PATH_MAX) == NULL)
		return (1);
	if (env_export(env, "PWD", cwd) == NULL)
		return (EVAL_FATAL);
	return (0);
}
