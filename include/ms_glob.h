/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_glob.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/05 11:45:11 by charles           #+#    #+#             */
/*   Updated: 2020/04/05 13:05:10 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_GLOB_H
# define MS_GLOB_H

# include <dirent.h>
# include <unistd.h>
# include <stddef.h>
# include "libft_str.h"
# include "libft_vec.h"
# include "utils.h"

struct		s_glob_param
{
	char	*pattern;
	t_ftvec	*matches;
};

t_ftvec		*glob_matches(char *pattern);
char		*ms_glob(char *pattern);

#endif
