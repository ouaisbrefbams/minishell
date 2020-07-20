/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_eval.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/20 13:07:25 by nahaddac          #+#    #+#             */
/*   Updated: 2020/07/20 14:51:40 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"


t_ftlst                 *split_token(t_ftlst **lst, enum e_token_tag tag)
{
    t_ftlst             *curr;
    t_ftlst             *start;

    start = *lst;
    curr = *lst;
    while (curr != NULL || ((t_token *)curr->data)->tag & tag)
	{
		if (curr->next == NULL || (!(((t_token *)curr->next->data)->tag & tag)))
		{
            *lst = curr->next;
			curr->next = NULL;
            return start;
		}
		curr = curr->next;
	}
    return start;
}
