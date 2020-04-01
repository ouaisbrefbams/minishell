/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:58 by charles           #+#    #+#             */
/*   Updated: 2020/04/01 17:05:59 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   pipe.c
** \brief  Pipes setup
*/

#include "eval.h"

/*
** \brief           Setup STDIN and STDOUT pipe in the parent process
** \param cmd       Command to setup
** \param pipe_in   STDIN pipe
** \param pipe_out  STDOUT pipe
** \return          -1 on error, 0 otherwise
*/

int	pipe_setup_parent(t_cmd *cmd, int pipe_in[2], int pipe_out[2])
{
	if (cmd->in != NULL)
	{
		if ((pipe_in[PIPE_WRITE] = open(cmd->in, O_RDONLY)) < 0)
			return (-1);
	}
	if (cmd->out != NULL)
	{
		if ((pipe_out[PIPE_READ] = open(cmd->out,
				(cmd->is_append ? O_WRONLY : O_APPEND) | O_CREAT)) < 0)
			return (-1);
	}
	return (0);
}

/*
** \brief           Setup STDIN and STDOUT pipe in the child process
** \param pipe_in   STDIN pipe
** \param pipe_out  STDOUT pipe
** \return          -1 on error, 0 otherwise
*/

int	pipe_setup_child(int pipe_in[2], int pipe_out[2])
{
	if (pipe_in[PIPE_READ] != PIPE_CLOSED)
		if (dup2(STDIN_FILENO, pipe_in[PIPE_READ]) == -1)
			return (-1);
	if (pipe_out[PIPE_WRITE] != PIPE_CLOSED)
		if (dup2(STDOUT_FILENO, pipe_out[PIPE_WRITE]) == -1)
			return (-1);
	return (0);
}
