/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:15 by nahaddac          #+#    #+#             */
/*   Updated: 2020/08/27 09:55:03 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

// check for append tag
enum e_tok                ret_token_sep_redir_append(char *input, int i)
{
    if (input[i + 1] ==  '>')
        return(TAG_REDIR_APPEND);
    return (TAG_REDIR_OUT);

}

// return token tag corresponding to string id
enum e_tok                ret_token(char *input, int  i)
{
    if (input[i] == ';')
        return(TAG_END);
    if (input[i] == '&' && input[i + 1] == '&')
        return(TAG_AND);
    if (input[i] == '|' && input[i + 1] == '|')
        return(TAG_OR);
    if(input[i]  == '|')
        return(TAG_PIPE);
    if (input[i] == '>')
        return(ret_token_sep_redir_append(input,i));
    if (input[i] == '<')
        return(TAG_REDIR_IN);
    if (input[i] ==  '(')
        return(TAG_PARENT_OPEN);
    if (input[i] == ')')
        return(TAG_PARENT_CLOSE);
    return(0);

}

// check is char is separator
// /!\ can be replaced by ft_strchr(";&|><()", input) == NULL
int                 	lexer_sep(char input)
{
    char            *sep;
    int             i;

    i = 0;
    sep = ";&|><()";
    while(sep[i] != '\0')
    {
        if(sep[i] == input)
            return(1);
        i++;
    }
    return (0);
}

// skip spaces
// /!\ can be replaced by strspn
int             		lexer_space(char *input)
{
    int i;

    i=0;
    while(input[i] == ' ')
        i++;
    return(i);
}

static int             lex_check_single_quote(char *input, int i)
{
    i++;
    while(input[i] != '\0')
    {
        if(input[i] == '\\')
            i+=1;
        if(input[i] == '\'')
            break;
        ++i;
    }
    if (input[i + 1] == ' ')
        while(input[i + 1] == ' ')
            i++;
    return(i + 1);
}

int             		lexer_check_between_quote(char *input, int i)
{
    if(input[i] == '\'')
        return(lex_check_single_quote(input, i));
    i++;
    while(input[i] != '"' && (input[i] != '\0'))
    {
        if (input[i] == '\\')
            i += 1;
        ++i;
    }
    if (input[i + 1] == ' ')
        while(input[i + 1] == ' ')
            i++;
    return(i + 1);
}
