/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/09 13:38:08 by charles           #+#    #+#             */
/*   Updated: 2020/06/09 13:39:27 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*token_new(enum e_token_tag tag, char *content)
{
	t_token	*token;

	if (content == NULL
		|| (token = malloc(sizeof(t_token))) == NULL)
		return (NULL);
	if ((token->content = ft_strdup(content)) == NULL)
	{
		free(token);
		return (NULL);
	}
	token->tag = tag;
	return token;
}
