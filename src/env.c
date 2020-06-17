/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 09:21:24 by cacharle          #+#    #+#             */
/*   Updated: 2020/06/17 12:50:26 by charles          ###   ########.fr       */
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

int		env_keycmp(char *var, char *key)
{
	size_t	key_len;

	key_len = ft_strlen(key);
	if (ft_strncmp(var, key, key_len) == 0
		&& ft_strlen(var) > key_len
		&& var[key_len] == '=')
		return (0);
	return (1);
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
	while (i < env->size - 1)
	{
		if (env_keycmp(env->data[i], key))
			return (ft_strchr(env->data[i], '=') + 1);
		i++;
	}
	return (NULL);
}

char	*env_search_first_match(t_env env, const char *haystack)
{
	size_t	len;
	size_t	i;
	size_t	key_len;

	if (ft_isdigit(*haystack))
		return (NULL);
	len = 0;
	while (ft_isalnum(haystack[len]) || haystack[len] == '_')
		len++;
	if (len == 0)
		return (NULL);
	i = -1;
	while (++i < env->size - 1)
	{
		key_len = ft_strchr(env->data[i], '=') - (char*)env->data[i];
		if (len != key_len)
			continue ;
		if (ft_strncmp((char*)env->data[i], haystack, len) == 0)
			return (ft_strchr(env->data[i], '=') + 1);
	}
	return (NULL);
}

char	*env_export(t_env env, char *key, char *value)
{
	char	*joined;
	size_t	i;

	if ((joined = ft_strjoin3(key, "=", value)) == NULL)
		return (NULL);
	if (env_search(env, key) == NULL)
	{
		if (ft_vecinsert(env, env->size - 1, joined) == NULL)
			return (NULL);
	}
	else
	{
		i = 0;
		while (i < env->size - 1)
		{
			if (env_keycmp(env->data[i], key) == 0)
			{
				free(env->data[i]);
				env->data[i] = joined;
				break;
			}
			i++;
		}
	}
	return (joined);
}
