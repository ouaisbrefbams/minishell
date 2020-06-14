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
        printf("%d\n", tag);
        if (parse_redir_true_false(tag))
            ;
		if (parse_cmd_str_true_false(tag))
			ret->ast = parse_cmd(ret->ast, ret->rest);
		ret->rest = ret->rest->next;
	}
    while(ret->ast->cmd_argv != NULL)
    {
        printf("%s\n", ((t_token *)ret->ast->cmd_argv->data)->content);
        ret->ast->cmd_argv = ret->ast->cmd_argv->next;
    }
	free(ret->ast);
	return NULL;
}
