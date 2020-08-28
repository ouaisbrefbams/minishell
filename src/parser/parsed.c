/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsed.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 20:27:42 by charles           #+#    #+#             */
/*   Updated: 2020/08/28 10:44:39 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_parsed					*parsed_new(t_ast *ast, t_tok_lst *rest)
{
	t_parsed	*ret;

	if ((ret = malloc(sizeof(t_parsed))) == NULL)
		return (NULL);
	ret->syntax_error = false;
	ret->rest = rest;
	ret->ast = ast;
	return ret;
}

t_parsed					*parsed_error(const char *format, ...)
{
	t_parsed	*err;
	va_list		ap;

	if ((err = parsed_new(NULL, NULL)) == NULL)
		return (NULL);
	err->syntax_error = true;
	va_start(ap, format);
	verrorf(format, ap);
	va_end(ap);
	return (err);
}