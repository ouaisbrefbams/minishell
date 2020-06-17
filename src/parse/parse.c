/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 18:09:04 by nahaddac          #+#    #+#             */
/*   Updated: 2020/06/17 20:39:25 by charles          ###   ########.fr       */
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

// <cmd> ::= ...
// <op>  ::= <cmd> <sep> <op> | <op>

t_ret		*parse_op(t_ftlst *input)
{
	t_ast			*ast;
	t_ret			*left_ret;
	t_ret			*right_ret;
	enum e_token_tag sep_tag;

	left_ret = parse_cmd(input);
	input = left_ret->rest;

	if (input == NULL)//((t_token*)input->data)->tag & TAG_IS_SEP)
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

t_ret		*parse(t_ftlst *input)
{
	return parse_op(input);
}

////////////////////////////////////////////////////////////////
// saved
////////////////////////////////////////////////////////////////

/* t_ast					*cmd_push_ast(t_ftlst *rest) */
/* { */
/* 	enum e_token_tag 	tag; */
/* 	t_ast 				*ast; */
/*  */
/* 	ast = NULL; */
/* 	while (rest != NULL) */
/* 	{ */
/* 		tag = ((t_token *)rest->data)->tag; */
/* 		if (parse_cmd_str_true_false(tag)) */
/* 		{ */
/* 			ast = push_cmd(ast, rest); */
/* 		} */
/* 		else if (parse_redir_true_false(tag)) */
/* 		{ */
/* 			while(rest != NULL) */
/* 			{ */
/* 				ast = push_redir(ast, rest); */
/* 				if (tag & TAG_IS_STR && tag & TAG_STICK) */
/* 					rest = rest->next; */
/* 				else if (tag & TAG_IS_REDIR) */
/* 					rest = rest->next; */
/* 				else */
/* 					break; */
/* 				tag = ((t_token *)rest->data)->tag; */
/* 			} */
/* 		} */
/* 		rest = rest->next; */
/* 	} */
/* 	return ast; */
/* } */
/*  */
/* t_ret					*parse(t_ftlst *input) */
/* { */
/* 	t_ret 				*ret; */
/* 	t_ret 				*first; */
/* 	t_ast 				*new_ast; */
/*  */
/* 	if(!(ret = malloc(sizeof(t_ret) * 1))) */
/* 		return(NULL); */
/* 	ret->rest = input; */
/* 	ret->ast = NULL; */
/* 	ret->unexpected = NULL; */
/* 	first = ret; */
/*  */
/* 	if(!(new_ast = malloc(sizeof(t_ast) * 1))) */
/* 		return(0); */
/* 	new_ast = cmd_push_ast(ret->rest); */
/* 	printf("%s\n",((t_token *)ret->rest->data)->content); */
/* 	if (ret->ast) */
/* 	{ */
/* 		while(ret->ast->cmd_argv != NULL) */
/* 	    { */
/* 	 	 	printf("[%s]\n", ((t_token *)ret->ast->cmd_argv->data)->content); */
/* 	      	ret->ast->cmd_argv = ret->ast->cmd_argv->next; */
/* 	    } */
/* 		while(ret->ast->redirs != NULL) */
/* 		{ */
/* 		 	printf("[%s]\n", ((t_token *)ret->ast->redirs->data)->content); */
/* 			ret->ast->redirs = ret->ast->redirs->next; */
/* 		} */
/* 	} */
/* 	ast_destroy(ret->ast); */
/* 	ft_lstdestroy(&ret->rest, (void (*)(void*))token_destroy); */
/* 	return first; */
/* } */
