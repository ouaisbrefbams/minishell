/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 15:27:22 by charles           #+#    #+#             */
/*   Updated: 2020/10/09 14:00:04 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

int			eval_operation(int fds[2], t_env env, t_ast *ast)
{
	int	status;
	int	left_fds[2];
	int	right_fds[2];

	left_fds[FD_READ] = fds[FD_READ];
	left_fds[FD_WRITE] = FD_NONE;
	right_fds[FD_READ] = FD_NONE;
	right_fds[FD_WRITE] = fds[FD_WRITE];
	if ((status = eval(left_fds, env, ast->op.left)) == EVAL_FATAL)
		return (EVAL_FATAL);
	g_state.last_status = status;
	if (g_state.killed)
		return (status);
	if ((ast->op.sep == TAG_AND && status != 0) ||
		(ast->op.sep == TAG_OR && status == 0))
		return (status);
	return (eval(right_fds, env, ast->op.right));
}

int			eval_pipeline(int fds[2], t_env env, t_ast *ast)
{
	t_ftlst	*curr;
	/* t_ftvec	*pids; */
	int	p[2];
	int prev_output;

	/* pids = ft_vecnew(16); */

	prev_output = STDIN_FILENO;
	curr = ast->pipeline;

	while (curr->next != NULL)
	{
		pipe(p);

		int pid = fork();
		if (pid == 0)
		{
			g_state.is_child = true;
			dup2(p[FD_WRITE], STDOUT_FILENO);
			if (prev_output != STDIN_FILENO)
			{
				dup2(prev_output, STDIN_FILENO);
				close(prev_output);
			}
			close(p[FD_READ]);
			fds[0] = FD_NONE;
			fds[1] = FD_NONE;
			exit(eval(fds, env, curr->data));
		}
		close(p[FD_WRITE]);
		if (prev_output != STDIN_FILENO)
			close(prev_output);
		prev_output = p[FD_READ];
		curr = curr->next;
	}

	int pid = fork();
	if (pid == 0)
	{
		g_state.is_child = true;
		if (prev_output != STDIN_FILENO)
		{
			dup2(prev_output, STDIN_FILENO);
			close(prev_output);
		}
		/* close(p[FD_WRITE]); */
		fds[0] = FD_NONE;
		fds[1] = FD_NONE;
		exit(eval(fds, env, curr->data));
	}
	g_child_pid = pid;
	close(p[FD_READ]);

	/* int status = 0; */

	waitpid(pid, &pid, 0);
	while (wait(NULL) != -1)
		;
	return (WEXITSTATUS(pid));
}
