/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:56:31 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/08 11:31:05 by cacharle         ###   ########.fr       */
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

	ft_bzero(cwd, PATH_MAX + 1);
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ft_putstr_fd("$ ", STDERR_FILENO);
		return ;
	}
	ft_putstr_fd("\033[0;32m", STDERR_FILENO);
	ft_putstr_fd(cwd, STDERR_FILENO);
	ft_putstr_fd("\033[0m$ ", STDERR_FILENO);
}

void	try(void *ptr)
{
	if (ptr != NULL)
		return ;
	ft_putendl_fd(STDERR_FILENO, "minishell: fatal error");
	exit(3);
}
