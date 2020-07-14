/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:56:31 by cacharle          #+#    #+#             */
/*   Updated: 2020/07/14 10:40:59 by nahaddac         ###   ########.fr       */
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

	if ((dir = opendir(dirname)) == NULL)  // add fail safe
		return (0);
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

/* bool	utils_is_var_name(char *name) */
/* { */
/* 	if (!(ft_isalpha(*name) || *name == '_')) */
/* 		return (false); */
/* 	name++; */
/* 	while (*name != '\0) */
/* 		if (!(ft_isalnum(*name) || *name == '_')) */
/* 			return (false); */
/* 	return (true); */
/* } */

size_t	utils_var_end(char *name)
{
	size_t	i;

	if (*name == '?')
		return (2);
	if (ft_isdigit(*name))
		return (0);
	i = 0;
	while (ft_isalnum(name[i]) || name[i] == '_')
		i++;
	return (i + 1);
}
