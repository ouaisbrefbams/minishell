/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:16 by charles           #+#    #+#             */
/*   Updated: 2020/07/17 16:36:03 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** \file   exit.c
** \brief  `exit` builtin
*/

int	builtin_exit(char **argv, t_env env)
{
	long	status;
	char	*after;

	(void)env;
	if (argv[1] == NULL)
		status = g_last_status_code;
	else
	{
		errno = 0;
		status = ft_strtol(argv[1], &after, 10);
		while (ft_isblank(*after))
			after++;
		if (*after != '\0' || errno == ERANGE)
		{
			// replace with minishell error system
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(argv[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			return 2;
		}
		if (argv[2] != NULL)
		{
			// replace with minishell error system
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			return 1;
		}
	}
	exit(status % 256);
	return (0);
}
