/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:40 by charles           #+#    #+#             */
/*   Updated: 2020/07/19 20:27:28 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   pwd.c
** \brief  `pwd` builtin
*/

#include "minishell.h"

int	builtin_pwd(char **argv, t_env env)
{
	char	buf[PATH_MAX];
	char	*working_directory;

	(void)argv;
	if ((working_directory = env_search(env, "PWD")) == NULL)
	{
		ft_bzero(buf, PATH_MAX);
		if (getcwd(buf, PATH_MAX) == NULL)
			return (1);
		ft_putendl(buf);
	}
	else
		ft_putendl(working_directory);
	return (0);
}
