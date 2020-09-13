/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:11:01 by charles           #+#    #+#             */
/*   Updated: 2020/09/13 20:18:32 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file      builtin.c
** \brief     Builtin functions
*/

#include "minishell.h"

/*
** \brief      Array storing builtin executable name, associated functions
**             and if they should be executed in a fork
*/

static t_builtin_entry	g_builtin_lookup[] = {
	{"echo", builtin_echo, true},
	{"cd", builtin_cd, false},
	{"pwd", builtin_pwd, true},
	{"export", builtin_export, false},
	{"unset", builtin_unset, false},
	{"env", builtin_env, false},
	{"exit", builtin_exit, false},
};

/*
** \brief       Search a builtin by name in g_builtin_lookup
** \param name  Name of the searched builtin
** \return      the builtin entry of the associated name
**              or NULL if name is not a builtin.
*/

t_builtin_entry			*builtin_search_func(char *name)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_builtin_lookup) / sizeof(t_builtin_entry))
	{
		if (ft_strcmp(g_builtin_lookup[i].name, name) == 0)
			return (&g_builtin_lookup[i]);
		i++;
	}
	return (NULL);
}
