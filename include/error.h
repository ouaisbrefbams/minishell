/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 20:34:25 by charles           #+#    #+#             */
/*   Updated: 2020/08/27 20:38:52 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include <stdarg.h>
# include "libft_io.h"

/*
** error.c
*/

typedef enum
{
	ERR_FATAL           = -1,
	ERR_NONE            = 0,
	ERR_AMBIGUOUS_REDIR = 1,
	ERR_OPEN            = 1,
	ERR_CMD_NOT_FOUND   = 127,
	ERR_SYNTAX          = 2,
}						t_err;

void					errorf(const char *format, ...);
void					verrorf(const char *format, va_list ap);

#endif
