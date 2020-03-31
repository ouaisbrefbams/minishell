/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 09:00:00 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/28 14:57:58 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_PARSE_H
# define MS_PARSE_H

# include "minishell.h"
# include "ast.h"

/**
** \file   ms_parse.h
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

/**
** \brief                  AST node tag
** \param TAG_STRING       string
** \param TAG_SEP          `;` | `|` | `&&` | `||`
** \param TAG_REDIR_OUT    `>`
** \param TAG_REDIR_IN     `<`
** \param TAG_REDIR_APPEND `>>`
*/

// typedef enum
// {
//     TAG_STRING,
//     TAG_SEP,
//     TAG_REDIR_OUT,
//     TAG_REDIR_IN,
//     TAG_REDIR_APPEND,
// 	TAG_CMD,
// 	TAG_LINE,
// }   t_ast_tag;

/**
** \brief               AST (Abstract Syntax Tree)
** \param tag           tag (type) of node
** \param content       substring of the parsed string which corespond to tag
** \param children_num  number of children
** \param children      children nodes
*/

// typedef struct		s_ast
// {
//     t_tag			tag;
//     char*			content;
//     int				children_num;
//     struct s_ast**	children;
// }					t_ast;

/*
** lexer.c
*/

char				**ms_lexer(char *input);

/*
** parse.c
*/

t_ast				*ms_parse(char *input);

/*
** ast.c
*/

// t_ast				*ms_ast_new(t_tag tag);
// void				ms_ast_destroy(t_ast *ast);
// void				ms_ast_iter(t_ast *ast, void (*f)(void *f_arg, t_ast *children), void *arg);

#endif
