/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/10/08 09:58:48 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "eval.h"

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
			break;
		}
		if (tok_lst_push_front(&ret, tok_lst_new_until(TAG_STR, str, match - str)) == NULL)
			return (tok_lst_destroy(&ret, free));
		while (*++match == ' ')
			;
		str = match;
		if (*str == '\0' && tok_lst_push_front(&ret, tok_lst_new(TAG_STR, str)) == NULL)
			return (tok_lst_destroy(&ret, free));
	}
	return ((t_tok_lst*)ft_lstreverse_ret((t_ftlst*)ret));
}


t_tok_lst *st_stick_tokens(t_tok_lst *tokens)
{
	t_tok_lst	*curr;

	curr = tokens;
	while (curr != NULL)
	{
		// curr->next shouldn't be null
		if (curr->tag & TAG_STICK && curr->next != NULL)
		{
			curr->content = ft_strjoinf_fst(curr->content, curr->next->content);
			t_tok_lst *tmp = curr->next->next;
			curr->tag = curr->next->tag;
			ft_lstdelone((t_ftlst*)curr->next, free);
			curr->next = tmp;
			continue;
		}
		curr = curr->next;
	}
	return (tokens);
}

char	**st_tokens_to_argv(t_tok_lst **tokens)
{
	char		**ret;
	size_t		i;
	t_tok_lst	*curr;

	curr = *tokens;
	ret = ft_calloc(ft_lstsize((t_ftlst*)curr) + 1, sizeof(char*));
	if (ret == NULL)
		return (NULL);
	i = 0;
	while (curr != NULL)
	{
		ret[i++] = curr->content;
		curr = curr->next;
	}
	tok_lst_destroy(tokens, NULL);
	return (ret);
}

bool escape(char *str, enum e_tok tag)
{
	if (str[0] == '\\' && (tag & TAG_STR
		|| ((tag & TAG_STR_DOUBLE) && ft_strchr("\\\"$", str[1]))))
	{
		ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
		return (true);
	}
	return (false);
}

size_t	interpolate(char *str, size_t i, t_tok_lst **curr_addr, enum e_tok prev_tag, t_env env)
{
	char		*match;
	size_t		var_len;
	t_tok_lst	*curr;
	char		*before;
	char		*after;
	size_t		len;
	t_tok_lst	*fields;
	t_tok_lst	*last;

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
		return i + ft_strlen(match);
	}
	if (curr->tag & TAG_STR)
	{
		fields = st_field_split(match);
		if (fields == NULL)
			return (i);

		last = tok_lst_last(fields);
		len = ft_strlen(last->content);

		if (!(prev_tag & TAG_STICK) && *before == '\0' && *fields->content == '\0')
			ft_lstpop_front((t_ftlst**)&fields, free);
		if (!(curr->tag & TAG_STICK) && *after == '\0' && *last->content == '\0')
			ft_lstpop_back((t_ftlst**)&fields, free);

		if (fields == NULL)
			;
		else if (fields->next == NULL)
		{
			curr->content = ft_strjoin3(before, fields->content, after);
			free(before);
			tok_lst_destroy(&fields, free);
			return i + len;
		}
		else
		{
			last = tok_lst_last(fields);
			last->tag = curr->tag;
			curr->tag = TAG_STR;
			curr->content = ft_strjoinf_snd(before, fields->content);
			last->content = ft_strjoinf_fst(last->content, after);
			free(before);

			t_tok_lst *tmp = curr->next;
			curr->next = fields->next;
			(*curr_addr) = last;
			(*curr_addr)->next = tmp;
			free(fields);
			return len;
		}
	}
	return i;
}

char			**preprocess(t_tok_lst **tokens, t_env env)
{
	t_tok_lst	*curr;
	enum e_tok	prev_tag;
	char		*str;
	size_t		i;

	prev_tag = 0;
	curr = *tokens;
	while (curr != NULL)
	{
		if (curr->tag & (TAG_STR | TAG_STR_DOUBLE))
		{
			i = -1;
			while ((str = curr->content) != NULL && str[++i] != '\0')
			{
				if (escape(str + i, curr->tag))
					continue ;
				if (str[i] == '$')
					i = interpolate(str, i, &curr, prev_tag, env) - 1;
			}
		}
		prev_tag = curr->tag;
		curr = curr->next;
	}
	st_stick_tokens(*tokens);
	return (st_tokens_to_argv(tokens));
}

int		preprocess_filename(t_tok_lst **tokens, t_env env, char **filename)
{
	char	**strs;

	if ((strs = preprocess(tokens, env)) == NULL
		|| strs[0] == NULL)
	{
		free(strs);
		return (EVAL_FATAL);
	}
	if (strs[1] != NULL)
	{
		errorf("%s: ambiguous redidrect\n", strs[1]);
		ft_split_destroy(strs);
		return (1);
	}
	*filename = strs[0];
	free(strs);
	return (*filename == NULL ? EVAL_FATAL : 0);
}
