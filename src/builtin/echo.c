/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:47 by charles           #+#    #+#             */
/*   Updated: 2020/09/13 20:24:34 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   echo.c
** \brief  `echo` builtin
*/

#include "minishell.h"

/*
** \brief       Print it's arguments out
** \param argv  arguments
** \param env   environment
** \return      a status code or EVAL_FATAL on fatal error
*/

int	builtin_echo(char **argv, t_env env)
{
	bool	newline;

	(void)env;
	if (argv[1] == NULL)
	{
		ft_putchar('\n');
		return (0);
	}
	newline = !(ft_strcmp(argv[1], "-n") == 0);
	if (!newline)
		argv++;
	while (*++argv != NULL)
	{
		ft_putstr(*argv);
		if (*(argv + 1) != NULL)
			ft_putchar(' ');
	}
	if (newline)
		ft_putchar('\n');
	return (0);
}
