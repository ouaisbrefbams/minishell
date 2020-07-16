/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 09:16:16 by charles           #+#    #+#             */
/*   Updated: 2020/07/16 09:18:21 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "eval.h"

void signal_sigint(int signum)
{
	(void)signum;
	g_last_status_code = 130;
	if (g_child_pid != -1)
	{
		kill(g_child_pid, SIGINT);
		ft_putchar('\n');
	}
	else
	{
		ft_putchar('\n');
		print_prompt();
	}
}

void signal_sigquit(int signum)
{
	(void)signum;
	g_last_status_code = 131;
	if (g_child_pid != -1)
	{
		kill(g_child_pid, SIGQUIT);
		ft_putstr("Quit (core dumped)\n");
	}
}

void signal_sigterm(int signum)
{
	(void)signum;
}

