/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 15:27:22 by charles           #+#    #+#             */
/*   Updated: 2020/09/12 17:00:11 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

int			eval_op(int fds[2], t_env env, t_path path, t_ast *ast)
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
		eval_forked(left_fds, env, path, ast->op.left, &left_pid);
		eval_forked(right_fds, env, path, ast->op.right, &right_pid);

		pid_t finished;
		finished = wait(NULL);
		close(p[FD_READ]);
		close(p[FD_WRITE]);
		if (finished == left_pid)
		{
			/* waitpid(right_pid, &right_pid, 0); */
		}
		else if (finished == right_pid)
		{
			kill(left_pid, SIGKILL);
			/* waitpid(left_pid, &left_pid, 0); */
		}
		return (0);
	}
	if ((status = eval(left_fds, env, path, ast->op.left)) == EVAL_FATAL)
		return (EVAL_FATAL);
	g_last_status = status;
	if ((ast->op.sep == TAG_AND && status != 0) ||
		(ast->op.sep == TAG_PIPE && status != 0) ||
		(ast->op.sep == TAG_OR && status == 0))
		return (status);
	return (eval(right_fds, env, path, ast->op.right));
}

int			wrapped_eval(void *void_param)
{
	t_fork_param_args	*param;

	param = void_param;
	return (eval(param->fds, param->env, param->path, param->ast));
}

int			eval_parent(int fds[2], t_env env, t_path path, t_ast *ast)
{
	int					status;

	if ((status = redir_extract(&ast->redirs, env, fds)) != 0)
		return (status);
	ast->tag ^= AST_PARENT;
	return (eval_forked(fds, env, path, ast->parent_ast, NULL));
}

int			eval_forked(int fds[2], t_env env, t_path path, t_ast *ast, pid_t *child_pid)
{
	t_fork_param_args	param;

	param.fds[0] = fds[0];
	param.fds[1] = fds[1];
	param.env = env;
	param.path = path;
	param.ast = ast;
	return (fork_wrap(fds, &param, wrapped_eval, child_pid));
}

int			eval(int fds[2], t_env env, t_path path, t_ast *ast)
{
	if (ast->tag == AST_PARENT)
		return (eval_parent(fds, env, path, ast));
	if (ast->tag == AST_OP)
		return (eval_op(fds, env, path, ast));
	if (ast->tag == AST_CMD)
		return (eval_cmd(fds, env, path, ast));
	return (EVAL_FATAL);
}
