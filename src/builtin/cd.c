/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:20 by charles           #+#    #+#             */
/*   Updated: 2020/06/17 12:36:29 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   cd.c
** \brief  `cd` builtin
*/

#include "minishell.h"

int	builtin_cd(char **argv, t_env env)
{
	char buf[PATH_MAX];

	(void)env;
	if (argv[1] == NULL)
		return (1);
	if (chdir(argv[1]) != -1)
	{
		if (!(getcwd(buf, PATH_MAX)))
			return(1);
		env_export(env, "PWD", buf);
	}
	return (0);
}
