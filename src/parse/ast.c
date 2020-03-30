/**
** \file   ast.c
** \brief  AST manipulation
*/

#include "ms_parse.h"

/**
** \brief      Create a new AST node with default values
** \param tag  Tag of the node
** \return     The allocated node
*/

t_ast	*ms_ast_new(t_tag tag)
{
	t_ast	*ast;

	if ((ast = (t_ast*)malloc(sizeof(t_ast))) == NULL)
		return (NULL);
	ast->tag = tag;
	ast->content = NULL;
	ast->children_num = 0;
	ast->children = NULL;
	return (ast);
}

/**
** \brief      Destroy an allocated AST
** \warning    Assumes that `content`, `children` and the node itself have been malloc'd
** \param ast  AST to destroy
*/

void	ms_ast_destroy(t_ast *ast)
{
	int	i;

	if (ast == NULL)
		return ;
	i = -1;
	while (++i < ast->children_num)
		ms_ast_destroy(ast->children[i]);
	free(ast->children);
	free(ast->content);
	free(ast);
}

/**
** \brief      Iterate over an AST node's childs
** \param f    Function applied to each child, take `arg` has his first argument.
** \param arg  Pointer that will be passed to `f`, to keep information between iterations
*/

void	ms_ast_iter(
	t_ast *ast,
	void (*f)(void *f_arg, t_ast *children),
	void *arg)
{
	int	i;

	i = -1;
	while (++i < ast->children_num)
		f(arg, ast->children[i]);
}
