/*
** \file   parse.c
** \brief  Parser
*/

#include "parse.h"
#include <stdio.h>



t_ret				*parse(t_ftlst *lst)
{
	t_ret 		*ret;

	if(!(ret = malloc(sizeof(t_ret) * 1)))
		return(NULL);
	ret->rest = lst;
	return NULL;
}
