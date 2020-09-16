/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 11:02:52 by charles           #+#    #+#             */
/*   Updated: 2020/09/16 16:28:10 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

/*
** \brief   printf like function that only works with `%s` and `%c`,
**          prefix the message with the program name
**          and output on STDERR
** \note    NULL arguments are ignored
*/

void		errorf(const char *format, ...)
{
	va_list	ap;

	va_start(ap, format);
	verrorf(format, ap);
	va_end(ap);
}

/*
** \brief   errorf with an argument pointer (ap) instead of arguments
*/

void		verrorf(const char *format, va_list ap)
{
	char	*str;
	char	c;

	ft_putstr_fd(g_state.progname, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	while (*format != '\0')
	{
		if (format[0] == '%' && format[1] == 's')
		{
			str = va_arg(ap, char*);
			ft_putstr_fd(str, STDERR_FILENO);
			format += 2;
		}
		else if (format[0] == '%' && format[1] == 'c')
		{
			c = va_arg(ap, int);
			ft_putchar_fd(c, STDERR_FILENO);
			format += 2;
		}
		else
		{
			ft_putchar_fd(*format, STDERR_FILENO);
			format++;
		}
	}
}

/*
** \brief   errorf helper to return an status code and print the error
*/

int			errorf_ret(int status, const char *format, ...)
{
	va_list	ap;

	va_start(ap, format);
	verrorf(format, ap);
	va_end(ap);
	return (status);
}
