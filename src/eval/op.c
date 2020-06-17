/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 15:27:22 by charles           #+#    #+#             */
/*   Updated: 2020/06/17 17:03:48 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

// TODO: add parent tag on first operation of parent to fork
int			eval_op(int fds[2], t_env env, t_path path, t_op *op)
{
	int	status;
	int	left_fds[2];
	int	right_fds[2];
	int p[2];

	left_fds[FDS_READ] = fds[FDS_READ];
	left_fds[FDS_WRITE] = MS_NO_FD;
	right_fds[FDS_READ] = MS_NO_FD;
	right_fds[FDS_WRITE] = fds[FDS_WRITE];
	if (op->sep == TAG_PIPE)
	{
		pipe(p);
		left_fds[FDS_WRITE] = p[FDS_WRITE];
		right_fds[FDS_READ] = p[FDS_READ];
	}
	if ((status = eval(left_fds, env, path, op->left)) == -1)
		return (-1);
	if ((op->sep == TAG_AND && status != 0) ||
		(op->sep == TAG_PIPE && status != 0) ||
		(op->sep == TAG_OR && status == 0))
		return (status);
	return (eval(right_fds, env, path, op->right));
}

int			eval(int fds[2], t_env env, t_path path, t_ast *ast)
{
	if (ast->tag == AST_OP)
		return (eval_op(fds, env, path, &ast->op));
	if (ast->tag == AST_CMD)
		return (eval_cmd(fds, env, path, ast));
	return (-1);
}
