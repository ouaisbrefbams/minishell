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
** \param entry    Hash table entry
**
*/

void				ht_del_str_entry(t_ftht_entry *entry)
{
	if (entry == NULL)
		return ;
	free(entry->key);
	free(entry->value);
	free(entry);
}
