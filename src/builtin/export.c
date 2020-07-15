/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:11:34 by charles           #+#    #+#             */
/*   Updated: 2020/07/15 13:22:18 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   export.c
** \brief  `export` builtin
*/

#include "minishell.h"

static void	st_put_declare_x(char *s)
{
	char	*equal_ptr;

	if (s == NULL)
		return ;
	ft_putstr("declare -x ");
	if ((equal_ptr = ft_strchr(s, '=')) == NULL)
		equal_ptr = ft_strchr(s, '\0');
	write(STDOUT_FILENO, s, equal_ptr - s);
	ft_putchar('=');
	ft_putchar('"');
	ft_putstr(equal_ptr + 1);
	ft_putchar('"');
	ft_putchar('\n');
}

int			builtin_export(char **argv, t_env env)
{
	int		status;
	size_t  i;
	char	*equal_ptr;
	bool	skip;

	if (argv[1] == NULL)
	{
		ft_veciter(env, (void (*)(void*))st_put_declare_x);
		return (0);
	}
	status = 0;
	i = 0;
	while (argv[++i] != NULL)
	{
		skip = (equal_ptr = ft_strchr(argv[i], '=')) == NULL;
		if (!skip)
			*equal_ptr = '\0';
		if (!utils_valid_identifier(argv[i]))
		{
			if (!skip)
				*equal_ptr = '=';
			error_put_invalid_identifier("export", argv[i]);
			status = 1;
			continue;
		}
		if (skip)
			continue;
		if (env_export(env, argv[i], equal_ptr + 1) == NULL)
			return (127); // malloc error
	}
	return (status);
}
