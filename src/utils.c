/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:56:31 by cacharle          #+#    #+#             */
/*   Updated: 2020/09/09 15:41:04 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   utils.c
** \brief  Various functions
*/

#include "minishell.h"


/* bool	utils_is_var_name(char *name) */
/* { */
/* 	if (!(ft_isalpha(*name) || *name == '_')) */
/* 		return (false); */
/* 	name++; */
/* 	while (*name != '\0) */
/* 		if (!(ft_isalnum(*name) || *name == '_')) */
/* 			return (false); */
/* 	return (true); */
/* } */

size_t	utils_var_end(char *name)
{
	size_t	i;

	if (*name == '?')
		return (2);
	if (ft_isdigit(*name))
		return (0);
	i = 0;
	while (ft_isalnum(name[i]) || name[i] == '_')
		i++;
	return (i + 1);
}

bool	utils_valid_identifier(char *name)
{
	if (ft_isdigit(*name) || *name == '\0')
		return (false);
	while (*name != '\0')
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}

bool	utils_start_with_valid_identifier(char *name)
{
	char	*equal_ptr;
	bool	ret;

	equal_ptr = ft_strchr(name, '=');
	if (equal_ptr == NULL)
		return (false);
	*equal_ptr = '\0';
	ret = utils_valid_identifier(name);
	*equal_ptr = '=';
	return (ret);
}

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
