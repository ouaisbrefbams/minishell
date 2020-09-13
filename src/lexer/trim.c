/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:36 by nahaddac          #+#    #+#             */
/*   Updated: 2020/09/13 20:42:05 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lexer.h"

void	del_space(char *str)
{
    int i;

    i = ft_strlen(str);
    if (ft_isblank(str[i - 1]))
    {
        i -= 1;
        while (ft_isblank(str[i]))
        {
            if (str[i - 1] == '\\')
                break ;
            i--;
        }
		str[i + 1] = '\0';
    }
}

void	del_quote(char *str)
{
    int	i;

    i = 0;
    if (str[0] == '\'')
	{
        while (str[i++] != '\0')
        {
            if (str[i] == '\\')
                i += 2;
            if (str[i] == '\'')
                break ;
        }
	}
    else if (str[0] == '"')
	{
        while (str[i++] != '\0')
        {
            if (str[i] == '\\')
                i += 2;
            if (str[i] == '"')
                break ;
        }
	}
	str[i] = '\0';
	ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
}

void	lexer_trim(t_tok_lst *tokens)
{
    while (tokens != NULL)
    {
        if (tokens->tag & (TAG_STR_DOUBLE | TAG_STR_SINGLE))
        {
            del_quote(tokens->content);
            if (tokens->next == NULL)
				tokens->tag &= ~TAG_STICK;
        }
        else
        {
            del_space(tokens->content);
            if (tokens->next == NULL)
				tokens->tag &= ~TAG_STICK;
        }
        tokens = tokens->next;
    }
}
