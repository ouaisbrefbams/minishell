/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/09/15 13:31:30 by charles          ###   ########.fr       */
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
** signal whole line
** path optimisation on command not found
** path tricks
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
	t_env	env;

	env = env_from_array(envp);

	char buf[PATH_MAX] = {0};
	if (!(getcwd(buf, PATH_MAX)))
		return(1);

	char *shlvl_str = env_search(env, "SHLVL");
	if (shlvl_str != NULL)
	{
		shlvl_str = ft_itoa(ft_atoi(shlvl_str));
		env_export(env, "SHLVL", shlvl_str);
		free(shlvl_str);
	}

	if (!env_set_default(env, "PWD", buf) ||
		!env_set_default(env, "SHLVL", "0") ||
		!env_set_default(env, "PATH", "/sbin:"))
		return (1);

	/* char *path_str = env_search(env, "PATH"); */
	/* if (ft_strstr(path_str, "/sbin") == NULL) */
	/* { */
	/* 	char *value = ft_strjoin("/sbin:", path_str); */
	/* 	env_export(env, "PATH", value); */
	/* 	free(value); */
	/* } */

	char env_exec_path[PATH_MAX + 1];
	if (path_search(env, "env", env_exec_path, false) != 0)
	{
		ft_strcpy(env_exec_path, "/sbin/env");
		/* errorf("env: command not found\n"); */
		/* return (127); */
	}
	env_export(env, "_", env_exec_path);

	g_last_status = 0;
	signal(SIGINT, signal_sigint);
	signal(SIGQUIT, signal_sigquit);
	signal(SIGTERM, signal_sigterm);

	char *last_slash = ft_strrchr(argv[0], '/');
	if (last_slash == NULL)
		g_basename = argv[0];
	else
		g_basename = last_slash + 1;

	if (argc == 3 && ft_strcmp(argv[1], "-l") == 0)
	{
		t_tok_lst *lex_out;
		int status = lexer(argv[2], &lex_out);
		if (status != 0)
			return (status);
		tok_lst_debug(lex_out);
		/* ft_lstiter((t_ftlst*)lex_out, token_debug); */
	}
	else if (argc == 3 && ft_strcmp(argv[1], "-c") == 0) // put in MINISHELL_TEST
	{
		t_tok_lst *lex_out;
		int status = lexer(argv[2], &lex_out);
		if (status != 0)
			return (status);

		t_parsed *parser_out = parse(lex_out);
		if (parser_out == NULL || parser_out->syntax_error)
			return (1);

		 /* ast_print(0, parser_out->ast); */

		/* printf("===cmd_argv===\n"); */
		/* ft_lstiter(parser_out->ast->cmd_argv, token_debug); */
		/* printf("===redirs===\n"); */
		/* ft_lstiter(parser_out->ast->redirs, token_debug); */
		int fds[2] = {FD_NONE, FD_NONE};
		status = eval(fds, env, parser_out->ast, NULL);
		if (status == EVAL_FATAL)
			exit(1);
		g_last_status = status;
		exit(status);
		return (0);
	}
	else
	{
		int		ret;
		char	*line;
		int		status;

		print_prompt();
		while ((ret = ft_getline(STDIN_FILENO, &line)) == FTGL_OK)
		{
			if (*line == '\0')
			{
				print_prompt();
				continue;
			}
			t_tok_lst *lex_out;
			status = lexer(line, &lex_out);
			if (status != 0)
				return (status);

			t_parsed *parser_out = parse(lex_out);
			if (parser_out == NULL)
				return (1);
			if (parser_out->syntax_error)
			{
				print_prompt();
				continue;
			}

			int fds[2] = {FD_NONE, FD_NONE};
			int status = eval(fds, env, parser_out->ast, NULL);
			if (status == EVAL_FATAL)
				exit(1);
			g_last_status = status;
			print_prompt();
		}
		if (ret != FTGL_EOF)
			return (1);
	}

	ft_vecdestroy(env, free);
	return (g_last_status);
}
