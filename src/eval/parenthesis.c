/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 20:38:29 by charles           #+#    #+#             */
/*   Updated: 2020/09/15 20:09:13 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

int			wrapped_eval(t_fork_param_args *param)
{
	return (eval(param->fds, param->env, param->ast, NULL, FD_NONE));
}

int			eval_parenthesis(int fds[2], t_env env, t_ast *ast, pid_t *child_pid, int fd_to_close)
{
	int					status;
	t_fork_param_args	param;

	if ((status = redir_extract(&ast->redirs, env, fds)) != 0)
		return (status);
	ast->tag ^= AST_PARENT;
	param.fds[0] = fds[0];
	param.fds[1] = fds[1];
	param.env = env;
	param.ast = ast->parent_ast;
	return (fork_wrap(fds, &param, (t_wrapped_func)wrapped_eval, child_pid, fd_to_close));
}
