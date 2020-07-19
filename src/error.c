/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 11:02:52 by charles           #+#    #+#             */
/*   Updated: 2020/07/19 15:34:20 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

static t_error	g_errors[] =
{
	{ERROR_AMBIGUOUS_REDIR, 1, "ambiguous redirect"},
	{ERROR_OPEN, 1, NULL},
	{ERROR_CMD_NOT_FOUND, 127, "command not found"},
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
	ft_putstr_fd(g_basename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
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

void			error_put_invalid_identifier(char *prefix, char *identifier)
{
	ft_putstr_fd(g_basename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(prefix, STDERR_FILENO);
	ft_putstr_fd(": `", STDERR_FILENO);
	ft_putstr_fd(identifier, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}
