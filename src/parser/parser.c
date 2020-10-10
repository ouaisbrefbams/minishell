/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 18:09:04 by nahaddac          #+#    #+#             */
/*   Updated: 2020/10/10 09:19:32 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   parse.c
** \brief  Parser
*/

#include "minishell.h"
#include "parser.h"

static char *g_sep_str_lookup[] = {
	[TAG_END] = ";",
	[TAG_OR] = "||",
	[TAG_REDIR_IN] = "<",
	[TAG_REDIR_APPEND] = ">>",
	[TAG_PARENT_CLOSE] = ")",
	[TAG_AND] = "&&",
	[TAG_PIPE] = "|",
	[TAG_REDIR_OUT] = ">",
	[TAG_PARENT_OPEN] = "(",
};

t_parsed	*destroy_ret(t_parsed *destroyed, t_parsed *ret)
{
	parsed_destroy(destroyed);
	return (ret);
}

t_parsed	*parse_pipeline(t_tok_lst *input)
{
	t_parsed	*expr;
	t_parsed	*tail;
	t_ast		*expr_ast;
	t_ast		*pipeline_ast;

	expr = parse_expr(input);
	if (parsed_err(expr) || expr->rest == NULL || expr->rest->tag != TAG_PIPE)
		return (expr);
	tok_lst_pop_front(&expr->rest, free);
	if (expr->rest == NULL)
		return (destroy_ret(expr, parsed_expected()));
	if (parsed_err(tail = parse_pipeline(expr->rest)))
		return (destroy_ret(expr, tail));
	expr_ast = expr->ast;
	free(expr);
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

t_parsed	*parse_op_build(t_parsed *left, t_parsed *right, enum e_tok sep_tag)
{
	t_ast	*ast;

	exit_if((ast = ast_new(AST_OP)) == NULL);
	ast->op.left = left->ast;
	ast->op.right = right->ast;
	ast->op.sep = sep_tag;
	free(left);
	left = parsed_new(ast, right->rest);
	free(right);
	return (left);
}

/*
** parse and operation (| ; && ||)
*/

t_parsed	*parse_op(t_tok_lst *input)
{
	t_parsed	*left;
	t_parsed	*right;
	enum e_tok	sep_tag;

	if (input == NULL)
		return (NULL);
	if (parsed_err(left = parse_pipeline(input)))
		return (left);
	if ((input = left->rest) == NULL || input->tag & TAG_PARENT_CLOSE)
		return (left);
	if (!((sep_tag = input->tag) & TAG_IS_SEP))
	{
		tok_lst_destroy(&left->rest, free);
		parsed_destroy(left);
		return (parsed_unexpected(g_sep_str_lookup[sep_tag]));
	}
	tok_lst_pop_front(&input, free);
	if (input == NULL && sep_tag == TAG_END)
		return ((left->rest = NULL) ? left : left);
	if (input == NULL)
		return (destroy_ret(left, parsed_expected()));
	right = parse_op(input);
	if (parsed_err(right))
		return (destroy_ret(left, right));
	return (parse_op_build(left, right, sep_tag));
}

t_parsed	*parse(t_tok_lst *input)
{
	t_parsed	*parsed;
	t_parsed	*ret;

	if (input == NULL)
		return (NULL);
	parsed = parse_op(input);
	if (parsed_err(parsed))
		return (parsed);
	if (parsed->rest != NULL)
	{
		ret = parsed_unexpected(parsed->rest->content);
		tok_lst_destroy(&parsed->rest, free);
		ast_destroy(parsed->ast);
		free(parsed);
		return (ret);
	}
	return (parsed);
}
