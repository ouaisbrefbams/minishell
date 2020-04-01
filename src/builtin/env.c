/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:32 by charles           #+#    #+#             */
/*   Updated: 2020/04/01 22:25:43 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   env.c
** \brief  `env` builtin
*/

#include "minishell.h"

int		builtin_env(char **argv, t_env env)
{
	(void)argv;
	ft_veciter(env, (void (*)(void*))ft_putendl);
	return (0);
}
