/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 18:09:04 by nahaddac          #+#    #+#             */
/*   Updated: 2020/09/16 20:19:05 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   parse.c
** \brief  Parser
*/

#include "parser.h"

/*
** push redirection token
** push while token is str and stick
** push last str token
*/

t_parsed	*parse_redir(t_tok_lst *input, t_tok_lst **redirs)
{
	tok_lst_push_back(redirs, tok_lst_uncons(&input));
	if (input == NULL)
		return (parsed_error("syntax error near unexpected token `newline'\n"));
	// FIXME big condition could be avoided with lexer not putting STICK if the next tokens isn't a string
	while (input != NULL
			&& input->tag & TAG_IS_STR && input->tag & TAG_STICK
			&& input->next != NULL && input->next->tag & TAG_IS_STR)
		tok_lst_push_back(redirs, tok_lst_uncons(&input));
	if (input == NULL)
		return (NULL);
	if (!(input->tag & TAG_IS_STR))
		return (parsed_error("syntax error near unexpected token `%s'\n", input->content));
	tok_lst_push_back(redirs, tok_lst_uncons(&input));
	return (parsed_new(NULL, input));
}

t_parsed	*parse_cmd(t_tok_lst *input)
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
			tok_lst_push_back(&ast->cmd_argv, tok_lst_uncons(&input));
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
	return (parsed_new(ast, input));
}

/*
** parse and operation (| ; && ||)
*/

t_parsed	*parse_op(t_tok_lst *input)
{
	t_ast		*ast;
	t_parsed	*left;
	t_parsed	*right;
	enum e_tok	sep_tag;

	left = parse_expr(input);
	if (left == NULL || left->syntax_error)
		return (left);
	input = left->rest;
	if (input == NULL || input->tag & TAG_PARENT_CLOSE)
		return (left);
	sep_tag = input->tag;
	if (!(sep_tag & TAG_IS_SEP))
		return (parsed_error("syntax error near unexpected token `%s'\n", input->content));
	ft_lstpop_front((t_ftlst**)&input, free);
	if (input == NULL && sep_tag == TAG_END)
		return (left);
	if (input == NULL)
		return (parsed_error("syntax error expected token\n"));
	right = parse_op(input);
	if (right == NULL  || right->syntax_error)
		return (right);
	if ((ast = ast_new(AST_OP)) == NULL)
		return (NULL);
	ast->op.left = left->ast;
	ast->op.right = right->ast;
	ast->op.sep = sep_tag;
	free(left);
	left = parsed_new(ast, right->rest);
	free(right);
	return (left);
}

/*
** try to parse parenthesis, if fail parse a command.
** parenthesis can be followed by redirections
*/

t_parsed	*parse_expr(t_tok_lst *input)
{
    t_parsed	*parsed;
    t_parsed	*tmp;
	t_ast		*ast;

    if (input->tag & TAG_PARENT_OPEN)
    {
        if (!(parsed = parse_op(input->next)) || parsed->syntax_error)
			return (parsed);
        input = parsed->rest;
		if (input == NULL || !(input->tag & TAG_PARENT_CLOSE))
			return (parsed_error("syntax error expected token\n"));
        input = input->next;
        if ((ast = ast_new(AST_PARENT)) == NULL)
			return (NULL);
        ast->parent_ast = parsed->ast;
        parsed->ast = ast;
		while (input != NULL && input->tag & TAG_IS_REDIR)
		{
			tmp = parse_redir(input, &parsed->ast->redirs);
			if (tmp == NULL || tmp->syntax_error)
				return (tmp);
			input = tmp->rest;
		}
        parsed->rest = input;
        return (parsed);
    }
    return (parse_cmd(input));
}

t_parsed	*parse(t_tok_lst *input)
{
	if (input == NULL)
		return (NULL);
	return (parse_op(input));
}
