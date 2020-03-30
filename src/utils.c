/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:56:31 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/28 11:57:30 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
** \file   utils.c
** \brief  Various functions
*/

#include "minishell.h"

/**
** \brief          Delete function for a entry containing a allocated key and value
** \param content  Hash table entry content
*/

void				ms_ht_del_str_entry(t_ftht_content *content)
{
	if (content == NULL)
		return ;
	free(content->key);
	free(content->value);
	free(content);
}
