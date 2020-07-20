/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_eval.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/20 13:07:25 by nahaddac          #+#    #+#             */
/*   Updated: 2020/07/20 17:53:18 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"


t_ftlst                 *split_token(t_ftlst **lst, enum e_token_tag tag)
{
    t_ftlst             *curr;
    t_ftlst             *start;
	enum e_token_tag	curr_tag;

    start = *lst;
    curr = *lst;
	t_ftlst  *prev = curr;
	while (curr != NULL)
	{
		curr_tag = ((t_token *)curr->data)->tag;
		if (!(curr_tag & TAG_STICK) || !(curr_tag & TAG_IS_STR))
		{
			*lst = prev->next;
			prev->next = NULL;
			return start;
		}
		prev = curr;
		curr = curr->next;
	}
	return start;



	/* if (curr != NULL) */
	/* 	curr_tag = ((t_token *)curr->data)->tag; */
    /* while (curr != NULL && curr_tag & TAG_STICK && curr_tag & TAG_IS_STR) */
	/* { */
	/* 	curr = curr->next; */
	/* 	curr_tag = ((t_token *)curr->data)->tag; */
	/* 	if (curr == NULL || !(curr_tag & TAG_STICK) || !(curr_tag & TAG_IS_STR)) */
	/* 	{ */
    /*         *lst = curr->next; */
	/* 		curr->next = NULL; */
	/* 	} */
	/* } */
    /* return start; */
}
