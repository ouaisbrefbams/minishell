/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 18:09:04 by nahaddac          #+#    #+#             */
/*   Updated: 2020/06/19 13:38:23 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   parse.c
** \brief  Parser
*/

#include "parser.h"
#include "lexer.h"


t_ftlst					*push_token(t_ftlst **tokens, t_token *pushed)
{
	t_ftlst *tmp;

	if ((tmp = ft_lstnew(pushed)) == NULL)
		return (NULL);
	ft_lstpush_back(tokens, tmp);
	return (tmp);
}

t_ret					*ret_wrap_ast(t_ast *ast, t_ftlst *rest)
{
	t_ret	*ret;

	if ((ret = malloc(sizeof(t_ret))) == NULL)
		return (NULL);
	ret->unexpected = NULL;
	ret->rest = rest;
	ret->ast = ast;
	return ret;
}

t_ret                   *parse_cmd(t_ftlst *input)
{
	enum e_token_tag    tag;
	t_ast               *ast;

	ast = ast_new(AST_CMD);
	while (input != NULL)
	{
		tag = ((t_token *)input->data)->tag;
		if (tag & TAG_IS_STR)
		{
			push_token(&ast->cmd_argv, input->data);
		}
		else if (tag & TAG_IS_REDIR)
		{
			while(input != NULL)
			{
				push_token(&ast->redirs, input->data);
				if (tag & TAG_IS_STR && tag & TAG_STICK)
					input = input->next;
				else if (tag & TAG_IS_REDIR)
					input = input->next;
				else
					break;
				tag = ((t_token *)input->data)->tag;
			}
		}
		else
			return ret_wrap_ast(ast, input);
		input = input->next;
	}
	return ret_wrap_ast(ast, input);
}

// <cmd>  ::= (<string> | <redir>)+
// <op>   ::= <expr> <sep> <op> | <expr>
// <expr> ::= '(' <op> ')' | <cmd>

t_ret		*parse_op(t_ftlst *input)
{
	t_ast			*ast;
	t_ret			*left_ret;
	t_ret			*right_ret;
	enum e_token_tag sep_tag;

	left_ret = parse_expr(input);
	input = left_ret->rest;

	if (input == NULL || ((t_token*)input->data)->tag == TAG_PARENT_CLOSE)
		return ret_wrap_ast(left_ret->ast, input);

	sep_tag = ((t_token*)input->data)->tag;
	input = input->next;

	right_ret = parse_op(input);

	ast = ast_new(AST_OP);
	ast->op.left = left_ret->ast;
	ast->op.right = right_ret->ast;
	ast->op.sep = sep_tag;
	return ret_wrap_ast(ast, right_ret->rest);
}

t_ret       *parse_expr(t_ftlst *input)
{
    t_ret               *tmp;
    enum e_token_tag    tag;
    tag = ((t_token*)input->data)->tag;
    if (tag == TAG_PARENT_OPEN)
    {
        tmp = parse_op(input->next);
        input = tmp->rest;
        tag = ((t_token*)input->data)->tag;
        if (tag != TAG_PARENT_CLOSE)
            return (NULL);
        input = input->next;
        t_ast *new_ast = ast_new(AST_PARENT);
        new_ast->parent_ast = tmp->ast;
        tmp->ast = new_ast;
		if (tag & TAG_IS_REDIR)
		{
			while(input != NULL)
			{
				push_token(&tmp->ast->redirs, input->data);
				if (tag & TAG_IS_STR && tag & TAG_STICK)
					input = input->next;
				else if (tag & TAG_IS_REDIR)
					input = input->next;
				else
					break;
				tag = ((t_token *)input->data)->tag;
			}
		}
        tmp->rest = input;
        return tmp;
    }
    return parse_cmd(input);
}

t_ret		*parse(t_ftlst *input)
{
	t_ret *ret;
	t_ftlst *in_f;

	/* in_f = input; */
	/* if (input == NULL) */
	/* 	return NULL; */
	/* if (!(ret = malloc(sizeof(t_ret) * 1))) */
	/* 	return (NULL); */
	/* ret->ast = NULL; */
	/* ret->rest = NULL; */
	/* if((ret->unexpected = error_syntax_simple(input)) != NULL) */
	/* 	printf("%s\n", ret->unexpected->content); */
	ret = parse_op(input);
	/* ast_destroy(ret->ast); */
	/* ft_lstdestroy(&ret->rest, (void (*)(void*))token_destroy); */
	return (ret);
}
