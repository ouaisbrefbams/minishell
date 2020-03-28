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

#include "minishell.h"

#define MS_PATH_HT_SIZE 4096

/*
** Update `path` with all files in the directory named `dirname`.
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

/*
** Update (or create if `path` is NULL) `path` according to `path_str`
** (each directory is separated by a ':').
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
