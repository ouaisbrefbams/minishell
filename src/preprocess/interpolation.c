/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpolation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <me@cacharle.xyz>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 15:27:46 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/10 10:26:43 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "lexer.h"
#include "minishell.h"

static t_tok_lst	*st_field_split(char *str)
{
	t_tok_lst	*ret;
	char		*match;

	ret = NULL;
	while (*str != '\0')
	{
		if ((match = ft_strchr(str, ' ')) == NULL)
		{
			if (tok_lst_push_front(&ret, tok_lst_new(TAG_STR, str)) == NULL)
				return (tok_lst_destroy(&ret, free));
			break ;
		}
		if (tok_lst_push_front(
				&ret, tok_lst_new_until(TAG_STR, str, match - str)) == NULL)
			return (tok_lst_destroy(&ret, free));
		while (*++match == ' ')
			;
		str = match;
		if (*str == '\0' &&
			tok_lst_push_front(&ret, tok_lst_new(TAG_STR, str)) == NULL)
			return (tok_lst_destroy(&ret, free));
	}
	return ((t_tok_lst *)ft_lstreverse_ret((t_ftlst *)ret));
}

#define BEFORE 0
#define MATCH 1
#define AFTER 2

static bool			st_make_strs(char *strs[3], t_env env, char *str, size_t i)
{
	size_t	var_len;

	var_len = env_key_len(&str[i + 1], true) + 1;
	if ((strs[MATCH] = env_search_first_match(env, &str[i + 1])) == NULL)
	{
		ft_memmove(&str[i], &str[i + var_len],
				ft_strlen(&str[i + var_len]) + 1);
		return (false);
	}
	str[i] = '\0';
	strs[BEFORE] = str;
	strs[AFTER] = &str[i + var_len];
	return (true);
}

static size_t		st_merge_fields_in_curr(
	char *strs[3], t_tok_lst **curr, t_tok_lst *fields, size_t len)
{
	t_tok_lst	*last;

	last = tok_lst_last(fields);
	last->tag = (*curr)->tag;
	(*curr)->tag = TAG_STR;
	(*curr)->content = ft_strjoinf_snd(strs[BEFORE], fields->content);
	last->content = ft_strjoinf_fst(last->content, strs[AFTER]);
	free(strs[BEFORE]);
	last->next = (*curr)->next;
	(*curr)->next = fields->next;
	(*curr) = last;
	free(fields);
	return (len);
}

static size_t		st_interpolate_non_quoted(
	char *strs[3], t_tok_lst **curr, size_t i, enum e_tok prev_tag)
{
	t_tok_lst	*fields;
	size_t		len;

	fields = st_field_split(strs[MATCH]);
	if (fields == NULL)
		return (i);
	len = ft_strlen(tok_lst_last(fields)->content);
	if (!(prev_tag & TAG_STICK) && *strs[BEFORE] == '\0' &&
		*fields->content == '\0')
		ft_lstpop_front((t_ftlst **)&fields, free);
	if (!((*curr)->tag & TAG_STICK) && *strs[AFTER] == '\0' &&
		*tok_lst_last(fields)->content == '\0')
		ft_lstpop_back((t_ftlst **)&fields, free);
	if (fields != NULL && fields->next == NULL)
	{
		(*curr)->content =
			ft_strjoin3(strs[BEFORE], fields->content, strs[AFTER]);
		free(strs[BEFORE]);
		tok_lst_destroy(&fields, free);
		return (i + len);
	}
	else if (fields != NULL)
		return (st_merge_fields_in_curr(strs, curr, fields, len));
	return (i);
}

size_t				interpolate(
	void *ptrs[2], size_t i, enum e_tok prev_tag, t_env env)
{
	char		*strs[3];
	char		*str;
	t_tok_lst	**curr;

	str = ptrs[INTERPOLATION_STR];
	curr = ptrs[INTERPOLATION_CURR];
	if (!st_make_strs(strs, env, str, i))
		return (i);
	if ((*curr)->tag & TAG_STR_DOUBLE)
	{
		(*curr)->content = ft_strjoin3(strs[BEFORE], strs[MATCH], strs[AFTER]);
		free(strs[BEFORE]);
		return (i + ft_strlen(strs[MATCH]));
	}
	return (st_interpolate_non_quoted(strs, curr, i, prev_tag));
}
