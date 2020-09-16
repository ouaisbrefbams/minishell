/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:25 by nahaddac          #+#    #+#             */
/*   Updated: 2020/09/16 20:17:10 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdio.h>

// len until meaningful character for non quoted str
int 			len_until_sep(char *input)
{
	int i;

	i = -1;
	while (input[++i])
	{
		if (input[i] == '\\')
		{
			i += 2;
			if (ft_isblank(input[i]))
				return (i + 1 + lexer_space(&input[i + 1]));
			else if (input[i] != '\'' || input[i] != '"')
				i += len_until_sep(&input[i]);
			return (i);
		}
		if (lexer_sep(input[i]))
			return (i);
		if (input[i] == '\'' || input[i] == '"')
			return (i);
		if (ft_isblank(input[i]))
			return (i + 1 + lexer_space(&input[i + 1]));
	}
	return (i);
}

// token content length
int				tok_len(char *input)
{
	int i;

	i = 0;
	if (input[i] == '\\' && lexer_sep(input[i + 1]))
	{
		i += 2;
		return (i + lexer_space(&input[i]));
	}
	if (input[i] == '(' || input[i] == ')')
		return (i + 1);
	if (lexer_sep(input[i]))
	{
		if (input[i] == input[i + 1])
			i++;
		return (i + 1 + lexer_space(&input[i + 1]));
	}
	if (input[i] == '\'' || input[i] == '"')
		return (quote_len(input, i));
	if (ft_isblank(input[i]))
		return (i + 1 + lexer_space(&input[i + 1]));
	return (len_until_sep(&input[i]));
}

/*
** \brief interate over input
**        get the number of character for the current token
**        create a token from a substring in input
**        assign a tag to the token
*/

t_tok_lst				*create_token_list(char *input, t_tok_lst **lst)
{
	t_tok_lst	*tok;
	size_t 		i;
	size_t		j;
	size_t		len;

	len = ft_strlen(input);
	i = 0;
	while (i < len)
	{
		j = tok_len(&input[i]);
		tok = tok_lst_new_until(0, input + i, j);
		tok->tag = tok_assign_tag(tok->content);
		if (tok->tag == 0)
			tok->tag = tok_assign_str(tok);
		if (!ft_isblank(tok->content[0])) // ?
			tok_lst_push_back(lst, tok);
		i += j;
	}
	return (*lst);
}

/*
** \brief        Create a token list from a string
** \param input  Input string
** \return       The created tokens or NULL on error
*/

int     			lexer(char *input, t_tok_lst **out)
{
	int status;
	t_tok_lst	*curr;

	if (!input)
		return (2);
	*out = NULL;
	*out = create_token_list(input, out);
	status = lexer_trim(*out);
	curr = *out;
	while (curr != NULL)
	{
		if (!(curr->tag & TAG_IS_STR))
		{
			free(curr->content);
			curr->content = NULL;
		}
		curr = curr->next;
	}
	return (status);
}
