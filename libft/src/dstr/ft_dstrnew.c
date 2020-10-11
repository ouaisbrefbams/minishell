/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dstrnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 13:54:52 by charles           #+#    #+#             */
/*   Updated: 2020/06/09 17:35:14 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_dstr.h"

/*
** \brief       Create a new dynamic string
** \param from  Static string to create the dynamic one from
**              (will be duplicated)
** \return      Created dynamic string or NULL on malloc error
*/

t_ftdstr	*ft_dstrnew(char *from)
{
	char		*clone;
	t_ftdstr	*ret;

	if ((clone = ft_strdup(from)) == NULL)
		return (NULL);
	if ((ret = ft_dstrwrap(clone)) == NULL)
		free(clone);
	return (ret);
}
