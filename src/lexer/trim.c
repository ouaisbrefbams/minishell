/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 08:18:36 by nahaddac          #+#    #+#             */
/*   Updated: 2020/10/10 08:32:41 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static void		st_del_space(char *str)
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

static size_t	st_count_quote(char *str)
{
	char	quote_char;
	size_t	counter;

	if (*str != '\'' && *str != '"')
		return (0);
	quote_char = *str;
	counter = 1;
	str++;
	while (*str != '\0')
	{
		if (quote_char == '"' && *str == '\\')
			str += 2;
		if (*str == quote_char)
		{
			counter++;
			*str = '\0';
			break ;
		}
		str++;
	}
	return (counter);
}

static int		st_del_quote(char *str)
{
	size_t	quote_counter;

	quote_counter = st_count_quote(str);
	if (quote_counter % 2 == 1)
	{
		errorf("unexpected EOF while looking for matching `%c'\n", str[0]);
		return (2);
	}
	ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
	return (0);
}

int				lexer_trim(t_tok_lst *tokens)
{
	int status;

	while (tokens != NULL)
	{
		if (tokens->tag & (TAG_STR_DOUBLE | TAG_STR_SINGLE))
		{
			if ((status = st_del_quote(tokens->content)) != 0)
				return (status);
			if (tokens->next == NULL)
				tokens->tag &= ~TAG_STICK;
		}
		else
		{
			st_del_space(tokens->content);
			if (tokens->next == NULL)
				tokens->tag &= ~TAG_STICK;
		}
		tokens = tokens->next;
	}
	return (0);
}
