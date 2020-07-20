/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_glob.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/05 11:44:07 by charles           #+#    #+#             */
/*   Updated: 2020/07/20 17:14:04 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_glob.h"

/*
** \brief    Match vector start size
** \note     From ~: average file in directory is 8
**           From /: average file in directory is 15
*/

#define MATCHES_VEC_START_SIZE 16

static char g_glob_path[PATH_MAX];

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
	struct dirent *entry,
	struct s_glob_param *param,
	char *subdir_pattern
)
{
	t_ftvec	*subdir_matches;
	size_t	i;

	subdir_matches = glob_matches(subdir_pattern);
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
	int		ret;

	(void)dirname;
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
		if (entry->d_type != DT_DIR && entry->d_type != DT_LNK)
		{
			subdir_pattern[-1] = '/';
			return (0);
		}
		ft_strcat3(g_glob_path, "/", entry->d_name);
		ret = glob_iter_subdir(entry, param, subdir_pattern);
		*ft_strrchr(g_glob_path, '/') = '\0';
		return (ret);
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
	struct s_glob_param	param;

	param.pattern = pattern;
	if ((param.matches = ft_vecnew(MATCHES_VEC_START_SIZE)) == NULL)
		return (NULL);
	if (utils_directory_iter(g_glob_path, &param, (t_directory_iter_func)glob_iter) == -1)
	{
		ft_vecdestroy(param.matches, free);
		return (NULL);
	}
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
	bool	absolute;
	size_t	i;

	absolute = *pattern == '/';
	if (absolute)
		ft_strcpy(g_glob_path, "/");
	else if (getcwd(g_glob_path, PATH_MAX) == NULL)
		return (NULL);
	if ((matches = glob_matches(absolute ? pattern + 1 : pattern)) == NULL)
		return (NULL);
	if (absolute)
	{
		i = -1;
		while (++i < matches->size)
			if ((matches->data[i] = ft_strjoinf("/", matches->data[i], FT_STRJOINF_SND)) == NULL)
			{
				ft_vecdestroy(matches, free);
				return (NULL);
			}
	}
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

/*
** \brief  Wrapper around `ms_glob` which free `pattern` for convenience
*/

char			*ms_globf(char *pattern)
{
	char	*ret;

	ret = ms_glob(pattern);
	free(pattern);
	return (ret);
}
