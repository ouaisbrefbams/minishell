/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:42 by charles           #+#    #+#             */
/*   Updated: 2020/08/27 20:31:48 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file    ast.c
** \brief   AST functions
*/

#include "parser.h"

/*
** \brief       Create a new AST node according to `tag`
** \param tag   Tag of node
** \return      Created node or NULL on error
*/

t_ast			*ast_new(enum e_ast tag)
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
	if (ast->tag == AST_CMD)
	{
		tok_lst_destroy(&ast->cmd_argv, free);
		tok_lst_destroy(&ast->redirs, free);
	}
	else if (ast->tag == AST_OP)
	{
		ast_destroy(ast->op.left);
		ast_destroy(ast->op.right);
	}
	else if (ast->tag == AST_PARENT)
		ast_destroy(ast->parent_ast);
	free(ast);
}
