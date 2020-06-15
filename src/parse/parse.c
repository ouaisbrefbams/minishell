/*
** \file   parse.c
** \brief  Parser
*/

#include "parser.h"
#include "lexer.h"
// stdio.h est deja include dans minishell.h temporairement
// (comme ca on doit le retirer a un seul endroit a la fin)



t_ret					*parse(t_ftlst *input)
{
	t_ret 				*ret;
	t_ret 				*first;
	enum e_token_tag 	tag;
	/* int 				i = 0; */

	if(!(ret = malloc(sizeof(t_ret) * 1)))
		return(NULL);
	ret->rest = input;
	ret->ast = NULL;
	ret->unexpected = NULL;
	first = ret;

	while (ret->rest != NULL)
	{
		tag = ((t_token *)ret->rest->data)->tag;
		if (parse_cmd_str_true_false(tag))
		{
			ret->ast = parse_cmd(ret->ast, ret->rest);
		}
		else if (parse_redir_true_false(tag))
		{
			ret->ast = parse_redir(ret->ast, ret->rest);
			ret->rest = ret->rest->next;
			if (ret->rest != NULL)
				ret->ast = parse_redir(ret->ast, ret->rest);
			/* printf("%s\n","ici" ); */
			ret->rest = ret->rest->next;
			if (ret->rest != NULL)
			{
				tag = ((t_token *)ret->rest->data)->tag;
				if ((tag & TAG_IS_STR) && (tag & TAG_STICK))
					ret->ast = parse_redir(ret->ast, ret->rest);
			}
		}
		if (ret->rest != NULL)
			ret->rest = ret->rest->next;
	}

	/* while(ret->ast->cmd_argv != NULL) */
	/* { */
		/* printf("%s\n", ((t_token *)ret->ast->cmd_argv->data)->content); */
	/* 	ret->ast->cmd_argv = ret->ast->cmd_argv->next; */
	/* } */
	/* while(ret->ast->redirs != NULL) */
	/* { */
	/* 	if (i == 0) */
	/* 	{ */
			/* printf("redir"); */
		/* 	i++; */
		/* } */
		/* printf("%s\n", ((t_token *)ret->ast->redirs->data)->content); */
	/* 	ret->ast->redirs = ret->ast->redirs->next; */
	/* } */
	/* ast_destroy(ret->ast); */
	/* ft_lstdestroy(&ret->rest, (void (*)(void*))token_destroy); */
	return first;
}
