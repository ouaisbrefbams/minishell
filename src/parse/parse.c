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
			ret->ast = push_cmd(ret->ast, ret->rest);
		}
		else if (parse_redir_true_false(tag))
		{
			while(ret->rest != NULL)
			{
				ret->ast = push_redir(ret->ast, ret->rest);
				if (tag & TAG_IS_STR && tag & TAG_STICK)
					ret->rest = ret->rest->next;
				else if (tag & TAG_IS_REDIR)
					ret->rest = ret->rest->next;
				else
				{
					//ret->rest = ret->rest->next;
					break;
				}
				tag = ((t_token *)ret->rest->data)->tag;
			}
		}
		ret->rest = ret->rest->next;
	}
	// printf("===========cmd=============\n");
    // while(ret->ast->cmd_argv != NULL)
    //  {
	// 	 printf("[%s]\n", ((t_token *)ret->ast->cmd_argv->data)->content);
    //      ret->ast->cmd_argv = ret->ast->cmd_argv->next;
    //  }
	//  printf("=========redir============\n");
	//  while(ret->ast->redirs != NULL)
	//  {
	// 	 printf("[%s]\n", ((t_token *)ret->ast->redirs->data)->content);
	// 	 ret->ast->redirs = ret->ast->redirs->next;
	//  }
	//ast_destroy(ret->ast);
	//ft_lstdestroy(&ret->rest, (void (*)(void*))token_destroy);
	return first;
}
