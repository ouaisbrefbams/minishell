/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 15:27:22 by charles           #+#    #+#             */
/*   Updated: 2020/09/15 16:43:21 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

int			eval_operation(int fds[2], t_env env, t_ast *ast)
{
	int	status;
	int	left_fds[2];
	int	right_fds[2];
	int p[2];

	left_fds[FD_READ] = fds[FD_READ];
	left_fds[FD_WRITE] = FD_NONE;
	right_fds[FD_READ] = FD_NONE;
	right_fds[FD_WRITE] = fds[FD_WRITE];
	if (ast->op.sep == TAG_PIPE)
	{
		pipe(p);
		left_fds[FD_WRITE] = p[FD_WRITE];
		right_fds[FD_READ] = p[FD_READ];

		pid_t left_pid;
		pid_t right_pid;
		eval(left_fds, env, ast->op.left, &left_pid);
		close(p[FD_WRITE]);
		status = eval(right_fds, env, ast->op.right, &right_pid);
		close(p[FD_READ]);

		pid_t finished;
		finished = wait(&finished);
		if (finished == left_pid)
		{
			waitpid(right_pid, &finished, 0);
		}
		else if (finished == right_pid)
		{
			/* kill(left_pid, SIGTERM); // FIXME weird bug with parent on both sides */
			waitpid(left_pid, &finished, 0);
		}
		return (status);
	}
	if ((status = eval(left_fds, env, ast->op.left, NULL)) == EVAL_FATAL)
		return (EVAL_FATAL);
	g_last_status = status;
	if ((ast->op.sep == TAG_AND && status != 0) ||
		(ast->op.sep == TAG_PIPE && status != 0) ||
		(ast->op.sep == TAG_OR && status == 0))
		return (status);
	return (eval(right_fds, env, ast->op.right, NULL));
}
