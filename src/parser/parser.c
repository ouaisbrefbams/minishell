/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 18:09:04 by nahaddac          #+#    #+#             */
/*   Updated: 2020/08/27 21:12:21 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   parse.c
** \brief  Parser
*/

#include "parser.h"

t_parsed					*parse_redir(t_tok_lst *input, t_tok_lst **redirs)
{
	tok_lst_push_back(redirs, tok_lst_pop_front(&input));
	/* input = input->next; */
	if (input == NULL)
		return (parsed_error("syntax error near unexpected token `newline'\n"));
	while(input != NULL
			&& input->next != NULL
			&& input->next->tag & TAG_IS_STR
			&& input->tag & TAG_IS_STR && input->tag & TAG_STICK)
	{
		tok_lst_push_back(redirs, tok_lst_pop_front(&input));
		/* input = input->next; */
	}
	if (input != NULL && !(input->tag & TAG_IS_STR))
		return (parsed_error("syntax error near unexpected token `%s'\n", input->content));
	tok_lst_push_back(redirs, tok_lst_pop_front(&input));
	return (parsed_new(NULL, input));
}

t_parsed                   *parse_cmd(t_tok_lst *input)
{
	t_ast		*ast;
	t_parsed	*tmp;

	if (input->tag & TAG_IS_SEP)
		return (parsed_error("syntax error near unexpected token `%s'\n", input->content));
	if ((ast = ast_new(AST_CMD)) == NULL)
		return (NULL);
	while (input != NULL)
	{
		if (input->tag & TAG_IS_STR)
			tok_lst_push_back(&ast->cmd_argv, tok_lst_pop_front(&input));
		else if (input->tag & TAG_IS_REDIR)
		{
			tmp = parse_redir(input, &ast->redirs);
			if (tmp == NULL || tmp->syntax_error)
				return (tmp);
			input = tmp->rest;
		}
		else
			break;
	}
	return parsed_new(ast, input);
}

// <cmd>  ::= (<string> | <redir>)+
// <op>   ::= <expr> <sep> <op> | <expr>
// <expr> ::= '(' <op> ')' | <cmd>

t_parsed		*parse_op(t_tok_lst *input)
{
	t_ast		*ast;
	t_parsed	*left_parsed;
	t_parsed	*right_parsed;
	enum e_tok	tag;

	left_parsed = parse_expr(input);
	if (left_parsed == NULL || left_parsed->syntax_error)
		return left_parsed;
	input = left_parsed->rest;
	if (input == NULL || input->tag & TAG_PARENT_CLOSE)
		return parsed_new(left_parsed->ast, input);

	tag = input->tag;
	if (!(tag & TAG_IS_SEP))
		return (parsed_error("syntax error near unexpected token `%s'\n", input->content));
	input = input->next;
	if (input == NULL)
		return (parsed_error("syntax error expected token\n"));
	right_parsed = parse_op(input);
	if (right_parsed == NULL  || right_parsed->syntax_error)
		return right_parsed;
	ast = ast_new(AST_OP);
	ast->op.left = left_parsed->ast;
	ast->op.right = right_parsed->ast;
	ast->op.sep = tag;
	return parsed_new(ast, right_parsed->rest);
}

t_parsed       *parse_expr(t_tok_lst *input)
{
    t_parsed	*tmp;
    enum e_tok	tag;
	t_ast		*new_ast;

    tag = input->tag;
    if (tag & TAG_PARENT_OPEN)
    {
        if (!(tmp = parse_op(input->next)) || tmp->syntax_error)
			return tmp;
        input = tmp->rest;
		if (input == NULL)
			return (parsed_error("syntax error expected token\n"));
		tag = input->tag;
        if (!(tag & TAG_PARENT_CLOSE))
			return (parsed_error("syntax error expected token\n"));
        input = input->next;
        new_ast = ast_new(AST_PARENT);
        new_ast->parent_ast = tmp->ast;
        tmp->ast = new_ast;
		if (input == NULL)
			return tmp;
		// could reuse parse_redir instead
		tag = input->tag;
		while (tag & TAG_IS_REDIR)
		{
			while(input != NULL)
			{
				tag = input->tag;
				tok_lst_push_back(&tmp->ast->redirs, tok_lst_pop_front(&input));
				if (tag & TAG_IS_STR && tag & TAG_STICK)
					input = input->next;
				else if (tag & TAG_IS_REDIR)
					input = input->next;
				else
					break;
			}
			if (input == NULL)
				break;
			input = input->next;
			tag = input->tag;
		}
        tmp->rest = input;
        return tmp;
    }
    return parse_cmd(input);
}

t_parsed		*parse(t_tok_lst *input)
{
	if (input == NULL)
		return NULL;
	return (parse_op(input));
}
