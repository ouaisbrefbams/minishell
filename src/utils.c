/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:56:31 by cacharle          #+#    #+#             */
/*   Updated: 2020/07/19 14:53:27 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   utils.c
** \brief  Various functions
*/

#include "minishell.h"

int		utils_directory_iter(
	char *dirname,
	void *param,
	int (*f)(char*, struct dirent*, void*)
)
{
	DIR				*dir;
	struct dirent	*entry;

	errno = 0;
	if ((dir = opendir(dirname)) == NULL)  // add fail safe
		return (errno == EACCES ? -2 : -1);
	while ((entry = readdir(dir)) != NULL)
		if (f(dirname, entry, param) == -1)
		{
			closedir(dir);
			return (-1);
		}
	if (closedir(dir) == -1)
		return (-1);
	return (0);
}

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

// TODO find a way to flush without fflush
void print_prompt(void)
{
	printf("\033[0;32m%s\033[0m$ ", getcwd(NULL, 0));
	fflush(stdout);
}
