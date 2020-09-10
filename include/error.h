/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 20:34:25 by charles           #+#    #+#             */
/*   Updated: 2020/09/10 14:48:42 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include <stdarg.h>
# include "libft_io.h"

/*
** error.c
*/

enum
{
	ERR_NONE = 1024,
	ERR_FATAL,
	ERR_SYNTAX,
	ERR_OPEN,
	ERR_AMBIGUOUS_REDIR,
	ERR_CMD_NOT_FOUND,
	ERR_IS_DIRECTORY,
	ERR_ERRNO,
};

typedef int t_err;

void					errorf(const char *format, ...);
void					verrorf(const char *format, va_list ap);
void					error_set_status(int status);
int						error_get_status(int status);

#endif
