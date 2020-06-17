/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:42 by charles           #+#    #+#             */
/*   Updated: 2020/06/17 15:48:56 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file    ast.c
** \brief   AST functions
*/

#include "ast.h"

/*
** \brief       Create a new AST node according to `tag`
** \param tag   Tag of node
** \return      Created node or NULL on error
*/

t_ast			*ast_new(enum e_ast_tag tag)
{
	t_ast	*ast;

	if ((ast = (t_ast*)malloc(sizeof(t_ast))) == NULL)
		return (NULL);
	ast->tag = tag;
	ast->redirs = NULL;
	ast->op.left = NULL;
	ast->op.right = NULL;
	ast->cmd_argv = NULL;
	return (ast);
}

/*
** \brief      Destroy an AST node and all his child nodes
** \param ast  AST to destroy
*/

void			ast_destroy(t_ast *ast)
{
	if (ast == NULL)
		return ;
	ft_lstdestroy(&ast->cmd_argv, (void (*)(void*))token_destroy);
	if (ast->tag == AST_CMD)
	{
		ft_lstdestroy(&ast->cmd_argv, (void (*)(void*))token_destroy);
		ft_lstdestroy(&ast->redirs, (void (*)(void*))token_destroy);
	}
	else if (ast->tag == AST_OP)
	{
		ast_destroy(ast->op.left);
		ast_destroy(ast->op.right);
	}
	free(ast);
}
