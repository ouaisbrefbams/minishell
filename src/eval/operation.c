/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 15:27:22 by charles           #+#    #+#             */
/*   Updated: 2020/09/15 20:30:27 by charles          ###   ########.fr       */
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

		/* left_pid = fork(); */
		/* if (left_pid != 0) */
		/* { */
		/* 	dup2(p[FD_WRITE], STDOUT_FILENO); */
		/* 	close(p[FD_READ]); */
		/* 	exit(eval(left_fds, env, ast->op.left, NULL)); */
		/* } */
        /*  */
		/* right_pid = fork(); */
		/* if (right_pid != 0) */
		/* { */
		/* 	dup2(p[FD_READ], STDIN_FILENO); */
		/* 	close(p[FD_WRITE]); */
		/* 	exit(eval(right_fds, env, ast->op.right, NULL)); */
		/* } */
		eval(left_fds, env, ast->op.left, &left_pid, p[FD_READ]);
		close(p[FD_WRITE]);
		status = eval(right_fds, env, ast->op.right, &right_pid, p[FD_WRITE]);
		close(p[FD_READ]);

		wait(&left_pid);
		wait(&left_pid);
		return (status);
	}
	if ((status = eval(left_fds, env, ast->op.left, NULL, FD_NONE)) == EVAL_FATAL)
		return (EVAL_FATAL);
	g_state.last_status = status;
	if ((ast->op.sep == TAG_AND && status != 0) ||
		(ast->op.sep == TAG_PIPE && status != 0) ||
		(ast->op.sep == TAG_OR && status == 0))
		return (status);
	return (eval(right_fds, env, ast->op.right, NULL, FD_NONE));
}
