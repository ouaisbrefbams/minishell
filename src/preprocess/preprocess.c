/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/10/10 10:32:53 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "lexer.h"
#include "minishell.h"

t_tok_lst			*st_stick_tokens(t_tok_lst *tokens)
{
	t_tok_lst	*curr;
	t_tok_lst	*tmp;

	curr = tokens;
	while (curr != NULL)
	{
		if (curr->tag & TAG_STICK && curr->next != NULL)
		{
			curr->content = ft_strjoinf_fst(curr->content, curr->next->content);
			tmp = curr->next->next;
			curr->tag = curr->next->tag;
			ft_lstdelone((t_ftlst *)curr->next, free);
			curr->next = tmp;
			continue;
		}
		curr = curr->next;
	}
	return (tokens);
}

char				**st_tokens_to_argv(t_tok_lst **tokens)
{
	char		**ret;
	size_t		i;
	t_tok_lst	*curr;

	curr = *tokens;
	ret = ft_calloc(ft_lstsize((t_ftlst *)curr) + 1, sizeof(char *));
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

bool				escape(char *str, enum e_tok tag)
{
	if (str[0] == '\\' &&
		(tag & TAG_STR || ((tag & TAG_STR_DOUBLE)
							&& ft_strchr("\\\"$", str[1]))))
	{
		ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
		return (true);
	}
	return (false);
}

char				**preprocess(t_tok_lst **tokens, t_env env)
{
	t_tok_lst	*curr;
	enum e_tok	prev_tag;
	char		*str;
	size_t		i;

	prev_tag = 0;
	curr = *tokens;
	while (curr != NULL)
	{
		if (curr->tag & (TAG_STR | TAG_STR_DOUBLE) && (i = -1))
		{
			while ((str = curr->content) != NULL && str[++i] != '\0')
			{
				if (escape(str + i, curr->tag))
					continue;
				if (str[i] == '$')
					i = interpolate((void*[2]){str, &curr},
							i, prev_tag, env) - 1;
			}
		}
		prev_tag = curr->tag;
		curr = curr->next;
	}
	st_stick_tokens(*tokens);
	return (st_tokens_to_argv(tokens));
}
