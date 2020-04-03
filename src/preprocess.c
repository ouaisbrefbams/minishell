/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/04/03 14:48:57 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ms_glob(char *pattern)
{
	(void)pattern;
	return (NULL);
}

/* void	iterpolate_iter_f(char **curr) */
/* { */
/* 	if (*curr != '$') */
/* 		return (i + 1); */
/* } */


char	*preprocess(char *input, t_env env)
{
	int			i;
	t_ftdstr	*input_dstr;
	char		*glob_str;
	/* char		*tmp; */

	glob_str = NULL;
	if ((input_dstr = ft_dstrnew(input)) == NULL)
		return (NULL);
	i = 0;
	while (input_dstr->str[i] != '\0')
	{
		if (input_dstr->str[i] == '*')
		{
			free(glob_str);
			/* if ((glob_str = ms_glob()) = NULL) */
			/* { */
			/* 	ft_dstrdestroy(input_str); */
			/* 	return (NULL); */
			/* } */
			if (ft_dstrinsert(input_dstr, glob_str, 0) == NULL)
			{
				free(glob_str);
				ft_dstrdestroy(input_dstr);
				return (NULL);
			}
			i += strlen(glob_str);
		}
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
			i++;
	}
	printf("%d %d %s\n", input_dstr->length, input_dstr->capacity, input_dstr->str);
	return (ft_dstrunwrap(input_dstr));
}
