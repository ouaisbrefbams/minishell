/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:51:01 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/28 12:33:31 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
** \file   path.c
** \brief  Path hash table manipulation
*/

#include "minishell.h"

/**
** \brief  Number of buckets of a path hash table
*/

#define MS_PATH_HT_SIZE 4096

/**
** \brief          Update `path` with all files in the directory named `dirname`.
** \param path     Path hash table
** \param dirname  directory name
** \return         Same path or NULL on error
*/

static t_path			st_path_dir_update(t_path path, char *dirname)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*tmp;

	if ((dir = opendir(dirname)) == NULL)
		return (NULL);
	while ((entry = readdir(dir)) != NULL)
	{
		if ((tmp = ft_strjoin3(dirname, "/", entry->d_name)) == NULL)
			return (NULL);
		if (ft_htset(path, entry->d_name, tmp, ms_ht_del_str_entry) == NULL)
			return (NULL);
	}
	if (closedir(dir) == -1)
		return (NULL);
	return (path);
}

/**
** \brief           Update the path
** \param path		Path hash table or NULL to create a new one
** \param path_var  PATH environment variable where each directory is separated by a ':'
** \return          The updated/created path hash table or NULL on error
*/

t_path					ms_path_update(t_path path, char *path_var)
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
		if (st_path_dir_update(path, dirs[i]) == NULL)
			return (ft_split_destroy(dirs));
	ft_split_destroy(dirs);
	return (path);
}
