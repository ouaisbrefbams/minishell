/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 15:58:35 by charles           #+#    #+#             */
/*   Updated: 2020/09/16 16:18:11 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

#include "lexer.h"

void	debug_tok_lst(t_tok_lst *tokens)
{
	while (tokens != NULL)
	{
		// FIXME libft for safer correction
		printf("[%#06x] |%s|%s\n", tokens->tag, tokens->content, tokens->tag & TAG_STICK ? " STICK" : "");
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

/* void token_debug(void *v) */
/* { */
/* 	t_token *t; */
/*  */
/* 	t= v; */
/* 	printf("[%4d %d] (%s)\n", t->tag, !!(t->tag & TAG_STICK), t->content); */
/* } */
/*  */
/* void token_put(void *v) */
/* { */
/* 	t_token *t; */
/*  */
/* 	t= v; */
/* 	printf("%s ", t->content); */
/* } */
/*  */
/* void print_level(int level) */
/* { */
/* 	while (level-- > 0) */
/* 		printf("  "); */
/* } */
/*  */
/* void ast_print(int level, t_ast *ast) */
/* { */
/* 	if (ast->tag == AST_PARENT) */
/* 	{ */
/* 		print_level(level); */
/* 		printf("parent: redir [ "); */
/* 		ft_lstiter(ast->redirs, token_put); */
/* 		printf(" ]\n"); */
/* 		ast_print(level + 1, ast->parent_ast); */
/* 	} */
/* 	else if (ast->tag == AST_CMD) */
/* 	{ */
/* 		print_level(level); */
/* 		printf("cmd: [ "); */
/* 		ft_lstiter(ast->cmd_argv, token_put); */
/* 		printf(" ] redirs: ["); */
/* 		ft_lstiter(ast->redirs, token_put); */
/* 		printf(" ]"); */
/* 	} */
/* 	else if (ast->tag == AST_OP) { */
/* 		#<{(| printf("SEP: %d\n", ast->op.sep); |)}># */
/* 		print_level(level); */
/* 		#<{(| printf("redirs: ["); |)}># */
/* 		#<{(| ft_lstiter(ast->redirs, token_put); |)}># */
/* 		#<{(| printf(" ] "); |)}># */
/* 		printf("{\n"); */
/*  */
/* 		print_level(level); */
/* 		printf(" left:\n"); */
/* 		ast_print(level + 1, ast->op.left); */
/*  */
/* 		printf("\n"); */
/* 		print_level(level); */
/* 		printf(" right:\n"); */
/* 		ast_print(level + 1, ast->op.right); */
/*  */
/* 		printf("\n"); */
/* 		print_level(level); */
/* 		printf("}\n"); */
/* 	} */
/* } */
