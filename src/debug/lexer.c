/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <me@cacharle.xyz>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 07:51:39 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/10 07:56:44 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

#ifdef MINISHELL_TEST

int	debug_lexer(char *input)
{
	int			status;
	t_tok_lst	*out;

	status = lexer(input, &out);
	if (status != 0)
		return (status);
	while (out != NULL)
	{
		ft_putnbr(out->tag);
		ft_putchar(' ');
		ft_putstr(out->content);
		ft_putendl(out->tag & TAG_STICK ? " STICK" : "");
		out = out->next;
	}
	return (status);
}

#endif
