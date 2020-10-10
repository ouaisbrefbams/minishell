/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <me@cacharle.xyz>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 15:46:09 by charles           #+#    #+#             */
/*   Updated: 2020/10/10 11:19:44 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** \brief        Export variable if not already in environment
** \param env    Environment where to export
** \param key    Key to check for export
** \param value  Default value
** \return       True if allocation succeded, false otherwise
*/

bool	st_export_default(t_env env, char *key, char *value)
{
	if (env_search(env, key, NULL) != NULL)
		return (true);
	return (env_export(env, key, value) != NULL);
}

/*
** \brief      Setup the environment variables
** \param env  Environment to setup
** \return     true on success, false otherwise
*/

bool	setup_env(t_env env)
{
	char buf[PATH_MAX + 1];

	if (!(getcwd(buf, PATH_MAX)))
		return (false);
	if (!st_export_default(env, "PWD", buf) ||
		!st_export_default(env, "SHLVL", "1") ||
		!st_export_default(env, "PATH", "/sbin:"))
		return (false);
	if (path_search(env, "env", buf, false) != 0)
		ft_strcpy(buf, "/sbin/env");
	env_export(env, "_", buf);
	return (true);
}

/*
** \brief      Increment the SHLVL variable
** \param env  Environment where to set SHLVL
** \return     true on success, false otherwise
*/

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

/*
** \brief            Initialize the progname global variable
**                   used in error messages
** \param first_arg  argv[0] of minishell
*/

void	setup_progname(char *first_arg)
{
	char	*last_slash;

	if (first_arg == NULL)
		return ;
	last_slash = ft_strrchr(first_arg, '/');
	if (last_slash == NULL)
		g_state.progname = first_arg;
	else
		g_state.progname = last_slash + 1;
}

/*
** \brief            Setup minishell (signals, env variables, progname)
** \param first_arg  Used in setup_progname
** \param env        Environment
** \return           true on succes, false otherwise
*/

bool	setup(char *first_arg, t_env env)
{
	signal(SIGINT, signal_sigint);
	signal(SIGQUIT, signal_sigquit);
	signal(SIGTERM, signal_sigterm);
	setup_progname(first_arg);
	return (setup_env(env) || setup_shlvl(env));
}
