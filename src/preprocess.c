/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/06/09 17:03:37 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_glob.h"
#include "lexer.h"

static char	*iterpolate(char *str, t_env env)
{
	size_t		i;
	t_ftdstr	*dstr;
	char		*match;

	if ((dstr = ft_dstrnew(str)) == NULL)
		return (NULL);
	free(str);
	i = 0;
	while (i < dstr->length)
	{
		if (dstr->str[i] == '$')
		{
			if ((match = env_search_first_match(env, dstr->str + i + 1)) == NULL)
			{
				ft_dstrerase(dstr, i, utils_var_end(dstr->str + i + 1));
				i--;
			}
			else
			{
				if (ft_dstrsubstitute(dstr, match, i, utils_var_end(dstr->str + i + 1)) == NULL)
				{
					ft_dstrdestroy(dstr);
					return (NULL);
				}
				i += ft_strlen(match) - 1;
			}
		}
		i++;
	}
	return (ft_dstrunwrap(dstr));
}

static char		*iterpolate_globs(char *str)
{
	char	**strs;
	int		i;

	if ((strs = ft_split(str, ' ')) == NULL)
		return (NULL);
	i = 0;
	while (strs[i] != NULL)
	{
		if (ft_strchr(strs[i], '*') != NULL
			&& (strs[i] = ms_glob(strs[i])) == NULL)
		{
			ft_split_destroy(strs);
			return (NULL);
		}
		i++;
	}
	return (ft_strsjoinf(strs, " "));
}

static int splat_arg(t_ftvec *argv, int i)
{
	t_token	*splated;
	char	**strs;
	int		j;

	if ((splated = ft_vectake(argv, i)) == NULL
		|| (strs = ft_split(splated->content, ' ')) == NULL)
		return (-1);
	j = 0;
	while (strs[j] != NULL)
	{
		if (ft_vecinsert_safe(argv, i + j, token_new(LTAG_STR, strs[j])) == NULL)
		{
			ft_split_destroy(strs);
			return (-1);
		}
		j++;
	}
	ft_split_destroy(strs);
	return i + j - 1;
}

void		iter_func_unwrap_token(void **addr)
{
	char	*content;

	content = (*(t_token**)addr)->content;
	free(*(t_token**)addr);
	*(char**)addr = content;
}

char		**preprocess_argv(t_ftvec *argv, t_env env)
{
	size_t	i;
	t_token	*token;

	i = -1;
	while (++i < argv->size)
	{
		token = argv->data[i];
		if (token->tag == LTAG_STR_SINGLE)
			continue ;
		token->content = iterpolate(token->content, env);
		if (token->tag == LTAG_STR)
		{
			if (ft_strchr(token->content, '*') != NULL)
				token->content = iterpolate_globs(token->content);
			if ((i = splat_arg(argv, i)) == (size_t)-1)
				return (NULL);
		}
	}
	ft_veciter_addr(argv, iter_func_unwrap_token);
	ft_vecpush(argv, NULL);
	return ((char**)ft_vecunwrap(argv));
}
