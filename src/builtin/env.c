/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:32 by charles           #+#    #+#             */
/*   Updated: 2020/09/13 20:23:59 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   env.c
** \brief  `env` builtin
*/

#include "minishell.h"

/*
** \brief       Print the environment variables (one on each line)
** \param argv  arguments
** \param env   environment
** \return      a status code or EVAL_FATAL on fatal error
*/

int		builtin_env(char **argv, t_env env)
{
	(void)argv;
	ft_veciter(env, (void (*)(void*))ft_putendl);
	return (0);
}
