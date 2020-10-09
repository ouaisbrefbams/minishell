/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpolation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <me@cacharle.xyz>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 15:27:46 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/09 15:32:33 by cacharle         ###   ########.fr       */
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
		if (tok_lst_push_front(&ret,
				tok_lst_new_until(TAG_STR, str, match - str)) == NULL)
			return (tok_lst_destroy(&ret, free));
		while (*++match == ' ')
			;
		str = match;
		if (*str == '\0' && tok_lst_push_front(&ret,
				tok_lst_new(TAG_STR, str)) == NULL)
			return (tok_lst_destroy(&ret, free));
	}
	return ((t_tok_lst *)ft_lstreverse_ret((t_ftlst *)ret));
}

size_t				interpolate(
	char *str, size_t i, t_tok_lst **curr_addr, enum e_tok prev_tag, t_env env)
{
	char		*match;
	size_t		var_len;
	t_tok_lst	*curr;
	char		*before;
	char		*after;
	size_t		len;
	t_tok_lst	*fields;
	t_tok_lst	*last;
	t_tok_lst	*tmp;

	curr = *curr_addr;
	var_len = env_key_len(&str[i + 1], true) + 1;
	if ((match = env_search_first_match(env, &str[i + 1])) == NULL)
	{
		ft_memmove(&str[i], &str[i + var_len], ft_strlen(&str[i + var_len]) + 1);
		return (i);
	}
	str[i] = '\0';
	before = str;
	after = &str[i + var_len];
	if (curr->tag & TAG_STR_DOUBLE)
	{
		curr->content = ft_strjoin3(before, match, after);
		free(before);
		return (i + ft_strlen(match));
	}
	if (curr->tag & TAG_STR)
	{
		fields = st_field_split(match);
		if (fields == NULL)
			return (i);
		last = tok_lst_last(fields);
		len = ft_strlen(last->content);
		if (!(prev_tag & TAG_STICK) && *before == '\0' && *fields->content == '\0')
			ft_lstpop_front((t_ftlst **)&fields, free);
		if (!(curr->tag & TAG_STICK) && *after == '\0' && *last->content == '\0')
			ft_lstpop_back((t_ftlst **)&fields, free);
		if (fields == NULL)
			;
		else if (fields->next == NULL)
		{
			curr->content = ft_strjoin3(before, fields->content, after);
			free(before);
			tok_lst_destroy(&fields, free);
			return (i + len);
		}
		else
		{
			last = tok_lst_last(fields);
			last->tag = curr->tag;
			curr->tag = TAG_STR;
			curr->content = ft_strjoinf_snd(before, fields->content);
			last->content = ft_strjoinf_fst(last->content, after);
			free(before);
			tmp = curr->next;
			curr->next = fields->next;
			(*curr_addr) = last;
			(*curr_addr)->next = tmp;
			free(fields);
			return (len);
		}
	}
	return (i);
}
