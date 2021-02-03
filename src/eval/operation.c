/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 15:27:22 by charles           #+#    #+#             */
/*   Updated: 2021/02/03 19:11:54 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

/*
** \brief      Evaluate an operation &&/||/;
** \param fds  Input/output filedescriptor
** \param env  Environment
** \param ast  Operation AST node
** \return     Status code of the left hand side if short circuit,
**             stat code of the right hand side otherwise.
*/

int				eval_operation(int fds[2], t_env env, t_ast *ast)
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
	if (g_state.killed)
		return (status);
	g_state.last_status = status;
	if ((ast->op.sep == TAG_AND && status != 0) ||
		(ast->op.sep == TAG_OR && status == 0))
		return (status);
	return (eval(right_fds, env, ast->op.right));
}

#define PIPES_PREV_OUTPUT 2

/*
** \brief          Evaluate a piped expression (i.e command or parenthesis)
** \param env      Environment
** \param ast      AST of the expression to evaluate
** \param pipes    File descriptor to setup in the child
**                 pipes[0] - read end
**                 pipes[1] - write end
**                 pipes[2] - output of the previous command
** \param is_last  Setup file dscriptors differently if
**                 it's the last expression in the pipeline
** \return         pid of the child process
*/

static pid_t	st_run_piped(
	t_env env, t_ast *ast, int pipes[3], bool is_last)
{
	pid_t	pid;
	int		fds[2];

	if ((pid = fork()) == -1)
		return (EVAL_FATAL);
	if (pid == 0)
	{
		g_state.is_child = true;
		if (!is_last)
			dup2(pipes[FD_WRITE], STDOUT_FILENO);
		if (pipes[PIPES_PREV_OUTPUT] != STDIN_FILENO)
		{
			dup2(pipes[PIPES_PREV_OUTPUT], STDIN_FILENO);
			close(pipes[PIPES_PREV_OUTPUT]);
		}
		if (!is_last)
			close(pipes[FD_READ]);
		fds[0] = FD_NONE;
		fds[1] = FD_NONE;
		exit(eval(fds, env, ast));
	}
	return (pid);
}

/*
** \brief      Evaluate a pipeline
** \param env  Environment
** \param ast  Pipeline AST node
** \return     Status of the last command in the pipeline
*/

int				eval_pipeline(t_env env, t_ast *ast)
{
	t_ftlst	*curr;
	int		pipes[3];
	int		pid;

	pipes[PIPES_PREV_OUTPUT] = STDIN_FILENO;
	pipes[FD_READ] = -1;
	curr = ast->pipeline;
	while (curr->next != NULL)
	{
		if (pipe(pipes) != 0)
			return (EVAL_FATAL);
		st_run_piped(env, curr->data, pipes, false);
		close(pipes[FD_WRITE]);
		if (pipes[PIPES_PREV_OUTPUT] != STDIN_FILENO)
			close(pipes[PIPES_PREV_OUTPUT]);
		pipes[PIPES_PREV_OUTPUT] = pipes[FD_READ];
		curr = curr->next;
	}
	pid = st_run_piped(env, curr->data, pipes, true);
	g_child_pid = pid;
	close(pipes[FD_READ]);
	waitpid(pid, &pid, 0);
	while (wait(NULL) != -1)
		;
	return (WEXITSTATUS(pid));
}
