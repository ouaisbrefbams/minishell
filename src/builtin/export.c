/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:11:34 by charles           #+#    #+#             */
/*   Updated: 2020/10/06 17:40:22 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   export.c
** \brief  `export` builtin
*/

#include "minishell.h"

/*
** \brief    Put an environment variable in the format
**           "declare -x id=value" of bash
** \param s  Full variable (id=value)
*/

static void	st_put_declare_x(char *s)
{
	char	*equal_ptr;
	int		shlvl;

	if (s == NULL)
		return ;
	if ((equal_ptr = ft_strchr(s, '=')) == NULL)
		equal_ptr = ft_strchr(s, '\0');
	*equal_ptr = '\0';
	if (ft_strcmp(s, "_") == 0)
		return ;
	ft_putstr("declare -x ");
	ft_putstr(s);
	ft_putchar('=');
	ft_putchar('"');
	while (*++equal_ptr != '\0')
	{
		if (*equal_ptr == '"')
			ft_putchar('\\');
		ft_putchar(*equal_ptr);
	}
	ft_putchar('"');
	ft_putchar('\n');
}

/*
** \brief       Export variables to the environment
** \param argv  arguments
** \param env   environment
** \return      a status code or EVAL_FATAL on fatal error
*/

int			builtin_export(char **argv, t_env env)
{
	int		status;
	size_t	i;
	char	*equal_ptr;

	if (argv[1] == NULL)
	{
		ft_veciter(env, (void (*)(void*))st_put_declare_x);
		return (0);
	}
	status = 0;
	i = 0;
	while (argv[++i] != NULL)
	{
		equal_ptr = ft_strchr(argv[i], '=');
		if (equal_ptr != NULL)
			*equal_ptr = '\0';
		if (*argv[i] == '\0'
			|| env_key_len(argv[i], false) != ft_strlen(argv[i]))
		{
			if (equal_ptr != NULL)
				*equal_ptr = '=';
			errorf("export: `%s': not a valid identifier\n", argv[i]);
			status = 1;
			continue;
		}
		if (equal_ptr != NULL
			&& env_export(env, argv[i], equal_ptr + 1) == NULL)
			return (EVAL_FATAL);
	}
	return (status);
}
