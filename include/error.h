/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 20:34:25 by charles           #+#    #+#             */
/*   Updated: 2020/09/10 20:29:17 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include <stdarg.h>
# include "libft_io.h"

/*
** error.c
*/

# define EVAL_FATAL 1024

void					errorf(const char *format, ...);
void					verrorf(const char *format, va_list ap);
int						errorf_ret(int err, const char *format, ...);

#endif
