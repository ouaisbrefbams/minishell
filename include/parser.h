/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 09:00:00 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/10 09:24:15 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

/*
** \file   parser.h
** \brief  Input parsing and AST manipulation
**
** Context free grammar:
** ```
** redir_in     ::= '<' <string>
** redir_out    ::= '>' <string>
** redir_append ::= '>>' <string>
** string       ::= '\'' .+ '\'' | '"' .+ '"' | [^ ]+
** sep          ::= '&&' | '||' | '|' | ';'
** expr         ::= <redir_in> | <redir_out> | <redir_append> | <string>
** cmd          ::= <expr>+
** line         ::= <cmd> <sep> <line> | <cmd>
** ```
*/

# include <stdlib.h>
# include <stdbool.h>
# include "libft_lst.h"
# include "lexer.h"
# include "error.h"

struct s_ast;

/*
** \brief            Operation struct
** \param left       AST to the left of separator
** \param right      AST to the right of separator
** \param sep        Type of separator
*/

typedef struct			s_op
{
	struct s_ast		*left;
	struct s_ast		*right;
	enum e_tok			sep;
}						t_op;

/*
** \brief              AST node tag (type)
** \param TAG_CMD      Command AST node
** \param TAG_OP       Operation AST node
** \param TAG_PARENT   Parenthesis AST node
** \param TAG_PIPELINE Pipeline AST node
*/

enum					e_ast
{
	AST_CMD,
	AST_OP,
	AST_PARENT,
	AST_PIPELINE,
};

/*
** \brief             AST node struct
** \param tag         Node tag
** \param op          Operation struct
** \param cmd_argv    Command argv tokens
** \param pipline     List of commands in a pipeline
** \param parend_ast  AST inside parenthesis
** \param redirs      Redirections tokens
** \note              Norminette doesn't like unamed union
**                    It breaks my heart to waste memory but
**                    t_ast isn't heavily used anyway
*/

typedef struct			s_ast
{
	enum e_ast			tag;
	t_op				op;
	t_tok_lst			*cmd_argv;
	t_ftlst				*pipeline;
	struct s_ast		*parent_ast;
	t_tok_lst			*redirs;
}						t_ast;

/*
** ast.c
*/

t_ast					*ast_new(enum e_ast tag);
void					ast_destroy(t_ast *ast);

typedef struct			s_parsed
{
	bool				syntax_error;
	t_ast				*ast;
	t_tok_lst			*rest;
}						t_parsed;

/*
** parsed.c
*/

t_parsed				*parsed_new(t_ast *ast, t_tok_lst *rest);
void					parsed_destroy(t_parsed *parsed);

/*
** parsed_error.c
*/

t_parsed				*parsed_error(const char *format, ...);
t_parsed				*parsed_expected(void);
t_parsed				*parsed_unexpected(char *content);
bool					parsed_check(t_parsed *parsed);

/*
** parse.c
*/

t_parsed				*parse(t_tok_lst *input);
t_parsed				*parse_op(t_tok_lst *input);

/*
** parse.c
*/

t_parsed				*parse_expr(t_tok_lst *input);
t_parsed				*parse_cmd(t_tok_lst *input);

#endif
