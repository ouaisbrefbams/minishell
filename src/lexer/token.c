/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/09 13:38:08 by charles           #+#    #+#             */
/*   Updated: 2020/08/19 13:40:55 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token				*token_new(enum e_tok tag, char *content)
{
	size_t	len;

	len = 0;
	if (content != NULL)
		len = ft_strlen(content);
	return (token_new_until(tag, content, len));
}

t_token				*token_new_until(enum e_tok tag, char *content, int n)
{
	t_token			*token;

	if ((token = (t_token*)malloc(sizeof(t_token))) == NULL)
		return (NULL);
	if (content == NULL)
		token->content = NULL;
	else if ((token->content = ft_strndup(content, n)) == NULL)
	{
		free(token);
		return (NULL);
	}
	token->tag = tag;
	return token;
}

void				token_destroy(t_token *token)
{
	free(token->content);
	free(token);
}

void				token_destroy_lst(t_ftlst *tokens)
{
	ft_lstdestroy(&tokens, (void (*)(void*))token_destroy);
}

void				token_destroy_lst2(t_ftlst *tokens1, t_ftlst *tokens2)
{
	ft_lstdestroy(&tokens1, (void (*)(void*))token_destroy);
	ft_lstdestroy(&tokens2, (void (*)(void*))token_destroy);
}

enum e_tok	token_tag(t_ftlst *token_lst)
{
	return (((t_token*)token_lst->data)->tag);
}

void				token_set_tag(t_ftlst *token_lst, enum e_tok tag)
{
	((t_token*)token_lst->data)->tag = tag;
}

char				*token_content(t_ftlst *token_lst)
{
	return (((t_token*)token_lst->data)->content);
}

void				token_set_content(t_ftlst *token_lst, char *content)
{
	((t_token*)token_lst->data)->content = content;
}

void				token_set_contentf(t_ftlst *token_lst, char *content)
{
	free(token_content(token_lst));
	token_set_content(token_lst, content);
}
