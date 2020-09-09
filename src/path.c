/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:51:01 by cacharle          #+#    #+#             */
/*   Updated: 2020/09/09 15:49:50 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   path.c
** \brief  Path hash table manipulation
*/

#include "minishell.h"

/*
** \brief  Number of buckets of a path hash table
*/

#define MS_PATH_HT_SIZE 8192

/*
** \brief          Update `path` with all files
**                 in the directory named `dirname`.
** \param path     Path hash table
** \param dirname  directory name
** \return         -1 on error, 0 or -2 otherwise
*/

static int		st_add_directory(t_path path, char *dirname)
{
	DIR				*dir;
	struct dirent	*entry;

	if ((dir = opendir(dirname)) == NULL)
		return (-2);
	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_htset_safe(path, entry->d_name,
			ft_strjoin3(dirname, "/", entry->d_name), free) == NULL)
		{
			closedir(dir);
			return (-1);
		}
	}
	if (closedir(dir) == -1)
		return (-1);
	return (0);
}

/*
** \brief           Update the path
** \param path		Path hash table or NULL to create a new one
** \param path_var  PATH environment variable where
**                  each directory is separated by a ':'
** \return          The updated/created path hash table or NULL on error
*/

// TODO check nullstring path == current directory
// i.e ./ not needed before executable
t_path					path_update(t_path path, char *path_var)
{
	int		i;
	char	**dirs;

	if (path_var == NULL)
		return (NULL);
	if (path == NULL && (path = ft_htnew(MS_PATH_HT_SIZE)) == NULL)
		return (NULL);
	if ((dirs = ft_split(path_var, ':')) == NULL)
		return (NULL);
	i = -1;
	while (dirs[++i] != NULL)
		if (st_add_directory(path, dirs[i]) == -1)
			return (ft_split_destroy(dirs));
	ft_split_destroy(dirs);
	return (path);
}
