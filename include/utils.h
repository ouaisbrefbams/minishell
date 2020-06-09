/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/05 12:05:49 by charles           #+#    #+#             */
/*   Updated: 2020/04/05 14:51:38 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/*
** \file   utils.h
** \brief  Various utilitary functions
*/

typedef int	(*t_directory_iter_func)(char*, struct dirent*, void*);

int		utils_directory_iter(
	char *dirname,
	void *param,
	t_directory_iter_func f
);

size_t	utils_var_end(char *name);

#endif
