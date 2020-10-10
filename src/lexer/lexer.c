/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:25 by nahaddac          #+#    #+#             */
/*   Updated: 2020/10/10 08:35:03 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdio.h>

static int			st_lexer_sep(char c)
{
	return (ft_strchr(";&|><()", c) != NULL);
}

static int			st_len_until_sep(char *input)
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
				i += st_len_until_sep(&input[i]);
			return (i);
		}
		if (st_lexer_sep(input[i]))
			return (i);
		if (input[i] == '\'' || input[i] == '"')
			return (i);
		if (ft_isblank(input[i]))
			return (i + 1 + lexer_space(&input[i + 1]));
	}
	return (i);
}

static int			st_tok_len(char *input)
{
	int i;

	i = 0;
	if (input[i] == '\\' && st_lexer_sep(input[i + 1]))
	{
		i += 2;
		return (i + lexer_space(&input[i]));
	}
	if (input[i] == '(' || input[i] == ')')
		return (i + 1);
	if (st_lexer_sep(input[i]))
	{
		if (input[i] != ';' && input[i] == input[i + 1])
			i++;
		return (i + 1 + lexer_space(&input[i + 1]));
	}
	if (input[i] == '\'' || input[i] == '"')
		return (quote_len(input, i));
	if (ft_isblank(input[i]))
		return (i + 1 + lexer_space(&input[i + 1]));
	return (st_len_until_sep(&input[i]));
}

/*
** \brief interate over input
**        get the number of character for the current token
**        create a token from a substring in input
**        assign a tag to the token
**		  return all token
*/

static t_tok_lst	*st_create_token_list(char *input, t_tok_lst **lst)
{
	t_tok_lst	*tok;
	size_t		i;
	size_t		j;
	size_t		len;

	len = ft_strlen(input);
	i = 0;
	while (i < len)
	{
		j = st_tok_len(&input[i]);
		tok = tok_lst_new_until(0, input + i, j);
		tok->tag = tok_assign_tag(tok->content);
		if (tok->tag == 0)
			tok->tag = tok_assign_str(tok);
		if (!ft_isblank(tok->content[0]))
			tok_lst_push_back(lst, tok);
		else
			tok_lst_destroy(&tok, free);
		i += j;
	}
	return (*lst);
}

/*
** \brief        Create a token list from a string
** \param input  Input string
** \return       The created tokens or NULL on error
*/

int					lexer(char *input, t_tok_lst **out)
{
	int status;

	*out = NULL;
	*out = st_create_token_list(input, out);
	status = lexer_trim(*out);
	return (status);
}
