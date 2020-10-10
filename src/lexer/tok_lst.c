/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 09:32:58 by charles           #+#    #+#             */
/*   Updated: 2020/10/10 08:15:34 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_tok_lst				*tok_lst_new(enum e_tok tag, char *content)
{
	return (tok_lst_new_until(
		tag, content, content == NULL ? 0 : ft_strlen(content)));
}

/*
** \brief          Create a new tok_lst
** \param tag      tok_lst tag
** \param content  tok_lst content
** \param n        The maximum number of character to take from content
** \return         An allocated tok_lst or NULL on error
*/

t_tok_lst				*tok_lst_new_until(
	enum e_tok tag, char *content, size_t n)
{
	t_tok_lst	*ret;

	if ((ret = malloc(sizeof(t_tok_lst))) == NULL)
		return (NULL);
	if (content == NULL)
		ret->content = NULL;
	else if ((ret->content = ft_strndup(content, n)) == NULL)
	{
		free(ret);
		return (NULL);
	}
	ret->tag = tag;
	ret->next = NULL;
	return (ret);
}

t_tok_lst				*tok_lst_push_front(
	t_tok_lst **tokens, t_tok_lst *pushed)
{
	if (pushed == NULL)
		return (NULL);
	ft_lstpush_front((t_ftlst**)tokens, (t_ftlst*)pushed);
	return (*tokens);
}

t_tok_lst				*tok_lst_uncons(t_tok_lst **tokens)
{
	t_tok_lst	*poped;

	if (tokens == NULL || *tokens == NULL)
		return (NULL);
	poped = *tokens;
	*tokens = poped->next;
	poped->next = NULL;
	return (poped);
}
