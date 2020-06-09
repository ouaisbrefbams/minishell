/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_glob.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/05 11:44:07 by charles           #+#    #+#             */
/*   Updated: 2020/06/09 17:51:30 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_glob.h"

/*
** \brief    Match vector start size
** \note     From ~: average file in directory is 8
**           From /: average file in directory is 15
*/

#define MATCHES_VEC_START_SIZE 16

/*
** \brief                 Glob directory iteration function
**                        for subdirectory matches
** \param dirname         Current directory name
** \param entry           Current directory entry
** \param param           Contain matches vector and pattern to match
** \param subdir_pattern  Pattern left after '/' in parrent pattern
** \return                0 on success, -1 on error
*/

static int		glob_iter_subdir(
	char *dirname,
	struct dirent *entry,
	struct s_glob_param *param,
	char *subdir_pattern
)
{
	char	subdir_name[PATH_MAX];
	t_ftvec	*subdir_matches;
	size_t	i;

	ft_strcat3(ft_strcpy(subdir_name, dirname), "/", entry->d_name);
	chdir(subdir_name);
	subdir_matches = glob_matches(subdir_pattern);
	chdir(dirname);
	if (subdir_matches == NULL)
		return (-1);
	i = 0;
	while (i < subdir_matches->size)
		if (ft_vecpush_safe(param->matches, ft_strjoin3(entry->d_name,
						"/", subdir_matches->data[i++])) == NULL)
		{
			ft_vecdestroy(subdir_matches, free);
			return (-1);
		}
	ft_vecdestroy(subdir_matches, free);
	subdir_pattern[-1] = '/';
	return (0);
}

/*
** \brief          Glob directory iteration function
** \param dirname  Current directory name
** \param entry    Current directory entry
** \param param    Contain matches vector and pattern to match
** \return         0 on success or -1 on error
*/

static int		glob_iter(
	char *dirname,
	struct dirent *entry,
	struct s_glob_param *param
)
{
	char	*subdir_pattern;

	if (param->pattern[0] != '.' && entry->d_name[0] == '.')
		return (0);
	if ((subdir_pattern = ft_strchr(param->pattern, '/')) != NULL)
		*subdir_pattern++ = '\0';
	if (!ft_fnmatch(param->pattern, entry->d_name))
	{
		if (subdir_pattern != NULL)
			subdir_pattern[-1] = '/';
		return (0);
	}
	if (subdir_pattern != NULL)
	{
		if (entry->d_type != DT_DIR)
		{
			subdir_pattern[-1] = '/';
			return (0);
		}
		return (glob_iter_subdir(dirname, entry, param, subdir_pattern));
	}
	if (ft_vecpush_safe(param->matches, ft_strdup(entry->d_name)) == NULL)
		return (-1);
	return (0);
}

/*
** \brief          Fill a vector with all match
** \param pattern  Pattern to match
** \return         Matches vector or NULL on error
*/

t_ftvec			*glob_matches(char *pattern)
{
	char				dirname[PATH_MAX];
	struct s_glob_param	param;

	if (getcwd(dirname, PATH_MAX) == NULL)
		return (NULL);
	if ((param.pattern = ft_strdup(pattern)) == NULL ||
		(param.matches = ft_vecnew(MATCHES_VEC_START_SIZE)) == NULL)
	{
		free(param.pattern);
		return (NULL);
	}
	if (utils_directory_iter(dirname, &param,
					(t_directory_iter_func)glob_iter) == -1)
	{
		free(param.pattern);
		ft_vecdestroy(param.matches, free);
		return (NULL);
	}
	free(param.pattern);
	return (param.matches);
}

/*
** \brief          Search files which match a pattern in the current directory
** \param pattern  Pattern to match
** \return         Space separated list of match,
**                 pattern if no match found,
**                 NULL on error
*/

char			*ms_glob(char *pattern)
{
	char	*join;
	t_ftvec	*matches;

	if ((matches = glob_matches(pattern)) == NULL)
		return (NULL);
	ft_vecsort(matches, ft_compar_str);
	if (ft_vecpush(matches, NULL) == NULL ||
		(join = ft_strsjoin((char**)matches->data, " ")) == NULL)
	{
		ft_vecdestroy(matches, free);
		return (NULL);
	}
	ft_vecdestroy(matches, free);
	if (*join == '\0')
	{
		free(join);
		return (ft_strdup(pattern));
	}
	return (join);
}

char			*ms_globf(char *pattern)
{
	char	*ret;

	ret = ms_glob(pattern);
	free(pattern);
	return (ret);
}
