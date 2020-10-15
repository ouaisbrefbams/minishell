/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:56:31 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/15 09:39:27 by cacharle         ###   ########.fr       */
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

/*
** \brief            Exit the program on condition
** \param predicate  Exit if true
*/

void	exit_if(bool predicate)
{
	if (!predicate)
		return ;
	ft_putendl_fd("minishell: fatal error", STDERR_FILENO);
	exit(3);
}

/*
** \brief      Check if a string only contains spaces and tabs
** \param str  String to chec
** \return     True if only contains blank, false otherwise
*/

bool	utils_strisblank(char *str)
{
	while (*str != '\0')
	{
		if (!ft_isblank(*str))
			return (false);
		str++;
	}
	return (true);
}
