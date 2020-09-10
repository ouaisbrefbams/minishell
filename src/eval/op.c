/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 15:27:22 by charles           #+#    #+#             */
/*   Updated: 2020/09/10 14:15:08 by charles          ###   ########.fr       */
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
	}
	if ((status = eval(left_fds, env, path, ast->op.left)) == -1)
		return (-1);
	if ((ast->op.sep == TAG_AND && status != 0) ||
		(ast->op.sep == TAG_PIPE && status != 0) ||
		(ast->op.sep == TAG_OR && status == 0))
		return (status);
	return (eval(right_fds, env, path, ast->op.right));
}

int			wrapped_eval(void *void_param)
{
	t_fork_param_parent	*param;

	param = void_param;
	return (eval(param->fds, param->env, param->path, param->ast));
}

int			eval_parent(int fds[2], t_env env, t_path path, t_ast *ast)
{
	t_fork_param_parent	param;

	if (!redir_extract(&ast->redirs, env, fds))
		return (ERR_FATAL);
	param.fds[0] = fds[0];
	param.fds[1] = fds[1];
	param.env = env;
	param.path = path;
	ast->tag ^= AST_PARENT;
	param.ast = ast->parent_ast;
	return (fork_wrap(fds, &param, wrapped_eval));
}

int			eval(int fds[2], t_env env, t_path path, t_ast *ast)
{
	if (ast->tag == AST_PARENT)
		return (eval_parent(fds, env, path, ast));
	if (ast->tag == AST_OP)
		return (eval_op(fds, env, path, ast));
	if (ast->tag == AST_CMD)
		return (eval_cmd(fds, env, path, ast));
	return (ERR_FATAL);
}
