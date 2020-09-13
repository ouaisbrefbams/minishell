/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:40 by charles           #+#    #+#             */
/*   Updated: 2020/09/13 20:23:15 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   pwd.c
** \brief  `pwd` builtin
*/

#include "minishell.h"

/*
** \brief       Print the current directory
** \param argv  arguments
** \param env   environment
** \return      a status code or EVAL_FATAL on fatal error
*/

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
