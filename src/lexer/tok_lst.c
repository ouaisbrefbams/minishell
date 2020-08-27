/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 09:32:58 by charles           #+#    #+#             */
/*   Updated: 2020/08/27 09:37:57 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_tok_lst				*tok_lst_new(enum e_tok tag, char *content)
{
	return (tok_lst_new_until(tag, content, content == NULL ? 0 : ft_strlen(content)));
}

t_tok_lst				*tok_lst_new_until(enum e_tok tag, char *content, size_t n)
{
	t_tok_lst	*ret;

	if ((ret = malloc(sizeof(t_tok_lst))) == NULL)
		return (NULL);
	if (content == NULL)
		ret->content =  NULL;
	else if ((ret->content = ft_strndup(content, n)) == NULL)
	{
		free(ret);
		return (NULL);
	}
	ret->tag = tag;
	return (ret);
}
