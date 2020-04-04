/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/04/04 18:30:10 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 8 en moyenne
#define MATCHES_VEC_DEFAULT_SIZE 32

struct s_glob_param
{
	char	*pattern;
	t_ftvec	*matches;
};

t_ftvec	*glob_matches(char *pattern);


int		glob_iter(char *dirname, struct dirent *entry, void *void_param)
{
	struct s_glob_param	*param;
	char				*subdir_pattern;
	char				*subdir_name;
	t_ftvec				*subdir_matches;

	param = void_param;
	/* printf("DIR: %15s %d\n", entry->d_name, entry->d_type == DT_DIR); */
	if (param->pattern[0] != '.' && entry->d_name[0] == '.')
		return (0);
	if ((subdir_pattern = ft_strchr(param->pattern, '/')) != NULL)
	{
		*subdir_pattern = '\0';
		subdir_pattern++;
	}
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
			if (ft_vecpush_safe(param->matches, ft_strdup(entry->d_name)) == NULL)
				return (-1);
			subdir_pattern[-1] = '/';
			return (0);
		}

		if ((subdir_name = ft_strjoin3(dirname, "/", entry->d_name)) == NULL)
			return (-1);
		chdir(subdir_name);
		free(subdir_name);
		subdir_matches = glob_matches(subdir_pattern);
		chdir(dirname);
		if (subdir_matches == NULL)
			return (-1);
		for (size_t i = 0; i < subdir_matches->size; i++)
		{
			if (ft_vecpush_safe(param->matches,
					ft_strjoin3(entry->d_name, "/", subdir_matches->data[i])) == NULL)
			{
				ft_vecdestroy(subdir_matches, free);
				return (-1);
			}
		}
		ft_vecdestroy(subdir_matches, free);
		subdir_pattern[-1] = '/';
	}
	else
	{
		if (ft_vecpush_safe(param->matches, ft_strdup(entry->d_name)) == NULL)
			return (-1);
	}
	return (0);
}

t_ftvec	*glob_matches(char *pattern)
{
	char				dirname[PATH_MAX];
	struct s_glob_param	param;

	if (getcwd(dirname, PATH_MAX) == NULL)
		return (NULL);
	if ((param.pattern = ft_strdup(pattern)) == NULL ||
		(param.matches = ft_vecnew(MATCHES_VEC_DEFAULT_SIZE)) == NULL)
	{
		free(param.pattern);
		return (NULL);
	}
	if (utils_directory_iter(dirname, &param, glob_iter) == -1)
	{
		free(param.pattern);
		ft_vecdestroy(param.matches, free);
		return (NULL);
	}
	free(param.pattern);
	return (param.matches);
}

char	*ms_glob(char *pattern)
{
	char	*join;
	t_ftvec	*matches;

	if ((matches = glob_matches(pattern)) == NULL)
		return (NULL);
	ft_vecsort(matches, ft_compar_str);
	if (ft_vecpush(matches, NULL) == NULL ||
		(join = ft_strsjoin((char**)matches->data, "\n")) == NULL)
	{
		ft_vecdestroy(matches, free);
		return (NULL);
	}
	ft_vecdestroy(matches, free);
	return (join);
}

/* void	iterpolate_iter_f(char **curr) */
/* { */
/* 	if (*curr != '$') */
/* 		return (i + 1); */
/* } */


char	*preprocess(char *input, t_env env)
{
	/* int			i; */
	/* t_ftdstr	*input_dstr; */
	/* char		*glob_str; */
	/* char		*tmp; */

	(void)input;
	(void)env;
	/* glob_str = NULL; */
	/* if ((input_dstr = ft_dstrnew(input)) == NULL) */
	/* 	return (NULL); */
	/* i = 0; */
	/* while (input_dstr->str[i] != '\0') */
	/* { */
		/* if (input_dstr->str[i] == '*') */
		/* { */
		/* 	free(glob_str); */
			/* if ((glob_str = ms_glob()) = NULL) */
			/* { */
			/* 	ft_dstrdestroy(input_str); */
			/* 	return (NULL); */
			/* } */
		/* 	if (ft_dstrinsert(input_dstr, glob_str, 0) == NULL) */
		/* 	{ */
		/* 		free(glob_str); */
		/* 		ft_dstrdestroy(input_dstr); */
		/* 		return (NULL); */
		/* 	} */
		/* 	i += strlen(glob_str); */
		/* } */
	/* 	else if (input_dstr->str[i] == '$') */
	/* 	{ */
	/* 		if ((tmp = env_match_first(env, input_dstr->str + i + 1)) == NULL) */
	/* 			tmp = ""; */
	/* 		if (ft_dstrreplace(input_dstr, tmp, i, i + ft_strlen(tmp)) == NULL) */
	/* 		{ */
	/* 			free(glob_str); */
	/* 			ft_dstrdestroy(input_dstr); */
	/* 			return (NULL); */
	/* 		} */
	/* 		i += ft_strlen(tmp); */
	/* 	} */
	/* 	else */
	/* 		i++; */
	/* } */
	/* printf("%d %d %s\n", input_dstr->length, input_dstr->capacity, input_dstr->str); */
	/* return (ft_dstrunwrap(input_dstr)); */
	return NULL;
}
