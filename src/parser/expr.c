/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <me@cacharle.xyz>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 08:42:24 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/10 18:28:02 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
** push redirection token
** push while token is str and stick
** push last str token
*/

/*
** \brief         Push tokens which bellong to a redirection in a token list
** \param input   Tokens where to extract the redirection tokens
** \param redirs  List where to push the redirections tokens
** \return        Parsed struct with the unconsumed input
*/

t_parsed		*parse_redir(t_tok_lst *input, t_tok_lst **redirs)
{
	t_parsed	*ret;

	tok_lst_push_back(redirs, tok_lst_uncons(&input));
	while (input != NULL && input->tag & TAG_IS_STR && input->tag & TAG_STICK &&
			input->next != NULL && input->next->tag & TAG_IS_STR)
		tok_lst_push_back(redirs, tok_lst_uncons(&input));
	if (input == NULL)
		return (parsed_unexpected("newline"));
	if (!(input->tag & TAG_IS_STR))
	{
		ret = parsed_unexpected(input->content);
		tok_lst_destroy(&input, free);
		return (ret);
	}
	tok_lst_push_back(redirs, tok_lst_uncons(&input));
	return (parsed_new(NULL, input));
}

/*
** \brief        Parse a command (this is the continuation of
**               parse_cmd splited for norm complience)
** \param input  Input tokens
** \return       Parsed struct containning a command AST
*/

static t_parsed	*st_parse_cmd_body(t_tok_lst *input)
{
	t_ast		*ast;
	t_parsed	*tmp;

	exit_if((ast = ast_new(AST_CMD)) == NULL);
	while (input != NULL &&
			(input->tag & TAG_IS_STR || input->tag & TAG_IS_REDIR))
	{
		if (input->tag & TAG_IS_STR)
			tok_lst_push_back(&ast->cmd_argv, tok_lst_uncons(&input));
		else if (input->tag & TAG_IS_REDIR)
		{
			tmp = parse_redir(input, &ast->redirs);
			if (parsed_check(tmp))
			{
				tok_lst_destroy(&tmp->rest, free);
				ast_destroy(ast);
				return (tmp);
			}
			input = tmp->rest;
			free(tmp);
		}
	}
	return (parsed_new(ast, input));
}

/*
** \brief        Check if the first token can be included in a command
**               then parse a command
** \param input  Input tokens
** \return       Parsed error if the first token isn't a redirection or string
**               Parsed containning a command AST otherwise
*/

t_parsed		*parse_cmd(t_tok_lst *input)
{
	t_parsed	*ret;

	if (input->tag & TAG_IS_SEP || input->tag == TAG_PIPE ||
		input->tag == TAG_PARENT_CLOSE)
	{
		ret = parsed_unexpected(input->content);
		tok_lst_destroy(&input, free);
		return (ret);
	}
	return (st_parse_cmd_body(input));
}

/*
** \brief        Parse a parenthesized expression
** \param input  Input tokens
** \return       Parsed containning a parenthesis AST
**               Parsed error if parenthesis don't match
** \note         Parenthesis can be followed by redirections
*/

static t_parsed	*st_parse_parenthesis(t_tok_lst *input)
{
	t_parsed	*parsed;
	t_parsed	*tmp;
	t_ast		*ast;

	if (parsed_check(parsed = parse_op(input)))
		return (parsed);
	if ((input = parsed->rest) == NULL || !(input->tag & TAG_PARENT_CLOSE))
	{
		ast_destroy(parsed->ast);
		free(parsed);
		return (parsed_expected());
	}
	tok_lst_pop_front(&input, free);
	exit_if((ast = ast_new(AST_PARENT)) == NULL);
	ast->parent_ast = parsed->ast;
	parsed->ast = ast;
	while (input != NULL && input->tag & TAG_IS_REDIR)
	{
		if (parsed_check(tmp = parse_redir(input, &parsed->ast->redirs)))
			return (tmp);
		input = tmp->rest;
		free(tmp);
	}
	parsed->rest = input;
	return (parsed);
}

/*
** \brief        Try to parse parenthesis, if fail parse a command.
** \param input  Input tokens
** \return       Whatever parse_parenthesis or parse_cmd returns
*/

t_parsed		*parse_expr(t_tok_lst *input)
{
	if (input->tag & TAG_PARENT_OPEN)
	{
		tok_lst_pop_front(&input, free);
		if (input == NULL)
			return (parsed_expected());
		return (st_parse_parenthesis(input));
	}
	return (parse_cmd(input));
}
