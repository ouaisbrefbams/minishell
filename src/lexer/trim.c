/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:36 by nahaddac          #+#    #+#             */
/*   Updated: 2020/09/13 09:24:21 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lexer.h"

char                *del_space(t_tok_lst *tok)
{
    int             i;

    i = ft_strlen(tok->content);
    if(tok->content[i - 1] == ' ')
    {
        i -= 1;
        while(tok->content[i] == ' ')
        {
            if (tok->content[i - 1] == '\\')
                break;
            i--;
        }
        tok->content = ft_strsubf(tok->content, 0, i + 1);
        return (tok->content);
    }
    return(tok->content);
}

char                *del_quote(char *str)
{
    int		i;
    i = 0;

    if (str[0] == '\'')
        while(str[i++] != '\0')
        {
            if(str[i] == '\\')
                i+=2;
            if (str[i] == '\'')
                break;
        }
    else if (str[0] == '"')
        while(str[i++] != '\0')
        {
            if(str[i] == '\\')
                i+=2;
            if (str[i] == '"')
                break;
        }
    if(str[i] != '\'' && str[i] != '"')
        return str;
    return (ft_strsubf(str, 1, i - 1));
}

t_tok_lst             *lexer_trim_out(t_tok_lst *tokens)
{
    t_tok_lst	*first;

    first = tokens;
    while (tokens != NULL)
    {
        if (tokens->tag & (TAG_STR_DOUBLE | TAG_STR_SINGLE))
        {
            tokens->content = del_quote(tokens->content);
            if (tokens->next == NULL)
                if (tokens->tag & TAG_STICK)
                    tokens->tag -= TAG_STICK;
        }
        else
        {
            tokens->content = del_space(tokens);
            if (tokens->next == NULL)
                if (tokens->tag & TAG_STICK)
                    tokens->tag -= TAG_STICK;
        }
        tokens = tokens->next;
    }
    return (first);
}
