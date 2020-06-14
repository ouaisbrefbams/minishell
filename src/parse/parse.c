/*
** \file   parse.c
** \brief  Parser
*/

#include "parser.h"
// stdio.h est deja include dans minishell.h temporairement
// (comme ca on doit le retirer a un seul endroit a la fin)


t_ret				*parse(t_ftlst *input)
{
	t_ret 		*ret;

	if(!(ret = malloc(sizeof(t_ret) * 1)))
		return(NULL);
	ret->rest = input;
	return NULL;
}
