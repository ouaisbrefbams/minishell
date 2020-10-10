/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 09:21:24 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/10 18:08:17 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   env.c
** \brief  Environment manipulation
*/

#include "minishell.h"
#include "eval.h"

#define ENV_VEC_DEFAULT_SIZE 64

/*
** \brief       Convert array of string to environment hash table
** \param envp  array of string (each in the format `name=value`)
** \return      Environment hash table or NULL on error
*/

t_env		env_from_array(char **envp)
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

/*
** \brief              Search a key in environment
** \param env          Searched environment
** \param key          Searched key
** \param found_index  If found_index != NULL and key is found
**                     put the index in the env array of key in found_index
** \return             Value after '=' in environment variable array
**                     or NULL if not found
*/

char		*env_search(t_env env, char *key, size_t *found_index)
{
	size_t	i;
	size_t	key_len;

	i = 0;
	while (i < env->size - 1)
	{
		key_len = ft_strlen(key);
		if (ft_strncmp(env->data[i], key, key_len) == 0
			&& ft_strlen(env->data[i]) > key_len
			&& ((char**)env->data)[i][key_len] == '=')
		{
			if (found_index != NULL)
				*found_index = i;
			return (ft_strchr(env->data[i], '=') + 1);
		}
		i++;
	}
	return (NULL);
}

/*
** \brief        Insert or update an environment variable
** \param env    Updated environment
** \param key    Name of the variable to update
** \param value  New value of the variable
** \return       The full variable (i.e `key=value`) or NULL on error
*/

char		*env_export(t_env env, char *key, char *value)
{
	char	*joined;
	size_t	found_index;

	if ((joined = ft_strjoin3(key, "=", value)) == NULL)
		return (NULL);
	if (env_search(env, key, &found_index) == NULL)
	{
		if (ft_vecinsert(env, env->size - 1, joined) == NULL)
			return (NULL);
	}
	else
	{
		free(env->data[found_index]);
		env->data[found_index] = joined;
	}
	return (joined);
}

/*
** \brief   Buffer containning the string representation of the status code
**          and returned by env_search_first_match if asked for $?
*/

static char	g_status_buf[64] = {'\0'};

/*
** \brief           Search the environment for a potential key located
**                  at the start of haystack
** \param haystack  Potential key
** \return          The value assiciated with the key or NULL if not found
*/

char		*env_search_first_match(t_env env, const char *haystack)
{
	size_t	len;
	size_t	i;
	size_t	key_len;

	if (!ft_isalnum(*haystack) && *haystack != '_' && *haystack != '?')
		return ("$");
	if (ft_isdigit(*haystack))
		return (NULL);
	len = 0;
	while (ft_isalnum(haystack[len]) || haystack[len] == '_')
		len++;
	if (haystack[0] == '?')
		return (ft_itoa_cpy(g_status_buf, g_state.last_status));
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

size_t		env_key_len(char *key, bool allow_status)
{
	size_t	i;

	if (allow_status && *key == '?')
		return (1);
	if (ft_isdigit(*key))
		return (0);
	i = 0;
	while (ft_isalnum(key[i]) || key[i] == '_')
		i++;
	return (i);
}
