/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 09:21:24 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/28 12:30:55 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MS_ENV_HT_SIZE 2048

t_env					ms_env_from_array(char **envp)
{
	t_env	env;
	int		i;
	char	*key;
	char	*value;

	if (envp == NULL)
		return (NULL);
	if ((env = ft_htnew(MS_ENV_HT_SIZE)) == NULL)
		return (NULL);
	i = -1;
	while (envp[++i] != NULL)
	{
		// free stuff on error
		if ((value = ft_strchr(envp[i], '=')) == NULL)
			return (NULL);
		if ((key = ft_strndup(envp[i], ft_strcspn(envp[i], "="))) == NULL)
			return (NULL);
		if ((value = ft_strdup(value + 1)) == NULL)
			return (NULL);
		if (ft_htset(env, key, value, ms_ht_del_str_entry) == NULL)
			return (NULL);
		free(key);
	}
	return (env);
}

char					**ms_env_to_array(t_env env)
{
	(void)env;
	// need ft_htlen
	return (NULL);
}
