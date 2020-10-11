/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vecremove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 22:45:07 by charles           #+#    #+#             */
/*   Updated: 2020/06/09 13:28:26 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_vec.h"

/*
** \brief      Remove element from vector
** \param vec  Vector to remove from
** \param i    Index of the element to remove
** \param del  Delete function applied to ith element
*/

void	ft_vecremove(t_ftvec *vec, size_t i, void (*del)(void *elem))
{
	void	*tmp;

	if ((tmp = ft_vectake(vec, i)) == NULL)
		return ;
	if (del != NULL)
		del(tmp);
}
