/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 09:00:00 by cacharle          #+#    #+#             */
/*   Updated: 2020/06/18 12:40:22 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "minishell.h"
# include "ast.h"

/*
** \file   parse.h
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

/*
** parse.c
*/


t_ret				*parse(t_ftlst *input);
t_ret		        *parse_op(t_ftlst *input)
t_ret		        *parse_expr(t_ftlst *input);
t_ret               *parse_cmd(t_ftlst *input);

// utils
t_ret			    *ret_wrap_ast(t_ast *ast, t_ftlst *rest);
t_ftlst				*push_token(t_ftlst **tokens, t_token *pushed);



t_ast 				*push_cmd(t_ast *ast, t_ftlst *ret);
t_ast 				*push_redir(t_ast *ast, t_ftlst *rest);
int                 parse_cmd_str_true_false(enum e_token_tag tag);
int                 parse_redir_true_false(enum e_token_tag tag);

#endif
