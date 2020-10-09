/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <me@cacharle.xyz>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 15:29:04 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/09 15:30:03 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"
#include "lexer.h"
#include "minishell.h"

int	preprocess_filename(t_tok_lst **tokens, t_env env, char **filename)
{
	char	**strs;

	if ((strs = preprocess(tokens, env)) == NULL || strs[0] == NULL)
	{
		free(strs);
		return (EVAL_FATAL);
	}
	if (strs[1] != NULL)
	{
		errorf("%s: ambiguous redidrect\n", strs[1]);
		ft_split_destroy(strs);
		return (1);
	}
	*filename = strs[0];
	free(strs);
	return (*filename == NULL ? EVAL_FATAL : 0);
}
