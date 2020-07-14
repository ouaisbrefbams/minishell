/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 15:09:48 by nahaddac          #+#    #+#             */
/*   Updated: 2020/07/14 09:43:35 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// t_token                 *error_syntax_parent(t_ftlst *in)
// {
//     t_token             *tk;
//     t_ftlst             *first;
//
//     first = in;
//     while(in != NULL)
//     {
//         tk = in->data;
//         if (tk->tag & TAG_PARENT_OPEN || tk->tag & TAG_PARENT_CLOSE)
//             printf("%s\n", "coucouc");
//         in = in->next;
//     }
//     return first;
// }

int                     out_error_first(t_token *tk)
{
    int                 i;

    i = 0;
    if(tk->tag & TAG_IS_SEP)
            return(1);
    if (tk->tag & TAG_IS_REDIR)
    {
        while(tk->content[i])
            i++;
        if (tk->tag & TAG_REDIR_APPEND && i <= 2)
            return (0);
        else
            return(1);
    }
    return(0);
}

t_token                 *error_syntax_simple(t_ftlst *in)
{
    t_token             *tk;
    size_t              i;
    t_ftlst             *first;

    tk = in->data;
    first = in;
    if(tk->tag & TAG_IS_SEP || (tk->tag & TAG_IS_REDIR))
    {
        if (out_error_first(tk))
        {
            i = ft_strlen(tk->content);
            if (i >= 2)
                tk->content[2] = '\0';
            tk->content =
            ft_strjoin3("minishell:  syntax error near unexpected token `",
                        tk->content, "'");
            return(tk);
        }
    }
    while(in != NULL)
    {
        i = 0;
        tk = in->data;
        if(tk->tag & TAG_IS_SEP || (tk->tag & TAG_IS_REDIR))
        {
            if (((t_token *)in->next->data)->tag &
            ((t_token*)in->next->data)->tag & TAG_IS_SEP ||
            (((t_token*)in->next->data)->tag & TAG_IS_REDIR))
            {
                tk = in->next->data;
                i = ft_strlen(tk->content);
                if (i >= 3)
                    tk->content[2] = '\0';
                tk->content =
                ft_strjoin3("minishell:  syntax error near unexpected token `",
                            tk->content, "'");
                printf("%s\n",tk->content );
                return(tk);
            }
        }
        in = in->next;
    }
    return 0;
}
