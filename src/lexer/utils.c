/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:15 by nahaddac          #+#    #+#             */
/*   Updated: 2020/09/14 11:38:18 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

// return token tag corresponding to string id
enum e_tok                tok_assign_tag(char *content)
{
    if (content[0] == ';')
        return (TAG_END);
    if (ft_strncmp(content, "&&", 2) == 0)
        return (TAG_AND);
    if (ft_strncmp(content, "||", 2) == 0)
        return (TAG_OR);
    if(content[0]  == '|')
        return (TAG_PIPE);
    if (content[0] == '>')
    	return (TAG_REDIR_OUT);
    if (content[0] == '<')
        return (TAG_REDIR_IN);
    if (ft_strncmp(content, ">>", 2) == 0)
    	return (TAG_REDIR_APPEND);
    if (content[0] == '(')
        return (TAG_PARENT_OPEN);
    if (content[0] == ')')
        return (TAG_PARENT_CLOSE);
    return (0);
}

enum e_tok tok_assign_stick(t_tok_lst *tok)
{
	int i;

	i = ft_strlen(tok->content);
	if (i > 0)
		if (ft_isblank(tok->content[i - 1]))
			return (tok->tag);
	return (tok->tag | TAG_STICK);
}

enum e_tok tok_assign_str(t_tok_lst *tok)
{
	int i;

	// could use strchr to search ' or "
	i = 0;
	while (tok->content[i] != '\0')
	{
		if (tok->content[i] == '\'')
		{
			tok->tag = TAG_STR_SINGLE;
			return (tok_assign_stick(tok));
		}
		if (tok->content[i] == '"')
		{
			tok->tag = TAG_STR_DOUBLE;
			return (tok_assign_stick(tok));
		}
		else
		{
			tok->tag = TAG_STR;
			return (tok_assign_stick(tok));
		}
		i++;
	}
	return (0);
}


// check is char is separator
// & alone could be considered a separator
int                 	lexer_sep(char c)
{
	return (ft_strchr(";&|><()", c) != NULL);
}

// number of starting space character
int             		lexer_space(char *input)
{
	return (ft_strspn(input, " \t"));
}

int             		quote_len(char *input, int i)
{
	char	quote_type;

	quote_type = input[i];
    i++;
    while (input[i] != quote_type && input[i] != '\0')
    {
        if(input[i] == '\\')
            i+=2;
        i++;
    }
	while (ft_isblank(input[i + 1]))
		i++;
    return (i + 1);
}
