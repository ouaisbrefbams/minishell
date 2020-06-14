/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:11:01 by charles           #+#    #+#             */
/*   Updated: 2020/06/14 12:52:12 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file      builtin.c
** \brief     Builtin functions
*/

#include "minishell.h"

/*
** \brief      Array storing builtin executable name and associated functions
*/

static struct s_builtin_entry	g_builtin_lookup[] = {
	{"echo", builtin_echo},
	{"cd", builtin_cd},
	{"pwd", builtin_pwd},
	{"export", builtin_export},
	{"unset", builtin_unset},
	{"env", builtin_env},
	{"exit", builtin_exit},
};

t_builtin_func					builtin_search_func(char *name)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_builtin_lookup) / sizeof(struct s_builtin_entry))
	{
		if (ft_strcmp(g_builtin_lookup[i].name, name) == 0)
			return (g_builtin_lookup[i].func);
		i++;
	}
	return (NULL);
}
