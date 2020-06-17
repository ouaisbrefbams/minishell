/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/06/17 17:10:08 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   main.c
** \brief  Minishell entrypoint
*/

#include "minishell.h"
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

/* void token_debug(void *v) */
/* { */
/* 	t_token *t; */
/*  */
/* 	t= v; */
/* 	printf("[%4d %d] (%s)\n", t->tag, !!(t->tag & TAG_STICK), t->content); */
/* } */
/*  */
/* int main(int argc, char **argv, char **envp) */
/* { */
/* 	t_path	path; */
/* 	t_env	env; */
/*  */
/* 	env = env_from_array(envp); */
/* 	path = path_update(NULL, env_search(env, "PATH")); */
/* 	#<{(| printf("%s\n", argv[2]); |)}># */
/*  */
/* 	if (argc == 3 && ft_strcmp(argv[1], "-c") == 0) */
/* 	{ */
/* 		//printf("%s\n", argv[2]); */
/* 		t_ftlst *lex_out = lexer(ft_strdup(argv[2])); */
/*  */
/* 		 //ft_lstiter(lex_out, token_debug); */
/*  */
/* 		 t_ret *parser_out = parse(lex_out); */
/*  */
/* 		#<{(| printf("===cmd_argv===\n"); |)}># */
/* 		#<{(| ft_lstiter(parser_out->ast->cmd_argv, token_debug); |)}># */
/* 		#<{(| printf("===redirs===\n"); |)}># */
/* 		#<{(| ft_lstiter(parser_out->ast->redirs, token_debug); |)}># */
/*  */
/* 		int fds[2] = {MS_NO_FD, MS_NO_FD}; */
/* 		int eval_out = eval_cmd(fds, env, path, parser_out->ast); */
/* 		(void)eval_out; */
/* 	} */
/*  */
/* 	ft_htdestroy(path, free); */
/* 	ft_vecdestroy(env, free); */
/* 	return (0); */
/* } */

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	/* (void)envp; */
	t_path	path;
	t_env	env;
	/* char	*line; */
	/* int		ret; */
	env = env_from_array(envp);
	path = path_update(NULL, env_search(env, "PATH"));

	t_ftlst *args1 = NULL;
	ft_lstpush_back(&args1, ft_lstnew(token_new(TAG_STR, "ls")));
	ft_lstpush_back(&args1, ft_lstnew(token_new(TAG_STR, "-l")));

	t_ftlst *args2 = NULL;
	ft_lstpush_back(&args2, ft_lstnew(token_new(TAG_STR, "cat")));
	ft_lstpush_back(&args2, ft_lstnew(token_new(TAG_STR, "-e")));
	/* ft_lstpush_back(&args2, ft_lstnew(token_new(TAG_STR, "je"))); */

	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "ls"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "-a"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "-l"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "$$LFS$TERM$TERM."))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "*.c"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "src.c include*.h"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "$A$B"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "\\$TERM"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "$TER\\M"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "\\\\"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_SINGLE, "''''$TEST\\TEST"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_DOUBLE, ",$TEST,$B,"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_DOUBLE | TAG_STICK, "$TEST"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_DOUBLE | TAG_STICK, "$TEST"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_DOUBLE , "$TEST"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_DOUBLE | TAG_STICK, "$TEST"))); */
	/* ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_SINGLE, "$TEST"))); */

	t_ftlst *redirs = NULL;
	ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_REDIR_OUT, NULL)));
	ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_STR, "bonjour")));
	/* ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_REDIR_APPEND, NULL))); */
	/* ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_STR, "yo"))); */
	/* ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_REDIR_OUT, NULL))); */
	/* ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_STR, "yo1"))); */

	t_ast *cmd1 = ast_new(AST_CMD);
	cmd1->cmd_argv = args1;
	cmd1->redirs = NULL;

	t_ast *cmd2 = ast_new(AST_CMD);
	cmd2->cmd_argv = args2;
	cmd2->redirs = NULL;

	t_ast *op_ast = ast_new(AST_OP);
	op_ast->op.left = cmd1;
	op_ast->op.right = cmd2;
	op_ast->op.sep = TAG_PIPE;


	int fds[2] = {MS_NO_FD, MS_NO_FD};
	printf("eval %d\n", eval(fds, env, path, op_ast));

	/* char **as = preprocess(l, env); */
	/* printf("%p\n", as); */
	/* printf("%p\n", *as); */
	/* char **tmp = as; */
	/* while (*as != NULL) */
	/* 	puts(*as++); */
	/* ft_split_destroy(tmp); */
	ft_htdestroy(path, free);
	ft_vecdestroy(env, free);
	return (0);
}

/////////////////////////////////////////////////////////////////////////////////////////
// lexer main
/////////////////////////////////////////////////////////////////////////////////////////

/* int         main(int argc, char **argv) */
/* { */
/*     char *input; */
/* 	t_ftlst *lst; */
/*  */
/* 	if (argv[1] == NULL) */
/* 		return (0); */
/*     if (!(input = malloc(sizeof(char) * ft_strlen(argv[1]) + 1))) */
/*         return(0); */
/*     ft_strlcpy(input, argv[1], ft_strlen(argv[1]) + 1); */
/*  */
/*     lst = lexer(input); */
/* 	parse(lst); */
/*     free(input); */
/* 	exit(0); */
/*     return (0); */
/* } */
