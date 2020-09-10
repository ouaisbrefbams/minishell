/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/28 16:52:54 by charles           #+#    #+#             */
/*   Updated: 2020/08/28 16:53:04 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

	/* while (ast->cmd_argv != NULL */
	/* 	&& ((t_token*)ast->cmd_argv->data)->tag & TAG_IS_STR */
	/* 	&& utils_start_with_valid_identifier(((t_token*)ast->cmd_argv->data)->content)) */
	/* { */
	/* 	t_ftlst *start; */
    /*  */
	/* 	id = ((t_token*)ast->cmd_argv->data)->content; */
	/* 	*ft_strchr(id, '=') = '\0'; */
	/* 	((t_token*)ast->cmd_argv->data)->content = ft_strchr(id, '\0') + 1; */
	/* 	if (*((t_token*)ast->cmd_argv->data)->content == '\0') */
	/* 		ft_lstpop_front(&ast->cmd_argv, NULL); */
	/* 	else */
	/* 	{ */
	/* 		t_ftlst *curr = ast->cmd_argv; */
	/* 		t_ftlst *prev = curr; */
    /*  */
	/* 		while (curr != NULL */
	/* 				&& ((t_token*)curr->data)->tag & TAG_STICK && ((t_token*)curr->data)->tag & TAG_IS_STR) */
	/* 		{ */
	/* 			prev = curr; */
	/* 			curr = curr->next; */
	/* 		} */
	/* 		if (curr != NULL && ((t_token*)curr->data)->tag & TAG_IS_STR) */
	/* 		{ */
	/* 			prev = curr; */
	/* 			curr = curr->next; */
	/* 		} */
    /*  */
	/* 		start = ast->cmd_argv; */
	/* 		ast->cmd_argv = prev->next; */
	/* 		prev->next = NULL; */
	/* 	} */
    /*  */
	/* 	#<{(| ft_lstiter(start, token_debug); |)}># */
	/* 	#<{(| puts(""); |)}># */
	/* 	#<{(| ft_lstiter(ast->cmd_argv, token_debug); |)}># */
    /*  */
	/* 	char **strs = preprocess(&start, env); */
    /*  */
	/* 	if (env_export(param.env_local, id, strs[0]) == NULL) */
	/* 		return (-1); */
	/* } */
	/* if (ast->cmd_argv == NULL) // FIXME special env not passed to child processes */
	/* { */
	/* 	ft_vecpop(param.env_local, NULL); */
	/* 	if (ft_vecswallow_at(env, env->size - 1, param.env_local) == NULL) */
	/* 	{ */
	/* 		ft_vecdestroy(param.env_local, free); */
	/* 		return (-1); */
	/* 	} */
	/* 	g_last_status = 0; */
	/* 	return (0); */
	/* } */

