/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 09:32:58 by charles           #+#    #+#             */
/*   Updated: 2020/09/09 18:10:26 by charles          ###   ########.fr       */
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
	ret->next = NULL;
	return (ret);
}

void					tok_lst_push_back(t_tok_lst **tokens, t_tok_lst *pushed)
{
	ft_lstpush_back((t_ftlst**)tokens, (t_ftlst*)pushed);
}

t_tok_lst				*tok_lst_push_front(t_tok_lst **tokens, t_tok_lst *pushed)
{
	if (pushed == NULL)
		return (NULL);
	ft_lstpush_front((t_ftlst**)tokens, (t_ftlst*)pushed);
	return (*tokens);
}

void					*tok_lst_destroy(t_tok_lst **tokens, void (*del)(void*))
{
	ft_lstdestroy((t_ftlst**)tokens, del);
	return (NULL);
}

t_tok_lst				*tok_lst_last(t_tok_lst *tokens)
{
	return ((t_tok_lst*)ft_lstlast((t_ftlst*)tokens));
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

t_tok_lst				*tok_lst_take_sticked(t_tok_lst **tokens)
{
	t_tok_lst	*start;
	t_tok_lst	*curr;

	if (*tokens == NULL)
		return (NULL);
	start = *tokens;
	curr = *tokens;
	while (curr->tag & TAG_STICK && curr->tag & TAG_IS_STR)
		curr = curr->next;
	/* if (curr->tag & TAG_IS_STR) */
	/* 	curr = curr->next; */
	*tokens = curr->next;
	curr->next = NULL;
	return (start);
}
