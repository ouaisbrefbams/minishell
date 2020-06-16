/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/06/16 10:11:30 by charles          ###   ########.fr       */
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

void token_debug(void *v)
{
	t_token *t;

	t= v;
	printf("[%4d] (%s)\n", t->tag, t->content);
}

int main(int argc, char **argv, char **envp)
{
	t_path	path;
	t_env	env;

	env = env_from_array(envp);
	path = path_update(NULL, env_search(env, "PATH"));


	if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		t_ftlst *lex_out = lexer(ft_strdup(argv[2]));

		 ft_lstiter(lex_out, token_debug);

		t_ret *ret = parse(lex_out);

		printf("===========cmd=============\n");
		while(ret->ast->cmd_argv != NULL)
	     {
			 printf("[%s]\n", ((t_token *)ret->ast->cmd_argv->data)->content);
	         ret->ast->cmd_argv = ret->ast->cmd_argv->next;
	     }
		printf("=========redir============\n");
		while(ret->ast->redirs != NULL)
		{
			printf("[%s]\n", ((t_token *)ret->ast->redirs->data)->content);
			ret->ast->redirs = ret->ast->redirs->next;
		}
		/* printf("%p\n", parser_out->ast->cmd_argv); */
		/* printf("%p\n", parser_out->ast->redirs); */

		/* ft_lstiter(parser_out->ast->cmd_argv, token_debug); */
		/* ft_lstiter(parser_out->ast->redirs, token_debug); */

		//int eval_out = eval_cmd(env, path, parser_out->ast);
		//(void)eval_out;
	}

	ft_htdestroy(path, free);
	ft_vecdestroy(env, free);
	return (0);
}

/* int main(int argc, char **argv, char **envp) */
/* { */
/* 	(void)argc; */
/* 	(void)argv; */
/* 	#<{(| (void)envp; |)}># */
/* 	t_path	path; */
/* 	t_env	env; */
/* 	#<{(| char	*line; |)}># */
/* 	#<{(| int		ret; |)}># */
/* 	env = env_from_array(envp); */
/* 	path = path_update(NULL, env_search(env, "PATH")); */
/* 	#<{(|  |)}># */
/* 	#<{(| t_ast *ast; |)}># */
/* 	#<{(| t_line line; |)}># */
/* 	#<{(| t_cmd cmd; |)}># */
/* 	#<{(| t_eval_state state; |)}># */
/* 	#<{(|  |)}># */
/* 	#<{(| cmd.argv = ft_split("ls -l", ' '); |)}># */
/* 	#<{(| cmd.in = NULL; |)}># */
/* 	#<{(| cmd.out = NULL; |)}># */
/* 	#<{(| cmd.is_append = false; |)}># */
/* 	#<{(|  |)}># */
/* 	#<{(| line.left = ast_new(TAG_CMD, &cmd); |)}># */
/* 	#<{(| line.right = NULL; |)}># */
/* 	#<{(| line.sep = SEP_END; |)}># */
/* 	#<{(| ast = ast_new(TAG_LINE, &line); |)}># */
/* 	#<{(| printf("%p\n", ast); |)}># */
/* 	#<{(| printf("%d\n", ast->tag); |)}># */
/* 	#<{(| printf("%p\n", ast->data.line.left); |)}># */
/* 	#<{(| printf("%p\n", ast->data.line.right); |)}># */
/* 	#<{(| printf("%d\n", ast->data.line.left->tag); |)}># */
/* 	#<{(| printf("%p\n", ast->data.line.left->data.cmd.argv); |)}># */
/* 	#<{(| printf("%s\n", ast->data.line.left->data.cmd.argv[0]); |)}># */
/* 	#<{(| printf("%s\n", ast->data.line.left->data.cmd.argv[1]); |)}># */
/* 	#<{(| state.pipe_in[0] = -1; |)}># */
/* 	#<{(| state.pipe_in[1] = -1; |)}># */
/* 	#<{(| state.pipe_out[0] = -1; |)}># */
/* 	#<{(| state.pipe_out[1] = -1; |)}># */
/* 	#<{(| state.path = path; |)}># */
/* 	#<{(| state.env = env; |)}># */
/* 	#<{(| t_io_frame frame; |)}># */
/* 	#<{(| io_frame_init(&frame); |)}># */
/* 	#<{(| printf("ret: %d %s\n", eval(&frame,&state, ast), strerror(errno)); |)}># */
/* 	#<{(| char buf[2048]; |)}># */
/* 	#<{(| printf("%s\n", getcwd(buf, 2048)); |)}># */
/* 	#<{(| builtin_env(NULL, state.env); |)}># */
/* 	#<{(| ast_destroy(ast); |)}># */
/* 	#<{(| while ((ret = ft_next_line(STDIN_FILENO, &line)) == 1) |)}># */
/* 	#<{(| { |)}># */
/* 	#<{(| 	if (eval(parse(line)) == -1) |)}># */
/* 	#<{(| 		continue ;  // and display error |)}># */
/* 	#<{(| 	free(line); |)}># */
/* 	#<{(| } |)}># */
/* 	#<{(| free(line); |)}># */
/* 	#<{(| ms_glob("src#<{(|"); |)}># */
/* 	#<{(| char *j = ms_glob("|)}>#*.c"); |)}># */
/* 	#<{(| printf("%s\n", j); |)}># */
/* 	#<{(| free(j); |)}># */
/*  */
/* 	t_ftlst *args = NULL; */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "echo"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "bonjour"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "je"))); |)}># */
/*  */
/* 	ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "cat"))); */
/* 	ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "-e"))); */
/*  */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "ls"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "-a"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "-l"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "$$LFS$TERM$TERM."))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "|)}>#*.c"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "src#<{(|.c include#<{(|.h"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "$A$B"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "\\$TERM"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "$TER\\M"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR, "\\\\"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_SINGLE, "''''$TEST\\TEST"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_DOUBLE, ",$TEST,$B,"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_DOUBLE | TAG_STICK, "$TEST"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_DOUBLE | TAG_STICK, "$TEST"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_DOUBLE , "$TEST"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_DOUBLE | TAG_STICK, "$TEST"))); |)}># */
/* 	#<{(| ft_lstpush_back(&args, ft_lstnew(token_new(TAG_STR_SINGLE, "$TEST"))); |)}># */
/*  */
/* 	t_ftlst *redirs = NULL; */
/* 	ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_REDIR_IN, NULL))); */
/* 	ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_STR, "bonjour"))); */
/* 	ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_REDIR_APPEND, NULL))); */
/* 	ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_STR, "yo"))); */
/* 	ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_REDIR_OUT, NULL))); */
/* 	ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_STR, "yo1"))); */
/*  */
/* 	t_ast *ast = ast_new(AST_CMD); */
/* 	ast->cmd_argv = args; */
/* 	ast->redirs = redirs; */
/*  */
/* 	printf("eval %d\n", eval_cmd(env, path, ast)); */
/* 	ast_destroy(ast); */
/*  */
/*  */
/* 	#<{(| char **as = preprocess(l, env); |)}># */
/* 	#<{(| printf("%p\n", as); |)}># */
/* 	#<{(| printf("%p\n", *as); |)}># */
/* 	#<{(| char **tmp = as; |)}># */
/* 	#<{(| while (*as != NULL) |)}># */
/* 	#<{(| 	puts(*as++); |)}># */
/* 	#<{(| ft_split_destroy(tmp); |)}># */
/* 	ft_htdestroy(path, free); */
/* 	ft_vecdestroy(env, free); */
/* 	return (0); */
/* } */

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
