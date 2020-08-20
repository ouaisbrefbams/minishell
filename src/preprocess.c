/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/08/20 14:24:51 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "eval.h"

static t_ftlst	*st_field_split(char *str)
{
	char	**strs;
	t_ftlst	*ret;
	t_ftlst	*node;
	int		i;

	if ((strs = ms_split_notrim(str, ' ')) == NULL)
		return (NULL);
	ret = NULL;
	i = 0;
	while (strs[i] != NULL)
	{
		if ((node = ft_lstnew(token_new(TAG_STR, strs[i]))) == NULL)
		{
			free(strs);
			return (NULL);
		}
		ft_lstpush_back(&ret, node);
		i++;
	}
	free(strs);
	return (ret);
}

void token_debug(void *v);

// FIXME field split space before/after when arg not sticked
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
						/* printf("fields \n"); */
						/* ft_lstiter(fields, token_debug); */
						/* printf("f %p\n", fields); */
						/* printf("f %s\n", token_content(fields)); */

						len = ft_strlen(token_content(ft_lstlast(fields)));
						/* printf("l %d\n", len); */

						/* printf("prev %d\n", prev_tag & TAG_STICK); */
						if (!(prev_tag & TAG_STICK) && *before == '\0' && *token_content(fields) == '\0')
						{
							/* printf("yo\n"); */
							ft_lstpop_front(&fields, NULL);
						}
						if (!(token_tag(curr) & TAG_STICK) && *after == '\0' && *token_content(ft_lstlast(fields)) == '\0')
							ft_lstpop_back(&fields, NULL);
						/* ft_lstiter(fields, token_debug); */

						if (fields == NULL)
							// delete curr
							;
						else if (fields->next == NULL)
						{
							/* printf("yo\n"); */
							token_set_content(curr, ft_strjoin3(before, token_content(fields), after));
							i += len - 1;
						}
						else
						{

							/*
							** A='  bonjour je suis '
							** echo $A
							** curr = $A
							** fs = '' 'bonjour' 'je' 'suis' ''
							*/

							/* if (*before != '\0' && */

							token_set_content(curr, ft_strjoin(before, token_content(fields)));
							token_set_content(ft_lstlast(fields),
											  ft_strjoin(token_content(ft_lstlast(fields)), after));

							t_ftlst *tmp = curr->next;
							curr->next = fields->next;
							curr = ft_lstlast(fields);
							curr->next = tmp;
							i = len - 1;
						str = token_content(curr);
						/* printf("%d\n", i); */
						/* i += len - 1; */
						/* printf("%d\n", i); */
						/* printf(">  |%s|\n", str); */
						/* printf(">> |%s|\n", str + i); */
						}
					}
					else if (token_tag(curr) & TAG_STR_DOUBLE)
					{
						token_set_content(curr, ft_strjoin3(before, match, after));
						/* printf(">%s< %d\n", match, ft_strlen(match)); */
						i += ft_strlen(match) - 1;
						/* printf(">  %zu %s\n", i, str); */
					}
				}
			}
		}
		prev_tag = token_tag(curr);
		curr = curr->next;
		/* if (curr != NULL) */
		/* { */
		/* prev_tag = token_tag(curr); */
		/* token_debug(curr->data); */
		/* } */
	}

	/* printf("=====================AFTER\n"); */
	/* ft_lstiter(*tokens, token_debug); */

	curr = *tokens;
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

	char **ret = malloc(sizeof(char*) * (ft_lstsize(*tokens) + 1));
	curr = *tokens;
	size_t	i = 0;
	while (curr != NULL)
	{
		ret[i] = token_content(curr);
		i++;
		curr = curr->next;
	}
	ret[i] = NULL;
	ft_lstdestroy(tokens, NULL);
	return ret;
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
