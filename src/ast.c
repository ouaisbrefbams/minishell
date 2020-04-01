#include "ast.h"

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

static void		cmd_destroy(t_cmd *cmd)
{
	ft_split_destroy(cmd->argv);
	free(cmd->in);
	free(cmd->out);
}

static void		line_destroy(t_line *line)
{
	ast_destroy(line->left);
	ast_destroy(line->right);
}

void			ast_destroy(t_ast *ast)
{
	if (ast == NULL)
		return ;
	if (ast->tag == TAG_CMD)
		cmd_destroy(&ast->data.cmd);
	else if (ast->tag == TAG_LINE)
		line_destroy(&ast->data.line);
	free(ast);
}
