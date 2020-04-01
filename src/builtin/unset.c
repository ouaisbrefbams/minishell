/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:51 by charles           #+#    #+#             */
/*   Updated: 2020/04/01 23:05:33 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   unset.c
** \brief  `unset` builtin
*/

#include "minishell.h"

int	builtin_unset(char **argv, t_env env)
{
	size_t	i;

	if (argv[1] == NULL)
		return (1);
	i = 0;
	while (i < env->size)
		if (ft_strncmp(env->data[i], argv[1], ft_strlen(argv[1])) == 0)
		{
			ft_vecremove(env, i, free);
			return (0);
		}
	return (1);
}
