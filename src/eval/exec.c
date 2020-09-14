/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:06:11 by charles           #+#    #+#             */
/*   Updated: 2020/09/14 17:20:50 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   exec.c
** \brief  Executable name and path
*/

#include "eval.h"

/*
** \brief             Check if executable path is valid
** \param exec_path   Executable path
** \return            status code
*/

int		exec_path_check(char *exec_path)
{
	struct stat	statbuf;

	if (stat(exec_path, &statbuf) == -1)
		return (errorf_ret(127, "%s: %s\n", exec_path, strerror(errno)));
	if (S_ISDIR(statbuf.st_mode))
		return (errorf_ret(126, "%s: Is a directory\n", exec_path));
	//
	if (!(statbuf.st_mode & 0444))
		return (errorf_ret(126, "%s: %s\n", exec_path, strerror(EACCES)));
	return (0);
}

/*
** \brief              Search executable name in path
** \param path         Path hash table
** \param path_var     Path environment string in case we need to update path
** \param exec_name    Executable name to search
** \return             Executable path or NULL if not found or path update error
*/

int	exec_search_path(t_path path, char *path_var, char *exec_name, char **exec_path)
{
	if (ft_strchr(exec_name, '/') != NULL) // TODO test recursive link
	{
		*exec_path = exec_name;
		return (0);
	}
	// TODO if PATH contain empty path, consider current directory files as cmd
	if ((*exec_path = ft_htget(path, exec_name)) == NULL)
	{
		if (path_update(path, path_var) == NULL) // optimise by not updating not changed path in ht
			return (EVAL_FATAL);   // FIXME need to distiguish between malloc error and cmd not found error
		if ((*exec_path = ft_htget(path, exec_name)) == NULL)
			return (127);
	}
	return (0);
}
