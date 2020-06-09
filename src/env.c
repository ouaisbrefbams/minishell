/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 09:21:24 by cacharle          #+#    #+#             */
/*   Updated: 2020/05/08 13:48:40 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   env.c
** \brief  Environment manipulation
*/

#include "minishell.h"

#define ENV_VEC_DEFAULT_SIZE 64

/*
** \brief       Convert array of string to environment hash table
** \param envp  array of string (each in the format `name=value`)
** \return      Environment hash table or NULL on error
*/

t_env	env_from_array(char **envp)
{
	t_env	env;

	if ((env = ft_vecnew(ENV_VEC_DEFAULT_SIZE)) == NULL)
		return (NULL);
	while (*envp != NULL)
	{
		if (ft_vecpush_safe(env, ft_strdup(*envp)) == NULL)
		{
			ft_vecdestroy(env, free);
			return (NULL);
		}
		envp++;
	}
	return (ft_vecpush(env, NULL));
}

/**
** \brief      Search a key in environment
** \param env  Search environment
** \param key  Searched key
** \return     Value after '=' in environment variable array or NULL if not found
*/

// could be a wrapper around ft_lfind
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

char	*env_search_first_match(t_env env, const char *haystack)
{
	int		len;
	size_t	i;

	if (ft_isdigit(*haystack))
		return (NULL);
	len = 0;
	while (ft_isalnum(haystack[len]) || haystack[len] == '_')
		len++;
	i = 0;
	while (i < env->size)
	{
		if (ft_strncmp((char*)env->data[i], haystack, len) == 0)
			return (ft_strchr((char*)env->data[i], '=') + 1);
		i++;
	}
	return (NULL);
}
