/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lst_wrapper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <me@cacharle.xyz>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 08:15:38 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/10 08:17:04 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void		tok_lst_push_back(t_tok_lst **tokens, t_tok_lst *pushed)
{
	ft_lstpush_back((t_ftlst **)tokens, (t_ftlst *)pushed);
}

void		tok_lst_pop_front(t_tok_lst **tokens, void (*del)(void *))
{
	ft_lstpop_front((t_ftlst **)tokens, del);
}

void		*tok_lst_destroy(t_tok_lst **tokens, void (*del)(void *))
{
	ft_lstdestroy((t_ftlst **)tokens, del);
	return (NULL);
}

t_tok_lst	*tok_lst_last(t_tok_lst *tokens)
{
	return ((t_tok_lst *)ft_lstlast((t_ftlst *)tokens));
}
