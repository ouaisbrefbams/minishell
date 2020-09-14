/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 20:38:29 by charles           #+#    #+#             */
/*   Updated: 2020/09/14 19:40:12 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

int			wrapped_eval(void *void_param)
{
	t_fork_param_args	*param;

	param = void_param;
	return (eval(param->fds, param->env, param->ast, NULL));
}

int			eval_parenthesis(int fds[2], t_env env, t_ast *ast)
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
	return (fork_wrap(fds, &param, wrapped_eval, NULL));
}
