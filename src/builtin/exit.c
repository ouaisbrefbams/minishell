/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:16 by charles           #+#    #+#             */
/*   Updated: 2020/06/23 10:03:39 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** \file   exit.c
** \brief  `exit` builtin
*/

int	builtin_exit(char **argv, t_env env)
{
	int	status;

	(void)env;
	if (argv[1] == NULL)
		status = 0;
	else if (argv[2] != NULL)
	{
		// replace with minishell error system
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return 1;
	}
	else
	{
		errno = 0;
		status = ft_atoi_strict(argv[1]);
		if (errno != 0)
		{
			// replace with minishell error system
			ft_putendl_fd("minishell: exit: ---argv[1]---: numeric argument required", STDERR_FILENO);
			return 2;
		}
	}
	exit(status % 256);
	return (0);
}
