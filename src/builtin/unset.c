/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:51 by charles           #+#    #+#             */
/*   Updated: 2020/07/19 18:47:36 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   unset.c
** \brief  `unset` builtin
*/

#include "minishell.h"

int	builtin_unset(char **argv, t_env env)
{
	size_t	i;
	int		found_index;
	int		status;

	status = 0;
	i = 0;
	while (argv[++i] != NULL)
	{
		if (!utils_valid_identifier(argv[i]))
		{
			errorf("unset: `%s': not a valid identifier\n", argv[i]);
			status = 1;
			continue; // put invalid identifier
		}
		found_index = env_search_index(env, argv[i]);
		if (found_index == -1)
			continue;
		ft_vecremove(env, found_index, free);
	}
	return (status);
}
