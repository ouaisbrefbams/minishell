/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 18:09:04 by nahaddac          #+#    #+#             */
/*   Updated: 2020/07/17 16:12:53 by nahaddac         ###   ########.fr       */
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
	ret->syntax_error = false;
	ret->rest = rest;
	ret->ast = ast;
	return ret;
}

t_ret					*parse_redir(t_ftlst *input, t_ftlst **redirs)
{
	enum e_token_tag    tag;
	t_ret				*tmp;
	char 				*error;

	push_token(redirs, input->data);
	input = input->next;
	tag = ((t_token *)input->data)->tag;
	while(input != NULL
			&& input->next != NULL
			&& (((t_token*)input->next->data)->tag & TAG_IS_STR)
			&& tag & TAG_IS_STR && tag & TAG_STICK)
	{
		push_token(redirs, input->data);
		input = input->next;
		tag = ((t_token *)input->data)->tag;
	}
	if (!(tag & TAG_IS_STR))
	{
		error = ft_strjoin3(g_basename,":  syntax error unexpected token `", ((t_token *)input->data)->content);
		error = ft_strjoin(error, "\'\n");
		ft_putstr_fd(error, 2);
		tmp = ret_wrap_ast(NULL, NULL);
		tmp->syntax_error = true;
		return tmp;
	}
	push_token(redirs, input->data);
	return (ret_wrap_ast(NULL, input));
}

t_ret                   *parse_cmd(t_ftlst *input)
{
	enum e_token_tag    tag;
	t_ast               *ast;
	char 				*error;
	t_ret				*tmp;

	tag = ((t_token *)input->data)->tag;
	if (tag & TAG_IS_SEP)
	{
		error = ft_strjoin3(g_basename,":  syntax error unexpected token `", ((t_token *)input->data)->content);
		error = ft_strjoin(error, "\'\n");
		ft_putstr_fd(error, 2);
		tmp = ret_wrap_ast(NULL, NULL);
		tmp->syntax_error = true;
		return tmp;
	}
	ast = ast_new(AST_CMD);
	while (input != NULL)
	{
		tag = ((t_token *)input->data)->tag;
		if (tag & TAG_IS_STR)
			push_token(&ast->cmd_argv, input->data);
		else if (tag & TAG_IS_REDIR)
		{
			tmp = parse_redir(input, &ast->redirs);
			if (tmp->syntax_error || tmp == NULL)
				return tmp;
			input = tmp->rest;
		}
		else
			break;
		if (input == NULL)
			break;
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
	enum e_token_tag tag;
	t_ret 			*tmp;
	char 			*error;

	left_ret = parse_expr(input);
	if (left_ret == NULL || left_ret->syntax_error)
		return left_ret;
	input = left_ret->rest;
	if (input == NULL || ((t_token*)input->data)->tag & TAG_PARENT_CLOSE)
		return ret_wrap_ast(left_ret->ast, input);

	tag = ((t_token*)input->data)->tag;
	if (!(tag & TAG_IS_SEP))
	{
		error = ft_strjoin3(g_basename,":  syntax error near unexpected token `", ((t_token *)input->data)->content);
		error = ft_strjoin(error, "\'\n");
		ft_putstr_fd(error, 2);
		tmp = ret_wrap_ast(NULL, NULL);
		tmp->syntax_error = true;
		return tmp;
	}
	input = input->next;
	if(input == NULL)
	{
		error = ft_strjoin(g_basename,":  syntax error expected token ");
		error = ft_strjoin(error, "\n");
		ft_putstr_fd(error, 2);
		tmp = ret_wrap_ast(NULL, NULL);
		tmp->syntax_error = true;
		return tmp;
	}
	right_ret = parse_op(input);
	if (right_ret == NULL  || right_ret->syntax_error)
		return right_ret;
	ast = ast_new(AST_OP);
	ast->op.left = left_ret->ast;
	ast->op.right = right_ret->ast;
	ast->op.sep = tag;
	return ret_wrap_ast(ast, right_ret->rest);
}

t_ret       *parse_expr(t_ftlst *input)
{
    t_ret               *tmp;
    enum e_token_tag    tag;
	t_ast 				*new_ast;
	char 				*error;

    tag = ((t_token*)input->data)->tag;
    if (tag & TAG_PARENT_OPEN)
    {
        if (!(tmp = parse_op(input->next)) || tmp->syntax_error)
			return tmp;
        input = tmp->rest;
		if (input == NULL)
		{
			error = ft_strjoin(g_basename,":  syntax error expected token");
			ft_putstr_fd(error, 2);
			tmp->syntax_error = true;
			return (tmp);
		}
		tag = ((t_token*)input->data)->tag;
        if (!(tag & TAG_PARENT_CLOSE) )
		{
			error = ft_strjoin(g_basename,":  syntax error expected token `)''");
			ft_putstr_fd(error, 2);
			tmp->syntax_error = TRUE;
			return (tmp);
		}
		// if (tag & TAG_IS_SEP)
		// {
		// 	error = ft_strjoin(g_basename,":  syntax error expected token `)''");
		// 	ft_putstr_fd(error, 2);
		// 	tmp->syntax_error = TRUE;
		// 	return (tmp);
		// }
        input = input->next;
        new_ast = ast_new(AST_PARENT);
        new_ast->parent_ast = tmp->ast;
        tmp->ast = new_ast;
		if (input == NULL)
			return tmp;
		// could reuse parse_redir instead
		tag = ((t_token*)input->data)->tag;
		while (tag & TAG_IS_REDIR)
		{
			while(input != NULL)
			{
				tag = ((t_token *)input->data)->tag;
				push_token(&tmp->ast->redirs, input->data);
				if (tag & TAG_IS_STR && tag & TAG_STICK)
					input = input->next;
				else if (tag & TAG_IS_REDIR)
					input = input->next;
				else
					break;
			}
			input = input->next;
			if (input == NULL)
				break;
			tag = ((t_token*)input->data)->tag;
		}
        tmp->rest = input;
        return tmp;
    }
    return parse_cmd(input);
}

t_ret		*parse(t_ftlst *input)
{
	t_ret *ret;

	if (input == NULL)
		return NULL;
	if (!(ret = malloc(sizeof(t_ret) * 1)))
		return (NULL);
	ret->ast = NULL;
	ret->rest = NULL;
	ret->syntax_error = false;
	// ret = error_syntax_simple(input, ret);
	// if (ret->syntax_error == TRUE)
	// 	return ret;
	ret = parse_op(input);
	return (ret);
}
