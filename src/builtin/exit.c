/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:16 by charles           #+#    #+#             */
/*   Updated: 2020/08/20 17:31:18 by charles          ###   ########.fr       */
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
			errorf("exit: %s: numeric argument required\n", argv[1]);
			return (2);
		}
		if (argv[2] != NULL)
		{
			errorf("exit: too many arguments\n");
			return (1);
		}
	}
	exit(status % 256);
	return (0);
}
