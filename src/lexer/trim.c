/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:36 by nahaddac          #+#    #+#             */
/*   Updated: 2020/08/27 17:30:11 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lexer.h"

char                *del_space(t_tok_lst *tok)
{
    int	i;

    i = 0;
    while (tok->content[i] != '\0')
    {
        if (tok->content[i] == '\\')
            return (tok->content);
        if (tok->content[i] == ' ')
            break ;
        i++;
    }
    return (ft_strsubf(tok->content, 0, i));
}

char                *del_quote(char *str)
{
    int		i;

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
