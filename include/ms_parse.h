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

typedef struct t_ast {
    t_tag           tag;
    char*           contents;
    int             children_num;
    struct t_ast**  children;
} t_ast;

/*
** parse.c
*/

t_parsing				*ms_parse(char *input);

#endif
