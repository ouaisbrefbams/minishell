/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/07/13 14:55:16 by nahaddac         ###   ########.fr       */
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

void token_debug(void *v);
void token_put(void *v);
void print_level(int level);
void ast_print(int level, t_ast *ast);

void print_prompt(void)
{
	printf("\033[0;32m%s\033[0m$ ", getcwd(NULL, 0));
	fflush(stdout);
}

void signal_sigint(int signum)
{
	(void)signum;
	if (g_child_pid != -1)
	{
		kill(g_child_pid, SIGINT);
		ft_putchar('\n');
	}
	else
	{
		ft_putchar('\n');
		print_prompt();
	}
}

void signal_sigquit(int signum)
{
	(void)signum;
	if (g_child_pid != -1)
	{
		kill(g_child_pid, SIGQUIT);
		ft_putstr("Quit (core dumped)\n");
	}
}

int main(int argc, char **argv, char **envp)
{
	t_path	path;
	t_env	env;

	env = env_from_array(envp);
	path = path_update(NULL, env_search(env, "PATH"));
	/* printf("%s\n", argv[2]); */

	signal(SIGINT, signal_sigint);
	signal(SIGQUIT, signal_sigquit);

	if (argc == 3 && ft_strcmp(argv[1], "-l") == 0)
	{
		t_ftlst *lex_out = lexer(ft_strdup(argv[2]));
		if (lex_out == NULL)
			return (1);
		ft_lstiter(lex_out, token_debug);
	}
	else if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		t_ftlst *lex_out = lexer(ft_strdup(argv[2]));
		if (lex_out == NULL)
			return (1);

		/* ft_lstiter(lex_out, token_debug); */

		t_ret *parser_out = parse(lex_out);
		printf("%s\n","ssss" );
		if (parser_out == NULL || parser_out->unexpected != NULL)
		 return (1);

		/* ast_print(0, parser_out->ast); */
		/* printf("\n"); */
		/* fflush(stdout); */
		/* fflush(stderr); */

		/* printf("===cmd_argv===\n"); */
		/* ft_lstiter(parser_out->ast->cmd_argv, token_debug); */
		/* printf("===redirs===\n"); */
		/* ft_lstiter(parser_out->ast->redirs, token_debug); */
		printf("%s\n", "salut ");
		int fds[2] = {MS_NO_FD, MS_NO_FD};
		int eval_out = eval(fds, env, path, parser_out->ast);
		(void)eval_out;
	}
	else
	{
		int		ret;
		char	*line;

		print_prompt();
		while ((ret = ft_getline(STDOUT_FILENO, &line)) == FTGL_OK)
		{
			if (*line == '\0')
			{
				print_prompt();
				continue;
			}
			t_ftlst *lex_out = lexer(line);
			if (lex_out == NULL)
				return (1);

			t_ret *parser_out = parse(lex_out);

			int fds[2] = {MS_NO_FD, MS_NO_FD};
			int eval_out = eval(fds, env, path, parser_out->ast);
			(void)eval_out;
			print_prompt();
		}
	}

	ft_htdestroy(path, free);
	ft_vecdestroy(env, free);
	return (0);
}
// 	else
// 	{
// 		int		ret;
// 		char	*line;
//
// 		printf("> ");
// 		fflush(stdout);
// 		while ((ret = ft_getline(STDOUT_FILENO, &line)) == FTGL_OK)
// 		{
// 			t_ftlst *lex_out = lexer(line);
// 			if (lex_out == NULL)
// 				return (1);
//
// 			t_ret *parser_out = parse(lex_out);
//
// 			int fds[2] = {MS_NO_FD, MS_NO_FD};
// 			int eval_out = eval(fds, env, path, parser_out->ast);
// 			(void)eval_out;
// 			printf("> ");
// 			fflush(stdout);
// 		}
// 	}
//
// 	ft_htdestroy(path, free);
// 	ft_vecdestroy(env, free);
// 	return (0);
// }



/////////////////////////////////////////////////////////////
// eval
/////////////////////////////////////////////////////////////

/* int main(int argc, char **argv, char **envp) */
/* { */
/* (void)argc; */
/* (void)argv; */
/* (void)envp; */
/* t_path	path; */
/* t_env	env; */
/* char	*line; */
/* int		ret; */
/* env = env_from_array(envp); */
/* path = path_update(NULL, env_search(env, "PATH")); */

/* t_ftlst *args1 = NULL; */
/* ft_lstpush_back(&args1, ft_lstnew(token_new(TAG_STR, "ls"))); */
/* ft_lstpush_back(&args1, ft_lstnew(token_new(TAG_STR, "-l"))); */

/* t_ftlst *args2 = NULL; */
/* ft_lstpush_back(&args2, ft_lstnew(token_new(TAG_STR, "cat"))); */
/* ft_lstpush_back(&args2, ft_lstnew(token_new(TAG_STR, "-e"))); */
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

/* t_ftlst *redirs = NULL; */
/* ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_REDIR_OUT, NULL))); */
/* ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_STR, "bonjour"))); */
/* ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_REDIR_APPEND, NULL))); */
/* ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_STR, "yo"))); */
/* ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_REDIR_OUT, NULL))); */
/* ft_lstpush_back(&redirs, ft_lstnew(token_new(TAG_STR, "yo1"))); */

/* t_ast *cmd1 = ast_new(AST_CMD); */
/* cmd1->cmd_argv = args1; */
/* cmd1->redirs = NULL; */

/* t_ast *cmd2 = ast_new(AST_CMD); */
/* cmd2->cmd_argv = args2; */
/* cmd2->redirs = NULL; */

/* t_ast *op_ast = ast_new(AST_OP); */
/* op_ast->op.left = cmd1; */
/* op_ast->op.right = cmd2; */
/* op_ast->op.sep = TAG_PIPE; */


/* int fds[2] = {MS_NO_FD, MS_NO_FD}; */
/* printf("eval %d\n", eval_cmd(fds, env, path, cmd1)); */

/* char **as = preprocess(l, env); */
/* printf("%p\n", as); */
/* printf("%p\n", *as); */
/* char **tmp = as; */
/* while (*as != NULL) */
/* 	puts(*as++); */
/* ft_split_destroy(tmp); */
/* 	ft_htdestroy(path, free); */
/* 	ft_vecdestroy(env, free); */
/* 	return (0); */
/* } */
