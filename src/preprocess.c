/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/08/28 17:42:02 by charles          ###   ########.fr       */
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

char	**st_tokens_to_argv(t_tok_lst *tokens)
{
	char	**ret;
	size_t	i;

	ret = ft_calloc(ft_lstsize((t_ftlst*)tokens) + 1, sizeof(char*));
	if (ret == NULL)
		return (NULL);
	i = 0;
	while (tokens != NULL)
	{
		ret[i++] = tokens->content;
		tokens = tokens->next;
	}
	tok_lst_destroy(&tokens, NULL);
	return (ret);
}

char			**preprocess(t_tok_lst **tokens, t_env env)
{
	t_tok_lst *curr;
	enum e_tok prev_tag;

	prev_tag = 0;
	curr = *tokens;
	while (curr != NULL)
	{
		if (curr->tag & (TAG_STR | TAG_STR_DOUBLE))
		{
			char *str = curr->content;

			size_t	i = -1;
			while (str[++i] != '\0')
			{
				str = curr->content;
				// escape
				if (str[i] == '\\'
						&& (curr->tag & TAG_STR
							|| ((curr->tag & TAG_STR_DOUBLE) && ft_strchr("\\\"$", str[i + 1]))))
				{
					ft_memmove(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
					continue;
				}

				// interpolate
				if (str[i] == '$')
				{
					char	*match;
					size_t	var_len = utils_var_end(&str[i + 1]);

					if ((match = env_search_first_match(env, &str[i + 1])) == NULL)
					{
						ft_memmove(&str[i], &str[i + var_len], var_len );
						i--;
						continue;
					}

					char *before;
					char *after;
					size_t len;

					str[i] = '\0';
					before = str;
					after = &str[i + var_len];
					if (curr->tag & TAG_STR)
					{
						t_tok_lst *fields = st_field_split(match);
						if (fields != NULL)
						{
							len = ft_strlen(tok_lst_last(fields)->content);

							if (!(prev_tag & TAG_STICK) && *before == '\0' && *fields->content == '\0')
								ft_lstpop_front((t_ftlst**)&fields, free);
							if (!(curr->tag & TAG_STICK) && *after == '\0'
									&& *tok_lst_last(fields)->content == '\0')
								ft_lstpop_back((t_ftlst**)&fields, free);

							if (fields == NULL)
								// delete curr?
								;
							else if (fields->next == NULL)
							{
								curr->content = ft_strjoin3(before, fields->content, after);
								i += len - 1;
							}
							else
							{
								curr->content = ft_strjoin(before, fields->content);
								tok_lst_last(fields)->content =
									ft_strjoin(tok_lst_last(fields)->content, after);

								t_tok_lst *tmp = curr->next;
								curr->next = fields->next;
								curr = tok_lst_last(fields);
								curr->next = tmp;
								i = len - 1;
								str = curr->content;
							}
						}
						/* else */
						/* 	printf("yo\n"); */
							/* i+=10; */
					}
					else if (curr->tag & TAG_STR_DOUBLE)
					{
						curr->content = ft_strjoin3(before, match, after);
						i += ft_strlen(match) - 1;
					}
				}
			}
		}
		prev_tag = curr->tag;
		curr = curr->next;
	}

	st_stick_tokens(*tokens);
	return (st_tokens_to_argv(*tokens));
}

// TODO malloc error vs shell error
char		*preprocess_filename(t_tok_lst **tokens, t_env env)
{
	char	**strs;
	char	*ret;

	if ((strs = preprocess(tokens, env)) == NULL
		|| strs[0] == NULL)
	{
		free(strs);
		return (NULL);
	}
	if (strs[1] != NULL)
	{
		/* save tokens */
		errorf("%s: ambiguous redidrect\n", strs[1]);
		ft_split_destroy(strs);
		return (NULL);
	}
	ret = strs[0];
	free(strs);
	return (ft_strdup(ret));
}
