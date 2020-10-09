/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 18:09:04 by nahaddac          #+#    #+#             */
/*   Updated: 2020/10/09 12:35:51 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   parse.c
** \brief  Parser
*/

#include "parser.h"
#include "minishell.h"

static char *g_sep_str_lookup[] = {
	[TAG_END] = ";",
	[TAG_AND] = "&&",
	[TAG_OR] = "||",
	[TAG_PIPE] = "|",
	[TAG_REDIR_IN] = "<",
	[TAG_REDIR_OUT] = ">",
	[TAG_REDIR_APPEND] = ">>",
	[TAG_PARENT_OPEN] = "(",
	[TAG_PARENT_CLOSE] = ")",
};

/*
** push redirection token
** push while token is str and stick
** push last str token
*/

t_parsed	*parse_redir(t_tok_lst *input, t_tok_lst **redirs)
{
	tok_lst_push_back(redirs, tok_lst_uncons(&input));
	// FIXME big condition could be avoided with lexer not putting STICK
	// if the next tokens isn't a string
	while (input != NULL
			&& input->tag & TAG_IS_STR && input->tag & TAG_STICK
			&& input->next != NULL && input->next->tag & TAG_IS_STR)
		tok_lst_push_back(redirs, tok_lst_uncons(&input));
	if (input == NULL)
		return (parsed_error("syntax error near unexpected token `newline'"));
	if (!(input->tag & TAG_IS_STR))
	{
		t_parsed *ret = parsed_error("syntax error near unexpected token `%s'", input->content);
		tok_lst_destroy(&input, free);
		return (ret);
	}
	tok_lst_push_back(redirs, tok_lst_uncons(&input));
	return (parsed_new(NULL, input));
}

t_parsed	*parse_cmd(t_tok_lst *input)
{
	t_ast		*ast;
	t_parsed	*tmp;

	if (input->tag & TAG_IS_SEP || input->tag == TAG_PIPE || input->tag == TAG_PARENT_CLOSE)
	{
		t_parsed *ret = parsed_error("syntax error near unexpected token `%s'", input->content);
		tok_lst_destroy(&input, free);
		return (ret);
	}
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
			{
				tok_lst_destroy(&tmp->rest, free);
				ast_destroy(ast);
				return (tmp);
			}
			input = tmp->rest;
			free(tmp);
		}
		else
			break;
	}
	return (parsed_new(ast, input));
}

t_parsed	*parse_pipeline(t_tok_lst *input)
{
	t_parsed	*expr;
	t_parsed	*tail;
	t_ast		*expr_ast;

	expr = parse_expr(input);
	if (expr == NULL || expr->syntax_error ||
		expr->rest == NULL || expr->rest->tag != TAG_PIPE)
		return (expr);
	tok_lst_pop_front(&expr->rest, free);
	if (expr->rest == NULL)
	{
		ast_destroy(expr->ast);
		free(expr);
		return (parsed_error("syntax error expected token"));
	}
	tail = parse_pipeline(expr->rest);
	if (tail == NULL || tail->syntax_error)
	{
		ast_destroy(expr->ast);
		free(expr);
		return (tail);
	}
	expr_ast = expr->ast;
	free(expr);
	t_ast *pipeline_ast;
	if (tail->ast->tag == AST_CMD || tail->ast->tag == AST_PARENT)
	{
		pipeline_ast = ast_new(AST_PIPELINE);
		exit_if((pipeline_ast->pipeline = ft_lstnew(tail->ast)) == NULL);
	}
	else
		pipeline_ast = tail->ast;
	exit_if(ft_lstpush_front_node(&pipeline_ast->pipeline, expr_ast) == NULL);
	tail->ast = pipeline_ast;
	return (tail);
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

	if (input == NULL)
		return (NULL);
	left = parse_pipeline(input);
	if (left == NULL || left->syntax_error)
		return (left);
	input = left->rest;
	if (input == NULL || input->tag & TAG_PARENT_CLOSE)
		return (left);
	sep_tag = input->tag;
	if (!(sep_tag & TAG_IS_SEP))
	{
		ast_destroy(left->ast);
		tok_lst_destroy(&left->rest, free);
		free(left);
		return (parsed_error("syntax error near unexpected token `%s'",
				g_sep_str_lookup[sep_tag]));
	}
	tok_lst_pop_front(&input, free);
	if (input == NULL && sep_tag == TAG_END)
	{
		left->rest = NULL;
		return (left);
	}
	if (input == NULL)
	{
		ast_destroy(left->ast);
		free(left);
		return (parsed_error("syntax error expected token"));
	}
	right = parse_op(input);
	if (right == NULL  || right->syntax_error)
	{
		ast_destroy(left->ast);
		free(left);
		return (right);
	}
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
		tok_lst_pop_front(&input, free);
		if (input == NULL)
			return (parsed_error("syntax error expected token"));
		parsed = parse_op(input);
        if (parsed == NULL || parsed->syntax_error)
			return (parsed);
        input = parsed->rest;
		if (input == NULL || !(input->tag & TAG_PARENT_CLOSE))
		{
			ast_destroy(parsed->ast);
			free(parsed);
			return (parsed_error("syntax error expected token"));
		}
		tok_lst_pop_front(&input, free);
        if ((ast = ast_new(AST_PARENT)) == NULL)
			return (NULL);
        ast->parent_ast = parsed->ast;
        parsed->ast = ast;
		while (input != NULL && input->tag & TAG_IS_REDIR)
		{
			tmp = parse_redir(input, &parsed->ast->redirs);
			if (tmp == NULL || tmp->syntax_error)
				return (tmp);
			free(tmp);
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
	t_parsed *parsed = parse_op(input);
	if (parsed == NULL || parsed->syntax_error)
		return parsed;
	if (parsed->rest != NULL)
	{
		t_parsed *ret = parsed_error("syntax error near unexpected token `%s'", parsed->rest->content);
		tok_lst_destroy(&parsed->rest, free);
		ast_destroy(parsed->ast);
		free(parsed);
		return ret;
	}
	return (parsed);
}
