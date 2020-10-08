/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:56:31 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/08 17:43:06 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   utils.c
** \brief  Various functions
*/

#include "minishell.h"

/*
** \brief Print the shell prompt, tries to display the current directory
*/

void	print_prompt(void)
{
	char	cwd[PATH_MAX + 1];
	char	*last_slash;

	ft_bzero(cwd, PATH_MAX + 1);
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ft_putstr_fd("$ ", STDERR_FILENO);
		return ;
	}
	last_slash = ft_strrchr(cwd, '/');
	if (last_slash != NULL && ft_strlen(cwd) != 1)
		ft_strcpy(cwd, last_slash + 1);
	ft_putstr_fd("\033[0;32m", STDERR_FILENO);
	ft_putstr_fd(cwd, STDERR_FILENO);
	ft_putstr_fd("\033[0m$ ", STDERR_FILENO);
}

void	exit_if(bool predicate)
{
	if (predicate)
		return ;
	ft_putendl_fd("minishell: fatal error", STDERR_FILENO);
	exit(3);
}
