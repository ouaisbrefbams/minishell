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

/*
** TAG_CMD:          command name
** TAG_ARG:          command argument
** TAG_ENDCMD:       `;`
** TAG_PIPE:         `|`
** TAG_AND:          `&&`
** TAG_OR:           `||`
** TAG_REDIR_OUT:    `>`
** TAG_REDIR_IN:     `<`
** TAG_REDIR_APPEND: `>>`
*/

typedef enum
{
    TAG_CMD,
    TAG_ARG,
    TAG_ENDCMD,
    TAG_PIPE,
    TAG_AND,
    TAG_OR,
    TAG_REDIR_OUT,
    TAG_REDIR_IN,
    TAG_REDIR_APPEND
}   t_tag;

/*
** AST (Abstract Syntax Tree)
** A node of the ast is represented by a tag (his type) and a content associated with it.
** (i.e a TAG_CMD would be paired with the command executable name)
** Each node also has children node which must be evaluated before him
**
** examle: "echo foo bar && ls"
**
** tags:
**       TAG_AND
**   	/       \
**     /         \
**   TAG_CMD    TAG_CMD
**   /      \
** TAG_ARG  TAG_ARG
**
** content:
**  	 &&
**   	/ \
**     /   \
**   echo   ls
**   /  \
** foo  bar
*/

typedef struct t_ast {
    t_tag           tag;
    char*           contents;
    int             children_num;
    struct t_ast**  children;
} t_ast;

char          **ms_lexer(char *input);

/*
** parse.c
*/


void		*ms_parse(char *input);

#endif
