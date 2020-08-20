/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/08/20 17:28:28 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "eval.h"

static t_ftlst	*st_field_split(char *str)
{
	t_ftlst	*ret;
	char	*match;

	ret = NULL;
	while (*str != '\0')
	{
		if ((match = ft_strchr(str, ' ')) == NULL)
		{
			if (ft_lstpush_front_node(&ret, token_new(TAG_STR, str)) == NULL)
				return (ft_lstdestroy(&ret, free));
			break;
		}
		if (ft_lstpush_front_node(&ret, token_new_until(TAG_STR, str, match - str)) == NULL)
			return (ft_lstdestroy(&ret, free));
		while (*++match == ' ')
			;
		str = match;
		if (*str == '\0' && ft_lstpush_front_node(&ret, token_new(TAG_STR, str)) == NULL)
			return (ft_lstdestroy(&ret, free));
	}
	return (ft_lstreverse_ret(ret));
}


t_ftlst *st_stick_tokens(t_ftlst *tokens)
{
	t_ftlst	*curr;

	curr = tokens;
	while (curr != NULL)
	{
		// curr->next shouldn't be null
		if (token_tag(curr) & TAG_STICK)
		{
			token_set_content(curr, ft_strjoinf_fst(token_content(curr), token_content(curr->next)));
			t_ftlst *tmp = curr->next->next;
			token_set_tag(curr, token_tag(curr->next));
			ft_lstdelone(curr->next, free);
			curr->next = tmp;
			continue;
		}
		curr = curr->next;
	}
	return (tokens);
}

char	**st_tokens_to_argv(t_ftlst *tokens)
{
	char	**ret;
	size_t	i;

	if ((ret = ft_calloc(ft_lstsize(tokens) + 1, sizeof(char*))) == NULL)
		return (NULL);
	i = 0;
	while (tokens != NULL)
	{
		ret[i++] = token_content(tokens);
		tokens = tokens->next;
	}
	ft_lstdestroy(&tokens, free);
	return (ret);
}

char			**preprocess(t_ftlst **tokens, t_env env)
{
	t_ftlst *curr;
	enum e_token_tag prev_tag;

	prev_tag = 0;
	curr = *tokens;
	while (curr != NULL)
	{
		if (token_tag(curr) & (TAG_STR | TAG_STR_DOUBLE))
		{
			char *str = token_content(curr);

			size_t	i = -1;
			while (str[++i] != '\0')
			{
				str = token_content(curr);
				// escape
				if (str[i] == '\\'
						&& (token_tag(curr)& TAG_STR
							|| ((token_tag(curr) & TAG_STR_DOUBLE) && ft_strchr("\\\"$", str[i + 1]))))
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
					if (token_tag(curr) & TAG_STR)
					{
						t_ftlst *fields = st_field_split(match);

						len = ft_strlen(token_content(ft_lstlast(fields)));

						if (!(prev_tag & TAG_STICK) && *before == '\0' && *token_content(fields) == '\0')
							ft_lstpop_front(&fields, (void (*)(void*))token_destroy);
						if (!(token_tag(curr) & TAG_STICK) && *after == '\0' && *token_content(ft_lstlast(fields)) == '\0')
							ft_lstpop_back(&fields, (void (*)(void*))token_destroy);

						if (fields == NULL)
							// delete curr?
							;
						else if (fields->next == NULL)
						{
							token_set_content(curr, ft_strjoin3(before, token_content(fields), after));
							i += len - 1;
						}
						else
						{
							token_set_content(curr, ft_strjoin(before, token_content(fields)));
							token_set_content(ft_lstlast(fields),
											  ft_strjoin(token_content(ft_lstlast(fields)), after));

							t_ftlst *tmp = curr->next;
							curr->next = fields->next;
							curr = ft_lstlast(fields);
							curr->next = tmp;
							i = len - 1;
						str = token_content(curr);
						}
					}
					else if (token_tag(curr) & TAG_STR_DOUBLE)
					{
						token_set_content(curr, ft_strjoin3(before, match, after));
						i += ft_strlen(match) - 1;
					}
				}
			}
		}
		prev_tag = token_tag(curr);
		curr = curr->next;
	}

	st_stick_tokens(*tokens);
	return (st_tokens_to_argv(*tokens));
}

// need to free tokens
char		*preprocess_filename(t_ftlst **tokens, t_env env)
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
	return (ret);
}
