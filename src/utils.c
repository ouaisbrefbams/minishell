/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:56:31 by cacharle          #+#    #+#             */
/*   Updated: 2020/04/03 14:58:17 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   utils.c
** \brief  Various functions
*/

#include "minishell.h"

int		utils_directory_iter(
	char *dirname,
	void *param,
	int (*f)(char*, struct dirent*, void*)
)
{
	DIR				*dir;
	struct dirent	*entry;

	if ((dir = opendir(dirname)) == NULL)
		return (-1);
	while ((entry = readdir(dir)) != NULL)
		if (f(dirname, entry, param) == -1)
		{
			closedir(dir);
			return (-1);
		}
	if (closedir(dir) == -1)
		return (-1);
	return (0);

}

/* int		utils_not_alnum */
