/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 20:38:29 by charles           #+#    #+#             */
/*   Updated: 2020/10/10 12:44:59 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

int			wrapped_eval(t_fork_param_args *param)
{
	return (eval(param->fds, param->env, param->ast));
}

/*
** \brief      Evaluate a parenthesized expression
**             Extract parenthesis redirection and evaluate
**             the expression contained in the parenthesis in a fork
** \param fds  Input/output filedescriptor
** \param env  Environment
** \param ast  Parenthesis AST node
** \return     Expression status code
*/

int			eval_parenthesis(int fds[2], t_env env, t_ast *ast)
{
	int					status;
	t_fork_param_args	param;

	if ((status = redir_extract(&ast->redirs, env, fds)) != 0)
		return (status);
	param.fds[0] = fds[0];
	param.fds[1] = fds[1];
	param.env = env;
	param.ast = ast->parent_ast;
	return (fork_wrap(fds, &param, (t_wrapped_func)wrapped_eval));
}
