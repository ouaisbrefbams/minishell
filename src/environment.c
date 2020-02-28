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

#define MS_ENVIRONMENT_HT_SIZE 2048

t_ftht					*ms_environment_from_array(const char **envp)
{
	t_ftht	*environment;
	int		i;
	char	*key;
	char	*value;
	/* int		equal_pos; */
	
	if (envp == NULL)
		return (NULL);
	if ((environment = ft_htnew(MS_ENVIRONMENT_HT_SIZE)) == NULL)
		return (NULL);
	i = -1;
	while (envp[++i] != NULL)
	{
		// free stuff on error
		if ((value = ft_strchr(envp[i], '=')) == NULL)
			return (NULL);
		if ((key = ft_strndup(envp[i], ft_strspn(envp[i], "="))) == NULL)
			return (NULL);
		if ((value = ft_strdup(value)) == NULL)
			return (NULL);
		if (ft_htset(environment, key, value, ms_ht_del_str_entry) == NULL)
			return (NULL);
	}
	return (environment);
}

char					**ms_environment_to_array(t_ftht *environment)
{
	(void)environment;
	// need ft_htlen
	return (NULL);

}

/* void					ms_environment_destroy(t_ftht *environment) */
/* { */
/* 	ft_htdestroy(environment, ms_ht_del_str_entry); */
/* } */
