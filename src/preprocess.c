/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/06/12 11:57:17 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_glob.h"
#include "lexer.h"

static bool		st_escapable(char c, enum e_token_tag tag)
{
	if (tag & LTAG_STR)
		return (true);
	if ((tag & LTAG_STR_DOUBLE) && (c == '\\' || c == '"' || c == '$'))
		return (true);
	return (false);
}

static char		*st_iterpolate_env(char *str, enum e_token_tag tag, t_env env)
{
	size_t		i;
	t_ftdstr	*dstr;
	char		*match;

	if ((dstr = ft_dstrwrap(str)) == NULL)
		return (NULL);
	i = -1;
	while (++i < dstr->length)
		if (dstr->str[i] == '\\' && st_escapable(dstr->str[i + 1], tag))
			ft_dstrerase(dstr, i, 1);
		else if (dstr->str[i] == '$')
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
	return (ft_dstrunwrap(dstr));
}

static char		*st_iterpolate_globs(char *str)
{
	char	**strs;
	int		i;

	if ((strs = ft_splitf(str, ' ')) == NULL)
		return (NULL);
	i = 0;
	while (strs[i] != NULL)
	{
		if (ft_strchr(strs[i], '*') != NULL
			&& (strs[i] = ms_globf(strs[i])) == NULL)
		{
			ft_split_destroy(strs);
			return (NULL);
		}
		i++;
	}
	return (ft_strsjoinf(strs, " "));
}

static int		st_splat_arg(t_ftvec *argv, int i)
{
	t_token	*splated;
	char	**strs;
	int		j;

	if ((splated = ft_vectake(argv, i)) == NULL
		|| (strs = ft_split(splated->content, ' ')) == NULL)
	{
		token_destroy(splated);
		return (-1);
	}
	j = 0;
	while (strs[j] != NULL)
	{
		if (ft_vecinsert_safe(argv, i + j, token_new(LTAG_STR, strs[j])) == NULL)
		{
			token_destroy(splated);
			ft_split_destroy(strs);
			return (-1);
		}
		j++;
	}
	token_destroy(splated);
	ft_split_destroy(strs);
	return (i + j - 1);
}

static void		st_iter_func_unwrap_token(void **addr)
{
	char	*content;

	content = (*(t_token**)addr)->content;
	free(*(t_token**)addr);
	*(char**)addr = content;
}

char			**preprocess(t_ftvec *argv, t_env env)
{
	size_t	i;
	t_token	*token;

	i = -1;
	while (++i < argv->size)
	{
		token = argv->data[i];
		if (token->tag & LTAG_STR_SINGLE)
			continue ;
		token->content = st_iterpolate_env(token->content, token->tag, env);
		if (token->tag & LTAG_STR)
		{
			if (ft_strchr(token->content, '*') != NULL)
				token->content = st_iterpolate_globs(token->content);
			if ((i = st_splat_arg(argv, i)) == (size_t)-1)
				return (NULL);
		}
	}

	t_token *next;
	i = -1;
	while (++i < argv->size - 1)
	{
		token = argv->data[i];
		while (token->tag & LTAG_STICK && i + 1 < argv->size)
		{
			next = argv->data[i + 1];
			token->content = ft_strjoinf_fst(token->content, next->content);
			if (token->content == NULL)
				return (NULL);
			token->tag &= next->tag;
			ft_vecremove(argv, i + 1, (void (*)(void*))token_destroy);
		}
	}

	ft_veciter_addr(argv, st_iter_func_unwrap_token);
	ft_vecpush(argv, NULL);
	return ((char**)ft_vecunwrap(argv));
}
