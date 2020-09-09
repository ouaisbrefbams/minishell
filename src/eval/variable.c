/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/09 17:12:14 by charles           #+#    #+#             */
/*   Updated: 2020/09/09 18:22:43 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

bool	variable_extract(t_tok_lst **argv, t_env env, t_env env_local)
{
	char 		*key;
	t_tok_lst 	*value_tokens;
	char		**strs;

	if (*argv == NULL || !((*argv)->tag & TAG_STR)
		|| !utils_start_with_valid_identifier((*argv)->content))
		return (true);

	key = (*argv)->content;
	(*argv)->content = ft_strchr(key, '=');
	*(*argv)->content = '\0';
	(*argv)->content++;
	(*argv)->content = ft_strdup((*argv)->content);

	/* printf("|%s| |%s|\n", key, (*argv)->content); */
	/* if (*(*argv)->content == '\0') */
	/* { */
	/* 	ft_lstpop_front((t_ftlst**)argv, NULL); */
	/* 	return (true); */
	/* } */
	value_tokens = tok_lst_take_sticked(argv);
	strs = preprocess(&value_tokens, env);
	if (env_export(env_local, key, strs[0]) == NULL)
		return (false);
	return (variable_extract(argv, env, env_local));
}
