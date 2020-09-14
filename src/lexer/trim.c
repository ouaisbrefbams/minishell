/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:36 by nahaddac          #+#    #+#             */
/*   Updated: 2020/09/14 16:33:13 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lexer.h"
#include <stdio.h>

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

int 	del_quote(char *str)
{
    size_t	i;
    int nb_q;

    i = 0;
    nb_q = 1;
    if (str[0] == '\'')
	{
        while (str[i++] != '\0')
        {
            if (str[i] == '\\')
                i += 2;
            if (str[i] == '\'')
            {
                nb_q++;
                break ;
            }
        }
	}
    else if (str[0] == '"')
	{
        while (str[i++] != '\0')
        {
            if (str[i] == '\\')
                i += 2;
            if (str[i] == '"')
            {
                nb_q++;
                break ;
            }
        }
	}
    if (nb_q % 2 == 0)
    {
        str[i] = '\0';
    	if(!(ft_memmove(str, str + 1, ft_strlen(str + 1) + 1)))
            return 1;
        else
            return 0;
    }
    else
        return 2;
}

int	lexer_trim(t_tok_lst *tokens)
{
    int status;

    while (tokens != NULL)
    {
        if (tokens->tag & (TAG_STR_DOUBLE | TAG_STR_SINGLE))
        {
            if ((status = del_quote(tokens->content)) != 0)
				return (status);
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
    return (0);
}
