/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 11:02:52 by charles           #+#    #+#             */
/*   Updated: 2020/06/14 12:19:14 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

static t_error	g_errors[] =
{
	{ERROR_AMBIGUOUS_REDIR, 1, "ambiguous redirect"},
	{ERROR_OPEN, 1, NULL},
	{ERROR_CMD_NOT_FOUND, 127, "command not found"},
	{ERROR_SYNTAX, 2, "syntax error near unexpected token "},
};

t_error			*st_error_get(enum e_error id)
{
	size_t	i;
	t_error	*match;

	match = NULL;
	i = 0;
	while (i < sizeof(g_errors) / sizeof(t_error))
	{
		if (g_errors[i].id == id)
			match = &g_errors[i];
		i++;
	}
	return (match);
}

void			error_eval_put(enum e_error id, char *unexpected)
{
	t_error	*err;

	err = st_error_get(id);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(unexpected, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (err->msg == NULL)
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	else
		ft_putendl_fd(err->msg, STDERR_FILENO);
}

int				error_status(enum e_error id)
{
	return (st_error_get(id)->status);
}
