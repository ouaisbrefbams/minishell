/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 11:02:52 by charles           #+#    #+#             */
/*   Updated: 2020/08/27 10:44:08 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

/*
** \brief   printf like function that only works with `%s`,
**          prefix the message with the program name
**          and output on STDERR
** \note    NULL arguments are ignored
*/

void			errorf(const char *format, ...)
{
	va_list	ap;

	va_start(ap, format);
	verrorf(format, ap);
	va_end(ap);
}

void			verrorf(const char *format, va_list ap)
{
	char	*arg;

	ft_putstr_fd(g_basename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	while (*format != '\0')
	{
		if (format[0] == '%' && format[1] == 's')
		{
			arg = va_arg(ap, char*);
			ft_putstr_fd(arg, STDERR_FILENO);
			format += 2;
		}
		else
		{
			ft_putchar_fd(*format, STDERR_FILENO);
			format++;
		}
	}
}
