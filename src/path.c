/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:51:01 by cacharle          #+#    #+#             */
/*   Updated: 2020/09/14 20:44:39 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   path.c
** \brief  Path search
*/

#include "minishell.h"

static bool		st_dir_search(char *dirname, char *exec_name, char exec_path[PATH_MAX + 1])
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
			return (true);
		}
	}
	closedir(dir);
	return (false);
}

bool			path_search(t_env env, char *exec_name, char exec_path[PATH_MAX + 1])
{
	char	*current_dir;
	char	*collon;
	char	cwd[PATH_MAX + 1];

	if (ft_strchr(exec_name, '/') != NULL)
	{
		ft_strcpy(exec_path, exec_name);
		return (true);
	}
	/* printf("==========\n"); */
	if ((current_dir = env_search(env, "PATH")) == NULL)
		return (st_dir_search(getcwd(cwd, PATH_MAX + 1), exec_name, exec_path));
	/* printf("%s\n", current_dir); */
	while ((collon = ft_strchr(current_dir, ':')) != NULL)
	{
		*collon = '\0';
		/* printf("%s\n", current_dir); */
		if (*current_dir == '\0')
			current_dir = getcwd(cwd, PATH_MAX + 1);
		if (st_dir_search(current_dir, exec_name, exec_path))
			return (true);
		*collon = ':';
		current_dir = collon + 1;
	}
	/* printf("> %s\n", current_dir); */

	if (*current_dir == '\0')
		current_dir = getcwd(cwd, PATH_MAX + 1);
	if (st_dir_search(current_dir, exec_name, exec_path))
		return (true);
	/* printf("yo\n"); */
	return (false);
}
