/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:36 by nahaddac          #+#    #+#             */
/*   Updated: 2020/07/17 10:18:51 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lexer.h"

char                *del_space(t_token *tk)
{
    int             i;
    char            *s;

    i = 0;
    while(tk->content[i] != '\0')
    {
        if(tk->content[i] == '\\')
            return tk->content;
        if(tk->content[i] == ' ')
            break;
        i++;
    }
    s = ft_strsubf(tk->content, 0, i);
    return(s);
}

char                *del_quote(char *str)
{
    int             i;
    char            *s;

    i = 0;
    while(str[i++] != '\0')
    {
        if(str[i] == '\\')
            i+=2;
        if (str[i] == '\'' || str[i] == '"')
            break;
    }

    if(str[i] != '\'' && str[i] != '"')
        return str;
    s = ft_strsubf(str, 1, i - 1);
    return (s);
}

t_ftlst             *lexe_trim_out(t_ftlst *lst)
{
    t_ftlst        *first;
    t_token        *tk;

    first = lst;
    while(lst != NULL)
    {
        tk = lst->data;
        if (tk->tag & (TAG_STR_DOUBLE | TAG_STR_SINGLE))
        {
            tk->content = del_quote(tk->content);
            if(lst->next == NULL)
                if (tk->tag & TAG_STICK)
                    tk->tag -= TAG_STICK;
        }
        else
        {
            tk->content = del_space(tk);
            if(lst->next == NULL)
                if (tk->tag & TAG_STICK)
                    tk->tag -= TAG_STICK;
        }
        lst = lst->next;
    }
    return (first);
}
