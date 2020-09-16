/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 15:46:09 by charles           #+#    #+#             */
/*   Updated: 2020/09/16 16:17:35 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	st_export_default(t_env env, char *key, char *value)
{
	if (env_search(env, key, NULL) != NULL)
		return (true);
	return (env_export(env, key, value) != NULL);
}

bool	setup_env(t_env env)
{
	char buf[PATH_MAX + 1];

	if (!(getcwd(buf, PATH_MAX)))
		return (false);
	if (!st_export_default(env, "PWD", buf) ||
		!st_export_default(env, "SHLVL", "0") ||
		!st_export_default(env, "PATH", "/sbin:"))
		return (false);

	/* char *path_str = env_search(env, "PATH"); */
	/* if (ft_strstr(path_str, "/sbin") == NULL) */
	/* { */
	/* 	char *value = ft_strjoin("/sbin:", path_str); */
	/* 	env_export(env, "PATH", value); */
	/* 	free(value); */
	/* } */

	if (path_search(env, "env", buf, false) != 0)
	{
		ft_strcpy(buf, "/sbin/env");
		/* errorf("env: command not found\n"); */
		/* return (127); */
	}
	env_export(env, "_", buf);
	return (true);
}

bool	setup_shlvl(t_env env)
{
	char	shlvl_str[64];
	char	*shlvl_value;

	shlvl_value = env_search(env, "SHLVL", NULL);
	if (shlvl_value == NULL)
		return (false);
	ft_itoa_cpy(shlvl_str, ft_atoi(shlvl_str));
	return (env_export(env, "SHLVL", shlvl_str) != NULL);
}

void	setup_progname(char *first_arg)
{
	char	*last_slash;

	if (first_arg == NULL)
		return ;
	last_slash = ft_strrchr(first_arg, '/');
	if (last_slash == NULL)
		g_progname = first_arg;
	else
		g_progname = last_slash + 1;
}

bool	setup(char *first_arg, t_env env)
{
	signal(SIGINT, signal_sigint);
	signal(SIGQUIT, signal_sigquit);
	signal(SIGTERM, signal_sigterm);
	setup_progname(first_arg);
	return (setup_env(env) || setup_shlvl(env));
}
