/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:20 by charles           #+#    #+#             */
/*   Updated: 2020/04/02 11:33:15 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   cd.c
** \brief  `cd` builtin
*/

#include "minishell.h"

int	builtin_cd(char **argv, t_env env)
{
	(void)env;
	if (argv[1] == NULL)
		return (1);
	if (chdir(argv[1]) == -1)
		return (1);
	return (0);
}
