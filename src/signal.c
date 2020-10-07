/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 09:16:16 by charles           #+#    #+#             */
/*   Updated: 2020/10/07 15:02:56 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "minishell.h"

void	signal_sigint(int signum)
{
	(void)signum;
	if (g_state.killed || g_state.is_child)
		return ;
	if (g_child_pid != -1)
	{
		g_state.last_status = 1;
		if (kill(g_child_pid, SIGINT) != -1)
			ft_putchar('\n');
		g_state.killed = true;
	}
	else
	{
		ft_putchar('\n');
		print_prompt();
	}
}

void	signal_sigquit(int signum)
{
	if (g_state.killed)
		return ;
	(void)signum;
	if (g_child_pid != -1)
	{
		g_state.last_status = 131;
		kill(g_child_pid, SIGQUIT);
		ft_putstr("Quit (core dumped)\n");
	}
}

void	signal_sigterm(int signum)
{
	(void)signum;
}
