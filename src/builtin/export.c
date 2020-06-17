/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:11:34 by charles           #+#    #+#             */
/*   Updated: 2020/06/17 13:47:18 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   export.c
** \brief  `export` builtin
*/

// modify existing
// set with no string without '='

#include "minishell.h"

int	builtin_export(char **argv)
{
	char	*temp;
	size_t  i;

	if (argv[1] == NULL)
		return (4);
	if(isdigit(argv[1][0]))
		return(0);
	i = 0;
	temp = argv[1];
	while(temp[i] != '\0')
	{
		if(temp[i] == ' ' || isalnum(temp[i]) == 0)
			return(2);
		if (temp[i] == '=')
		{
			temp[i] = '\0';
			env_export(env, temp, argv[1][i + 1]);
			return(0);
		}
		i++;
	}
	return (3);
}
