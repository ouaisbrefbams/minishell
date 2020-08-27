/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:25 by nahaddac          #+#    #+#             */
/*   Updated: 2020/08/27 17:34:53 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int 			len_until_sep(char *input)
{
	int i;

	i = -1;
	while(input[++i])
	{
		if (input[i] == '\\')
		{
			i +=2;
			if (input[i] == '\\')
				i += len_until_sep(&input[i]);
		}
		if (lexer_sep(input[i]))
			return(i);
		if (input[i] == '\'' || input[i] == '"')
			return(i);
		if (input[i] == ' ')
		{
			while(input[++i] == ' ')
				;
			return (i);
		}
	}
	return(i);
}

int				check_input(char *input)
{
	int 				i;
	int 				op;

	i = 0;
	op = 1;
	if (input[i] == '\\' && lexer_sep(input[i + 1]))
		i += 2;
	if (input[i] == '(' || input[i] == ')')
	{
		i +=1;
		if(input[i] == ' ')
			while(input[i++] != ' ')
				;
		return (i);
	}
	if (lexer_sep(input[i]))
	{
		if (input[i] == ';')
			return (i += lexer_space(&input[i + 1]) + 1);
		while(input[i] == input[i + 1] && op < 2)
		{
			i++;
			op++;
		}
		i += lexer_space(&input[i + 1]);
		return (i + 1);
	}
	if (input[i] == 39 || input[i] == '"')
		return(lexer_check_between_quote(input, i));
	if (input[i] == ' ')
	{
		while(input[++i] == ' ')
			;
		 return (i);
	}
	return(len_until_sep(&input[i]));
}


int 					check_input_out(char *input)
{
	int i;
	int j;

	i = 0;
	while(input[i] != '\0')
	{
		j = 0;
		j += len_until_sep(&input[i]);
		if (j != 0)
			return(j);
		i += j;
		j = check_input(&input[i]);
		return(j);
	}
	return(0);
}

enum e_tok token_check_stick(t_tok_lst *tok)
{
	int i;

	i = ft_strlen(tok->content);
	if (i > 0)
		if (tok->content[i - 1] == ' ')
			return(tok->tag);
	return(tok->tag | TAG_STICK);
}

enum e_tok token_str_or_quote(t_tok_lst *tok)
{
	int i;

	i = 0;
	while(tok->content[i] != '\0')
	{
		if(tok->content[i] == '\'')
		{
			tok->tag = TAG_STR_SINGLE;
			return(token_check_stick(tok));
		}
		if(tok->content[i] == '"')
		{
			tok->tag = TAG_STR_DOUBLE;
			return(token_check_stick(tok));
		}
		else
		{
			tok->tag = TAG_STR;
			return(token_check_stick(tok));
		}
		i++;
	}
	return(0);
}

void						push_token_enum(t_tok_lst *tok)
{
	enum e_tok 		tag;

	tag = ret_token(tok->content, 0);
	if (tag == 0)
		tok->tag = token_str_or_quote(tok);
	else
		tok->tag = tag;
}

static t_tok_lst				*create_token_list(char *input, t_tok_lst **lst)
{
	t_tok_lst	*tok;
	size_t 		i;
	size_t		j;

	i = 0;
	while (i < ft_strlen(input))
	{
		j = 0;
		j += check_input(&input[i]);
		tok = tok_lst_new_until(0, input + i, j);
		push_token_enum(tok);
		if (tok->content[0] != ' ')
			tok_lst_push_back(lst, tok);
		i += j;
	}
	return (*lst);
}

t_tok_lst        			*lexer(char *input)
{
	t_tok_lst	*lst;

	if (!input)
		return (NULL);
	lst = NULL;
	lst = create_token_list(input, &lst);
	lst = lexer_trim_out(lst);
	return (lst);
}
