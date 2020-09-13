/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:16 by charles           #+#    #+#             */
/*   Updated: 2020/09/13 20:25:55 by charles          ###   ########.fr       */
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
		status = g_last_status;
	else
	{
		errno = 0;
		status = ft_strtol(argv[1], &after, 10);
		while (ft_isblank(*after))
			after++;
		if (*after != '\0' || errno == ERANGE)
			return (errorf_ret(2, "exit: %s: numeric argument required\n", argv[1]));
		if (argv[2] != NULL)
			return (errorf_ret(1, "exit: too many arguments\n"));
	}
	exit(status % 256);
	return (0);
}
