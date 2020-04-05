/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 08:58:49 by charles           #+#    #+#             */
/*   Updated: 2020/04/05 14:58:00 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ms_glob.h"

static char	*iterpolate(char *str, t_env env)
{
	size_t		i;
	t_ftdstr	*dstr;
	char		*match;

	if ((dstr = ft_dstrnew(str)) == NULL)
		return (NULL);
	free(str);
	i = 0;
	while (i < dstr->length)
	{
		if (dstr->str[i] == '$')
		{
			if ((match = env_search_first_match(env, dstr->str + i + 1)) == NULL)
				ft_dstrerase(dstr, i, utils_var_end(dstr->str + i + 1));
			else
			{
				if (ft_dstrsubstitute(dstr, match, i, utils_var_end(dstr->str + i + 1)) == NULL)
					return (NULL);
			}
		}
		i++;
	}
	return (ft_dstrunwrap(dstr));
}

static char	*preprocess_arg(char *arg, t_env env)
{
	if (*arg == '\'')
		return (ft_strsubf(arg, 1, ft_strlen(arg) - 1));
	if (*arg == '"')
	{
		if (ft_strchr(arg, '$') != NULL)
			arg = iterpolate(arg, env);
		return (ft_strsubf(arg, 1, ft_strlen(arg) - 1));
	}
	if (ft_strchr(arg, '$') != NULL)
		return (iterpolate(arg, env));
	if (ft_strchr(arg, '*') != NULL)
		return (ms_glob(arg));
	return (arg);
}

char		**preprocess_argv(char **argv, t_env env)
{
	int	i;

	i = -1;
	while (argv[++i] != NULL)
		if ((argv[i] = preprocess_arg(argv[i], env)) == NULL)
			return (NULL);
	return (argv);
}
