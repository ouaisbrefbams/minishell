/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:16 by charles           #+#    #+#             */
/*   Updated: 2020/09/16 16:26:08 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   exit.c
** \brief  `exit` builtin
*/

#include "minishell.h"

/*
** \brief       Exit the current process with a status
** \param argv  arguments
** \param env   environment
** \return      always 0
*/

int	builtin_exit(char **argv, t_env env)
{
	long	status;
	char	*after;

	(void)env;
	if (argv[1] == NULL)
		status = g_state.last_status;
	else
	{
		errno = 0;
		status = ft_strtol(argv[1], &after, 10);
		while (ft_isblank(*after))
			after++;
		if (*after != '\0' || errno == ERANGE)
		{
			errorf("exit: %s: numeric argument required\n", argv[1]);
			return (2);
		}
		if (argv[2] != NULL)
			return (errorf_ret(1, "exit: too many arguments\n"));
	}
	exit(status % 256);
	return (0);
}
