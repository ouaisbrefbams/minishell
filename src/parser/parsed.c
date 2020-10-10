/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsed.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 20:27:42 by charles           #+#    #+#             */
/*   Updated: 2020/10/10 09:20:28 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_parsed	*parsed_new(t_ast *ast, t_tok_lst *rest)
{
	t_parsed	*ret;

	if ((ret = malloc(sizeof(t_parsed))) == NULL)
		return (NULL);
	ret->syntax_error = false;
	ret->rest = rest;
	ret->ast = ast;
	return (ret);
}

void		parsed_destroy(t_parsed *parsed)
{
	ast_destroy(parsed->ast);
	free(parsed);
}
