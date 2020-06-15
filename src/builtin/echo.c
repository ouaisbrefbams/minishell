/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:47 by charles           #+#    #+#             */
/*   Updated: 2020/06/15 10:12:53 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   echo.c
** \brief  `echo` builtin
*/

#include "minishell.h"

int	builtin_echo(char **argv, t_env env)
{
	bool	newline;

	(void)env;
	if (argv[1] == NULL)
	{
		ft_putchar('\n');
		return (0);
	}
	newline = !ft_strcmp(argv[1], "-n") == 0;
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
