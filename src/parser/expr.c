/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <me@cacharle.xyz>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 08:42:24 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/10 09:11:10 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/*
** push redirection token
** push while token is str and stick
** push last str token
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
			if (parsed_err(tmp))
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

static t_parsed	*st_parse_parenthesis(t_tok_lst *input)
{
	t_parsed	*parsed;
	t_parsed	*tmp;
	t_ast		*ast;

	if (parsed_err(parsed = parse_op(input)))
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
		if (parsed_err(tmp = parse_redir(input, &parsed->ast->redirs)))
			return (tmp);
		input = tmp->rest;
		free(tmp);
	}
	parsed->rest = input;
	return (parsed);
}

/*
** try to parse parenthesis, if fail parse a command.
** parenthesis can be followed by redirections
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
