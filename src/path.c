/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:51:01 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/06 17:26:05 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   path.c
** \brief  Path search
*/

#include "minishell.h"

static int		st_path_check(char exec_path[PATH_MAX + 1], bool in_path)
{
	struct stat	statbuf;

	if (stat(exec_path, &statbuf) == -1)
		return (errorf_ret(127, "%s: %s\n", exec_path, strerror(errno)));
	if (S_ISDIR(statbuf.st_mode))
		return (errorf_ret(126, "%s: is a directory\n", exec_path));
	if (!in_path && !(statbuf.st_mode & 0444))
		return (errorf_ret(126, "%s: %s\n", exec_path, strerror(EACCES)));
	return (0);
}

static bool		st_dir_search(
	char *dirname, char *exec_name, char exec_path[PATH_MAX + 1])
{
	DIR				*dir;
	struct dirent	*entry;

	if ((dir = opendir(dirname)) == NULL)
		return (false);
	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_strcmp(entry->d_name, exec_name) == 0)
		{
			ft_strcpy(exec_path, dirname);
			ft_strcat(exec_path, "/");
			ft_strcat(exec_path, exec_name);
			closedir(dir);
			return (true);
		}
	}
	closedir(dir);
	return (false);
}

int				path_search(
	t_env env, char *exec_name, char exec_path[PATH_MAX + 1], bool print)
{
	char	*current_dir;
	char	*collon;
	char	cwd[PATH_MAX + 1];

	if (ft_strchr(exec_name, '/') != NULL)
	{
		ft_strcpy(exec_path, exec_name);
		return (st_path_check(exec_path, false));
	}
	if ((current_dir = env_search(env, "PATH", NULL)) == NULL)
		return (st_dir_search(getcwd(cwd, PATH_MAX + 1), exec_name, exec_path));
	while ((collon = ft_strchr(current_dir, ':')) != NULL)
	{
		*collon = '\0';
		if (*current_dir == '\0')
			current_dir = getcwd(cwd, PATH_MAX + 1);
		if (st_dir_search(current_dir, exec_name, exec_path))
		{
			*collon = ':';
			return (st_path_check(exec_path, true));
		}
		*collon = ':';
		current_dir = collon + 1;
	}
	if (*current_dir == '\0')
		current_dir = getcwd(cwd, PATH_MAX + 1);
	if (st_dir_search(current_dir, exec_name, exec_path))
		return (st_path_check(exec_path, true));
	if (print)
		errorf("%s: command not found\n", exec_name);
	return (127);
}
