/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:11:34 by charles           #+#    #+#             */
/*   Updated: 2020/09/12 11:06:47 by charles          ###   ########.fr       */
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
	ft_putstr("export ");
	if ((equal_ptr = ft_strchr(s, '=')) == NULL)
		equal_ptr = ft_strchr(s, '\0');
	write(STDOUT_FILENO, s, equal_ptr - s);
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
		if (!utils_valid_identifier(argv[i]))
		{
			if (equal_ptr != NULL)
				*equal_ptr = '=';
			errorf("export: `%s': not a valid identifier\n", argv[i]);
			status = 1;
			continue;
		}
		if (env_export(env, argv[i], equal_ptr == NULL ? "" : equal_ptr + 1) == NULL)
			return (EVAL_FATAL);
	}
	return (status);
}
