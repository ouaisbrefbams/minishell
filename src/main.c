/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/07/19 17:04:19 by charles          ###   ########.fr       */
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

/*
** TODO
** $?
** pipeline
** cmd variable preprocess
** PATH with no permission, link and other file system fun stuff
** escape lexer
** escape split preprocessing (escaped spaces)
** signal on whole line instead of single command
** parsing error
** env local to current minishell process
** BETTER ERROR HANDLING IS BECOMING URGENT (spagetti code everywhere)
*/

char	*g_basename = "minishell";

int main(int argc, char **argv, char **envp)
{
	t_path	path;
	t_env	env;

	env = env_from_array(envp);
	path = path_update(NULL, env_search(env, "PATH"));

	g_last_status_code = 0;
	signal(SIGINT, signal_sigint);
	signal(SIGQUIT, signal_sigquit);
	signal(SIGTERM, signal_sigterm);

	char *last_slash = ft_strrchr(argv[0], '/');
	if (last_slash == NULL)
		g_basename = argv[0];
	else
		g_basename = last_slash + 1;

	char *pwd_var;

	if ((pwd_var = env_search(env, "PWD")) == NULL)
	{
		char buf[PATH_MAX] = {0};
		if (!(getcwd(buf, PATH_MAX)))
			return(1);
		env_export(env, "PWD", buf);
	}

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
		if (parser_out == NULL || parser_out->syntax_error)
			return (1);

		 //ast_print(0, parser_out->ast);

		/* printf("===cmd_argv===\n"); */
		/* ft_lstiter(parser_out->ast->cmd_argv, token_debug); */
		/* printf("===redirs===\n"); */
		/* ft_lstiter(parser_out->ast->redirs, token_debug); */
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
			if (parser_out == NULL)
				return (1);
			if (parser_out->syntax_error)
			{
				print_prompt();
				continue;
			}

			int fds[2] = {MS_NO_FD, MS_NO_FD};
			int eval_out = eval(fds, env, path, parser_out->ast);
			(void)eval_out;
			print_prompt();
		}
	}

	ft_htdestroy(path, free);
	ft_vecdestroy(env, free);
	return (g_last_status_code);
}
