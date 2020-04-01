/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:11:34 by charles           #+#    #+#             */
/*   Updated: 2020/04/01 22:37:47 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   export.c
** \brief  `export` builtin
*/

#include "minishell.h"

int	builtin_export(char **argv, t_env env)
{
	char	*tmp;

	if (ft_strchr(argv[1], '=') == NULL)
		return (1);
	if ((tmp = ft_strdup(argv[1])) == NULL)
		return (2);
	if (ft_vecpush(env, tmp) == NULL)
		return (2); // internal error code
	return (0);
}
