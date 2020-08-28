/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/08/28 10:45:33 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   main.c
** \brief  Minishell entrypoint
*/

#include "minishell.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

/* void token_debug(void *v); */
/* void token_put(void *v); */
/* void print_level(int level); */
/* void ast_print(int level, t_ast *ast); */
void tok_lst_debug(t_tok_lst *tokens);

/*
** TODO
** $?
** pipeline
** cmd variable preprocess
** PATH with no permission, link and other file system fun stuff
** escape lexer ------- ok
** escape split preprocessing (escaped spaces)
** signal on whole line instead of single command
** parsing error ------ ok
** env local to current minishell process
** BETTER ERROR HANDLING IS BECOMING URGENT (spagetti code everywhere)
*/

bool	env_set_default(t_env env, char *key, char *value)
{
	if (env_search(env, key) != NULL)
		return (true);
	return (env_export(env, key, value) != NULL);
}

char	*g_basename = "minishell";

int main(int argc, char **argv, char **envp)
{
	t_path	path;
	t_env	env;

	env = env_from_array(envp);

	char buf[PATH_MAX] = {0};
	if (!(getcwd(buf, PATH_MAX)))
		return(1);
	if (!env_set_default(env, "PWD", buf) ||
		!env_set_default(env, "SHLVL", "1") ||   // TODO increment if set
		!env_set_default(env, "PATH", "/sbin:")) // FIXME need to prefix if /sbin not in
		return (1);

	path = path_update(NULL, env_search(env, "PATH"));

	/* char *env_exec_path; */
	// if ((env_exec_path = ft_htget(path, "env")) == NULL)
	// {
	// 	errorf("env: command not found\n");
	// 	return (127);
	// }
	// env_export(env, "_", env_exec_path);

	g_last_status_code = 0;
	/* signal(SIGINT, signal_sigint); */
	/* signal(SIGQUIT, signal_sigquit); */
	/* signal(SIGTERM, signal_sigterm); */

	char *last_slash = ft_strrchr(argv[0], '/');
	if (last_slash == NULL)
		g_basename = argv[0];
	else
		g_basename = last_slash + 1;

	if (argc == 3 && ft_strcmp(argv[1], "-l") == 0)
	{
		t_tok_lst *lex_out = lexer(argv[2]);
		if (lex_out == NULL)
			return (1);
		tok_lst_debug(lex_out);
		/* ft_lstiter((t_ftlst*)lex_out, token_debug); */
	}
	else if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		t_tok_lst *lex_out = lexer(argv[2]);
		if (lex_out == NULL)
			return (1);

		t_parsed *parser_out = parse(lex_out);
		if (parser_out == NULL || parser_out->syntax_error)
			return (1);

		 /* ast_print(0, parser_out->ast); */

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
			t_tok_lst *lex_out = lexer(line);
			if (lex_out == NULL)
				return (1);

			t_parsed *parser_out = parse(lex_out);
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
		if (ret != FTGL_EOF)
			return (1);
	}

	ft_htdestroy(path, free);
	ft_vecdestroy(env, free);
	return (g_last_status_code);
}
