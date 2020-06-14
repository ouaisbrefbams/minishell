/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/09 13:38:08 by charles           #+#    #+#             */
/*   Updated: 2020/06/14 20:51:25 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token				*token_new(enum e_token_tag tag, char *content)
{
	t_token			*token;

	if ((token = (t_token*)malloc(sizeof(t_token))) == NULL)
		return (NULL);
	if (content == NULL)
		token->content = NULL;
	else if ((token->content = ft_strdup(content)) == NULL)
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
