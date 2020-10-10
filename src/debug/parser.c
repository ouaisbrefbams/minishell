/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <me@cacharle.xyz>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 07:53:11 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/10 08:06:35 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include "parser.h"

#ifdef MINISHELL_TEST

static void	st_tok_lst_put(t_tok_lst *tokens)
{
	while (tokens != NULL)
	{
		ft_putchar('|');
		ft_putstr(tokens->content);
		ft_putstr("| ");
		tokens = tokens->next;
	}
}

static void	st_print_level(int level)
{
	while (level-- > 0)
		ft_putstr("  ");
}

void		st_ast_print_parent_pipeline(int level, t_ast *ast)
{
	t_ftlst	*curr;

	if (ast->tag == AST_PARENT)
	{
		st_print_level(level);
		ft_putstr("parent: redir [ ");
		st_tok_lst_put(ast->redirs);
		ft_putstr(" ]\n");
		ast_print(level + 1, ast->parent_ast);
	}
	else if (ast->tag == AST_PIPELINE)
	{
		st_print_level(level);
		ft_putstr("pipeline: {\n");
		curr = ast->pipeline;
		while (curr != NULL)
		{
			ast_print(level + 1, (t_ast *)curr->data);
			ft_putstr("\n");
			curr = curr->next;
		}
		st_print_level(level);
		ft_putstr(" }\n");
	}
}

void		ast_print(int level, t_ast *ast)
{
	if (ast->tag == AST_PARENT || ast->tag == AST_PIPELINE)
		st_ast_print_parent_pipeline(level, ast);
	else if (ast->tag == AST_CMD)
	{
		st_print_level(level);
		ft_putstr("cmd: [ ");
		st_tok_lst_put(ast->cmd_argv);
		ft_putstr(" ] redirs: [");
		st_tok_lst_put(ast->redirs);
		ft_putstr(" ]");
	}
	else if (ast->tag == AST_OP)
	{
		ft_putstr("{\n");
		st_print_level(level);
		ft_putstr(" left:\n");
		ast_print(level + 1, ast->op.left);
		ft_putstr("\n");
		st_print_level(level);
		ft_putstr(" right:\n");
		ast_print(level + 1, ast->op.right);
		ft_putstr("\n");
		st_print_level(level);
		ft_putstr("}\n");
	}
}

int			debug_parser(char *input)
{
	int			status;
	t_tok_lst	*out;
	t_parsed	*ret;

	status = lexer(input, &out);
	if (status != 0)
	{
		ft_putendl("Lexer error\n");
		return (status);
	}
	ret = parse(out);
	if (ret == NULL || ret->syntax_error)
		return (1);
	ast_print(0, ret->ast);
	return (status);
}

#endif
