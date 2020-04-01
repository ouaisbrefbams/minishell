/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:42 by charles           #+#    #+#             */
/*   Updated: 2020/04/01 17:05:44 by charles          ###   ########.fr       */
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
** \param data  Pointer to node data (t_cmd or t_line)
**              which will be copied in ast::data union
** \return      Created node or NULL on error
*/

t_ast			*ast_new(t_ast_tag tag, void *data)
{
	t_ast	*ast;

	if (data == NULL)
		return (NULL);
	if ((ast = (t_ast*)malloc(sizeof(t_ast))) == NULL)
		return (NULL);
	ft_bzero(ast, sizeof(t_ast));
	ast->tag = tag;
	if (tag == TAG_CMD)
		ft_memcpy(&ast->data.cmd, (t_cmd*)data, sizeof(t_cmd));
	else if (tag == TAG_LINE)
		ft_memcpy(&ast->data.line, (t_line*)data, sizeof(t_line));
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
	if (ast->tag == TAG_CMD)
	{
		ft_split_destroy(ast->data.cmd.argv);
		free(ast->data.cmd.in);
		free(ast->data.cmd.out);
	}
	else if (ast->tag == TAG_LINE)
	{
		ast_destroy(ast->data.line.left);
		ast_destroy(ast->data.line.right);
	}
	free(ast);
}
