/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:10:51 by charles           #+#    #+#             */
/*   Updated: 2020/09/15 17:46:55 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   unset.c
** \brief  `unset` builtin
*/

#include "minishell.h"

/*
** \brief       Remove variables from the environment
** \param argv  arguments
** \param env   environment
** \return      a status code or EVAL_FATAL on fatal error
*/

int	builtin_unset(char **argv, t_env env)
{
	size_t	i;
	size_t	found_index;
	int		status;

	status = 0;
	i = 0;
	while (argv[++i] != NULL)
	{
		if (*argv[i] == '\0'
			|| env_key_len(argv[i], false) != ft_strlen(argv[i]))
		{
			errorf("unset: `%s': not a valid identifier\n", argv[i]);
			status = 1;
			continue ;
		}
		if (env_search(env, argv[i], &found_index) == NULL)
			continue ;
		ft_vecremove(env, found_index, free);
	}
	return (status);
}
