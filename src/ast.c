/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:42 by charles           #+#    #+#             */
/*   Updated: 2020/06/15 13:01:20 by charles          ###   ########.fr       */
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
	ast->line.left = NULL;
	ast->line.right = NULL;
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
	else if (ast->tag == AST_LINE)
	{
		ast_destroy(ast->line.left);
		ast_destroy(ast->line.right);
	}
	free(ast);
}
