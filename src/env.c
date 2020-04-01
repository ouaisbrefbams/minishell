/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 09:21:24 by cacharle          #+#    #+#             */
/*   Updated: 2020/04/01 23:09:33 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   env.c
** \brief  Environment manipulation
*/

#include "minishell.h"

/*
** \brief       Convert array of string to environment hash table
** \param envp  array of string (each in the format `name=value`)
** \return      Environment hash table or NULL on error
*/

t_env	env_from_array(char **envp)
{
	t_env	env;
	size_t	i;

	i = 0;
	while (envp[i] != NULL)
		if (ft_strchr(envp[i++], '=') == NULL)
			return (NULL);
	if ((env = ft_vecnew(i)) == NULL)
		return (NULL);
	env->size = i;
	i = 0;
	while (envp[i] != NULL)
	{
		if ((env->data[i] = ft_strdup(envp[i])) == NULL)
		{
			ft_vecdestroy(env, free);
			return (NULL);
		}
		i++;
	}
	return (env);
}

/**
** \brief      Search a key in environment
** \param env  Search environment
** \param key  Searched key
** \return     Value after '=' in environment variable array or NULL if not found
*/

char	*env_search(t_env env, char *key)
{
	size_t	i;

	i = 0;
	while (i < env->size)
	{
		if (ft_strncmp((char*)env->data[i], key, ft_strlen(key)) == 0)
			return (ft_strchr((char*)env->data[i], '=') + 1);
		i++;
	}
	return (NULL);
}
