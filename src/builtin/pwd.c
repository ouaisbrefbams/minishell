/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:40 by charles           #+#    #+#             */
/*   Updated: 2020/04/01 17:10:40 by charles          ###   ########.fr       */
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

	(void)argv;
	(void)env;
	ft_bzero(buf, PATH_MAX);
	if (getcwd(buf, PATH_MAX) == NULL)
		return (1);
	ft_putendl(buf);
	return (0);
}
