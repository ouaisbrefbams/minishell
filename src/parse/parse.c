/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahaddac <nahaddac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/17 18:09:04 by nahaddac          #+#    #+#             */
/*   Updated: 2020/06/17 18:36:54 by nahaddac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   parse.c
** \brief  Parser
*/

#include "parser.h"
#include "lexer.h"
// stdio.h est deja include dans minishell.h temporairement
// (comme ca on doit le retirer a un seul endroit a la fin)
// oui je sais maintenant tu peux effacer se petit commantaire ;)



t_ast					*cmd_push_ast(t_ftlst *rest)
{
	enum e_token_tag 	tag;
	t_ast 				*ast;

	ast = NULL;
	while (rest != NULL)
	{
		tag = ((t_token *)rest->data)->tag;
		if (parse_cmd_str_true_false(tag))
		{
			ast = push_cmd(ast, rest);
		}
		else if (parse_redir_true_false(tag))
		{
			while(rest != NULL)
			{
				ast = push_redir(ast, rest);
				if (tag & TAG_IS_STR && tag & TAG_STICK)
					rest = rest->next;
				else if (tag & TAG_IS_REDIR)
					rest = rest->next;
				else
					break;
				tag = ((t_token *)rest->data)->tag;
			}
		}
		rest = rest->next;
	}
	return ast;
}

t_ret					*parse(t_ftlst *input)
{
	t_ret 				*ret;
	t_ret 				*first;
	t_ast 				*new_ast;

	if(!(ret = malloc(sizeof(t_ret) * 1)))
		return(NULL);
	ret->rest = input;
	ret->ast = NULL;
	ret->unexpected = NULL;
	first = ret;

	if(!(new_ast = malloc(sizeof(t_ast) * 1)))
		return(0);
	new_ast = cmd_push_ast(ret->rest);
	printf("%s\n",((t_token *)ret->rest->data)->content);
	if (ret->ast)
	{
		while(ret->ast->cmd_argv != NULL)
	    {
	 	 	printf("[%s]\n", ((t_token *)ret->ast->cmd_argv->data)->content);
	      	ret->ast->cmd_argv = ret->ast->cmd_argv->next;
	    }
		while(ret->ast->redirs != NULL)
		{
		 	printf("[%s]\n", ((t_token *)ret->ast->redirs->data)->content);
			ret->ast->redirs = ret->ast->redirs->next;
		}
	}
	ast_destroy(ret->ast);
	ft_lstdestroy(&ret->rest, (void (*)(void*))token_destroy);
	return first;
}
