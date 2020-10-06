/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 15:58:35 by charles           #+#    #+#             */
/*   Updated: 2020/10/06 13:14:44 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

void	debug_tok_lst(t_tok_lst *tokens)
{
	while (tokens != NULL)
	{
		// FIXME libft for safer correction
		printf("[%#06x] |%s|%s\n", tokens->tag, tokens->content,
				tokens->tag & TAG_STICK ? " STICK" : "");
		tokens = tokens->next;
	}
}

void	debug_tok_lst_line(t_tok_lst *tokens)
{
	while (tokens != NULL)
	{
		// FIXME libft for safer correction
		printf("|%s| ", tokens->content);
		tokens = tokens->next;
	}
}

int		debug_lexer(char *input)
{
	int			status;
	t_tok_lst	*out;

	status = lexer(input, &out);
	if (status != 0)
		return (status);
	debug_tok_lst(out);
	return (status);
}

void print_level(int level)
{
	while (level-- > 0)
		printf("  ");
}

void ast_print(int level, t_ast *ast)
{
	/* printf("%p\n", ast); */
	if (ast == NULL)
		return ;
	if (ast->tag == AST_PARENT)
	{
		print_level(level);
		printf("parent: redir [ ");
		debug_tok_lst_line(ast->redirs);
		printf(" ]\n");
		ast_print(level + 1, ast->parent_ast);
	}
	else if (ast->tag == AST_PIPELINE)
	{
		print_level(level);
		printf("pipeline: {\n");
		t_ftlst *curr = ast->pipeline;
		/* printf("%p\n", curr); */
		/* printf("%p\n", curr->next); */
		/* printf("%p\n", curr->next->next); */

		/* printf("%p\n", curr->data); */
		/* printf("%p\n", curr->next->); */
		while (curr != NULL)
		{
			ast_print(level + 1, (t_ast*)curr->data);
			printf("\n");
			curr = curr->next;
		}
		print_level(level);
		printf(" }\n");
	}
	else if (ast->tag == AST_CMD)
	{
		print_level(level);
		printf("cmd: [ ");
		debug_tok_lst_line(ast->cmd_argv);
		printf(" ] redirs: [");
		debug_tok_lst_line(ast->redirs);
		printf(" ]");
	}
	else if (ast->tag == AST_OP) {
		/* printf("SEP: %d\n", ast->op.sep); */
		/* printf("redirs: ["); */
		/* ft_lstiter(ast->redirs, token_put); */
		/* printf(" ] "); */
		printf("{\n");

		print_level(level);
		printf(" left:\n");
		ast_print(level + 1, ast->op.left);

		printf("\n");
		print_level(level);
		printf(" right:\n");
		ast_print(level + 1, ast->op.right);

		printf("\n");
		print_level(level);
		printf("}\n");
	}
}

int		debug_parser(char *input)
{
	int			status;
	t_tok_lst	*out;

	status = lexer(input, &out);
	if (status != 0)
	{
		printf("Lexer error\n");
		debug_tok_lst(out);
		return (status);
	}
	t_parsed *ret = parse(out);
	if (ret == NULL || ret->syntax_error)
		return (1);
	ast_print(0, ret->ast);
	return (status);
}
