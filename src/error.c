/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 11:02:52 by charles           #+#    #+#             */
/*   Updated: 2020/09/10 14:48:19 by charles          ###   ########.fr       */
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

static int		g_error_to_status[] = {
	[ERR_NONE]            = 0,
	[ERR_AMBIGUOUS_REDIR] = 1,
	[ERR_OPEN]            = 1,
	[ERR_CMD_NOT_FOUND]   = 127,
	[ERR_SYNTAX]          = 2,
	[ERR_IS_DIRECTORY]    = 126,
	[ERR_ERRNO]           = 126,
};

int				error_get_status(int status)
{
	return (g_error_to_status[status]);
}

void			error_set_status(int status)
{
	if (status == ERR_FATAL)
		exit(1);
	/* printf("%d\n", status); */
	if (status < ERR_NONE)
		g_last_status = status;
	else
		g_last_status = g_error_to_status[status];
}
