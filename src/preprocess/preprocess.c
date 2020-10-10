/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/10/10 10:56:41 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "lexer.h"
#include "minishell.h"

/*
** \brief         Concatenate sticked string tokens
** \param tokens  List of tokens to concatenate
** \return        The resulting list of tokens
*/

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

/*
** \brief         Convert a list of tokens to a NULL terminated string array
** \param tokens  A pointer to a list of tokens to convert
** \return        The string array of arguments
** \note          tokens is destroyed
*/

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

/*
** \brief      Try to escape the first character of a string
** \param str  String to escape
** \param tag  Tag of the current token
**             (different characters are escaped in different type of strings)
** \return     true if the first there was a character to escape,
**             false otherwise
*/

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

/*
** \brief         Preprocess (escaping and interpolation)
**                tokens an convert then to an argv
** \param tokens  List of token to preprocess
** \param env     Environment
** \return        The arguments on success, NULL on allocation error
*/

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
