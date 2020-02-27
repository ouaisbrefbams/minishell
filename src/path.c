/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:51:01 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/27 18:07:16 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void				st_path_command_entry_del(t_ftht_content *content)
{
	if (content == NULL)
		return ;
	free(content->key);
	free(content->value);
}

static int				st_in_dirs(char **dirs, char *dir)
{
	while (*dirs != NULL)
		if (ft_strcmp(*dirs++, dir) == 0)
			return (TRUE); // maybe change ft_lstremove_if func to cmp instead of bool
	return (FALSE);
}

static t_path			*st_path_commands_update(t_path *path, char *dirname)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*tmp;

	if ((dir = opendir(dirname)) == NULL)
		return (NULL);
	while ((entry = readdir(dir)) != NULL)
	{
		if ((tmp = ft_strjoin(dirname, entry->d_name)) == NULL)
			return (NULL);
		if (ft_htset(path->commands, entry->d_name, st_path_command_entry_del) == NULL)
			return (NULL);
	}
	if (closedir(dir) == -1)
		return (NULL);
	return (path);
}

/*
** Update path list with `dirname`
*/

static t_path			*st_path_dir_update(t_path *path, char *dirname)
{
	t_ftlst	*front;

	if (ft_lstlfind(path->dirs, (int (*)(void*, void*))ft_strcmp, dirname) == NULL)
	{
		if ((dirname = ft_strdup(dirname)) == NULL)
			return (NULL);
		if ((front = ft_lstnew(dirname)) == NULL)
		{
			free(dirname);
			return (NULL);
		}
		ft_lstadd_front(&path->dirs, front);
	}
	return (st_path_commands_update(path, dirname));
}

/*
** Update (or create if `path` is NULL) `path` according to `path_str`
** (each directory is separated by a ':').
*/

t_path					*ms_path_update(t_path *path, const char *path_str)
{
	int		i;
	char	**dirs;

	if (path == NULL)
		if ((path = malloc(sizeof(t_path))) == NULL)
			return (NULL);
	if ((dirs = ft_split(path_str, ':')) == NULL)
		return (NULL);
	ft_lstremove_if(&path->dirs, (t_ftbool (*)(void*, void*))st_in_dirs, free, (void*)dirs);  // change t_ftbool to std bool
	i = -1;
	while (dirs[i] != NULL)
		if (st_path_dir_update(path, dirs[i]) == NULL)
			return (ft_split_destroy(dirs));
	ft_split_destroy(dirs);
	return (path);
}

void					ms_path_destroy(t_path *path)
{
	if (path == NULL)
		return ;
	ft_lstclear(&path->dirs, free);
	ft_htdestroy(path->commands, st_path_command_entry_del);
	free(path);
}
