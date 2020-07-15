/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:51:01 by cacharle          #+#    #+#             */
/*   Updated: 2020/07/15 12:11:48 by charles          ###   ########.fr       */
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
** \return         Same path or NULL on error
*/

static int			st_add_file(char *dirname, struct dirent *entry, void *path)
{
	char	*filepath;

	if ((filepath = ft_strjoin3(dirname, "/", entry->d_name)) == NULL ||
		ft_htset((t_path)path, entry->d_name, filepath, free) == NULL)
	{
		free(filepath);
		return (-1);
	}
	return (0);
}

/*
** \brief           Update the path
** \param path		Path hash table or NULL to create a new one
** \param path_var  PATH environment variable where
**                  each directory is separated by a ':'
** \return          The updated/created path hash table or NULL on error
*/

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
	while (dirs[++i] != NULL)  // carefull with non existant dir error
		if (utils_directory_iter(dirs[i], path, st_add_file) == -1)
			return (ft_split_destroy(dirs));
	ft_split_destroy(dirs);
	return (path);
}
