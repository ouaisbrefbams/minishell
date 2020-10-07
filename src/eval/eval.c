/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 20:38:06 by charles           #+#    #+#             */
/*   Updated: 2020/10/07 10:25:39 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

int st_replace(int oldfd, int newfd)
{
	if (oldfd != FD_NONE)
	{
		dup2(oldfd, newfd);
		/* close(oldfd); */
	}
	return 0;
}

/*
** \brief            Wrap a function in a fork
** \param fds        fork read/write file descriptors
** \param passed     param of the wrapped function
** \param wrapped    function to wrap
** \param child_pid  Pointer where to store the child pid
**                   or NULL if the child should be waited
** \return           The child status code or EVAL_FATAL on error
*/

int			fork_wrap(int fds[2], void *passed, t_wrapped_func wrapped)
{
	int		status;
	/* bool	waiting; */
	pid_t	pid;

	if ((pid = fork()) == -1)
		return (EVAL_FATAL);
	if (pid == 0)
	{
		if (st_replace(fds[FD_READ], STDIN_FILENO) != 0)
			exit(EXIT_FAILURE);
		if (st_replace(fds[FD_WRITE], STDOUT_FILENO) != 0)
			exit(EXIT_FAILURE);
		/* if (fd_to_close != FD_NONE) */
		/* 	close(fd_to_close); */
		if ((status = wrapped(passed)) == EVAL_FATAL)
			exit(EXIT_FAILURE);
		exit(status);
	}
	g_child_pid = pid;
	waitpid(pid, &pid, 0);
	/* close(fds[FD_WRITE]); */
	return (WEXITSTATUS(pid));
}

/*
** \brief    Evaluate and AST
** \return   The last command status or EVAL_FATAL on error
*/

int			eval(int fds[2], t_env env, t_ast *ast)
{
	if (ast->tag == AST_OP)
		return (eval_operation(fds, env, ast));
	if (ast->tag == AST_CMD)
		return (eval_cmd(fds, env, ast));
	if (ast->tag == AST_PIPELINE)
		return (eval_pipeline(fds, env, ast));
	if (ast->tag == AST_PARENT)
		return (eval_parenthesis(fds, env, ast));
	return (EVAL_FATAL);
}
