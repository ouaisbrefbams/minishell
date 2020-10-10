/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsed_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <me@cacharle.xyz>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 09:20:17 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/10 09:24:59 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_parsed	*parsed_error(const char *format, ...)
{
	t_parsed	*err;
	va_list		ap;

	if ((err = parsed_new(NULL, NULL)) == NULL)
		return (NULL);
	err->syntax_error = true;
	va_start(ap, format);
	verrorf(format, ap);
	va_end(ap);
	ft_putchar_fd('\n', STDERR_FILENO);
	return (err);
}

t_parsed	*parsed_expected(void)
{
	return (parsed_error("syntax error expected token"));
}

t_parsed	*parsed_unexpected(char *content)
{
	return (parsed_error("syntax error near unexpected token `%s'", content));
}

bool		parsed_check(t_parsed *parsed)
{
	return (parsed == NULL || parsed->syntax_error);
}
