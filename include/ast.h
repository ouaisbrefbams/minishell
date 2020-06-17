/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:38 by charles           #+#    #+#             */
/*   Updated: 2020/06/17 16:43:12 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

/*
** \file     ast.h
** \brief    AST structs
*/

# include <stdlib.h>
# include <stdbool.h>
# include "libft_lst.h"
# include "lexer.h"

struct s_ast;

/*
** \brief            Line struct
** \param left       AST to the left of separator
** \param right      AST to the right of separator
** \param sep        Type of separator
*/


typedef struct			s_op
{
	struct s_ast		*left;
	struct s_ast		*right;
	enum e_token_tag	sep;
}						t_op;

/*
** \brief            AST node tag (type)
** \param TAG_CMD    Command AST node
** \param TAG_LINE   Line AST node
*/

enum			e_ast_tag
{
	AST_CMD,
	AST_OP,
	AST_OP_PARENT,
};

/*
** \brief             AST node struct
** \param tag         Node tag
** \param line        Line struct
** \param cmd_argv    Array of string tokens
** \param in          STDIN redirection string tokens
** \param out         STDOUT redirection string tokens
** \param is_append   True if out redirection is append to file
*/

typedef struct			s_ast
{
	enum e_ast_tag		tag;
	union
	{
		t_op			op;
		t_ftlst			*cmd_argv;
	};
	t_ftlst				*redirs;
}						t_ast;

typedef struct			s_ret
{
	t_token 			*unexpected;
	t_ast				*ast;
	t_ftlst				*rest;
}						t_ret;

t_ast					*ast_new(enum e_ast_tag tag);
void					ast_destroy(t_ast *ast);

#endif
