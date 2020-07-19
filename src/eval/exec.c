/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:06:11 by charles           #+#    #+#             */
/*   Updated: 2020/07/18 09:39:53 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   exec.c
** \brief  Executable name and path
*/

#include "eval.h"

/*
** \brief             Check if executable name is already a path
** \param exec_name   Executable name
** \return            True if valid
*/

/* bool	exec_is_path(char *exec_name) */
/* { */
/* 	return (ft_strncmp(exec_name, "../", 3) == 0 */
/* 			|| ft_strncmp(exec_name, "./", 2) == 0 */
/* 			|| ft_strncmp(exec_name, "/", 1) == 0); */
/* } */

/*
** \brief             Check if executable path is valid
** \param exec_path   Executable path
** \return            True if valid
*/

bool	exec_is_valid(char *exec_path)
{
	struct stat	statbuf;

	if (stat(exec_path, &statbuf) != 0)
		return (false);
	if (!S_ISREG(statbuf.st_mode)) // also need to manage link
		return (false);
	// could test permission but probably handled by execve
	return (true);
}

/*
** \brief              Search executable name in path
** \param path         Path hash table
** \param path_var     Path environment string in case we need to update path
** \param exec_name    Executable name to search
** \return             Executable path or NULL if not found or path update error
*/

char	*exec_search_path(t_path path, char *path_var, char *exec_name)
{
	char			*exec_path;

	if (ft_strchr(exec_name, '/') != NULL) // TODO test recursive link
		return (exec_name);
	// TODO if PATH contain empty path, consider current directory files as cmd
	if ((exec_path = ft_htget(path, exec_name)) == NULL)
	{
		if (path_update(path, path_var) == NULL) // optimise by not updating not changed path in ht
			return (NULL);   // FIXME need to distiguish between malloc error and cmd not found error
		if ((exec_path = ft_htget(path, exec_name)) == NULL)
			return (NULL);
	}
	return (exec_path);
}
