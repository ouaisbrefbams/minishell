/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 20:38:06 by charles           #+#    #+#             */
/*   Updated: 2020/09/14 19:39:33 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

/*
** \brief            Wrap a function in a fork
** \param fds        fork read/write file descriptors
** \param passed     param of the wrapped function
** \param wrapped    function to wrap
** \param child_pid  Pointer where to store the child pid
**                   or NULL if the child should be waited
** \return           The child status code or EVAL_FATAL on error
*/

int			fork_wrap(
	int fds[2],
	void *passed,
	int (*wrapped)(void *param),
	pid_t *child_pid
)
{
	int		status;
	bool	waiting;
	pid_t	pid;

	waiting = child_pid == NULL;
	if (waiting)
		child_pid = &pid;
	if ((*child_pid = fork()) == -1)
		return (EVAL_FATAL);
	if (*child_pid == 0)
	{
		if ((fds[FD_READ] != FD_NONE && dup2(fds[FD_READ], STDIN_FILENO) == -1) ||
			(fds[FD_WRITE] != FD_NONE && dup2(fds[FD_WRITE], STDOUT_FILENO) == -1))
			exit(EXIT_FAILURE);
		if ((status = wrapped(passed)) == EVAL_FATAL)
			exit(EXIT_FAILURE);
		exit(status);
	}
	g_child_pid = *child_pid;
	if (waiting)
	{
		waitpid(*child_pid, child_pid, 0);
		close(fds[FD_WRITE]);
		return (WEXITSTATUS(*child_pid));
	}
	return (0);
}

/*
** \brief    Evaluate and AST
** \return   The last command status or EVAL_FATAL on error
*/

int			eval(int fds[2], t_env env, t_ast *ast, pid_t *child_pid)
{
	if (ast->tag == AST_PARENT)
		return (eval_parenthesis(fds, env, ast));
	if (ast->tag == AST_OP)
		return (eval_operation(fds, env, ast));
	if (ast->tag == AST_CMD)
		return (eval_cmd(fds, env, ast, child_pid));
	return (EVAL_FATAL);
}
