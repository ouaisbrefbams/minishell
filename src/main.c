/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/09/16 16:14:21 by charles          ###   ########.fr       */
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

int		debug_lexer(char *input);

/*
** TODO
** signal whole line
** path optimisation on command not found
** path tricks
*/

char	*g_progname = "minishell";

int	execute(t_env env, char *input)
{
	t_tok_lst	*lexer_out;
	int			status;
	t_parsed	*parser_out;
	int			fds[2];

	status = lexer(input, &lexer_out);
	if (status != 0)
		return (status);
	parser_out = parse(lexer_out);
	if (parser_out == NULL || parser_out->syntax_error)
		return (1);
	fds[0] = FD_NONE;
	fds[1] = FD_NONE;
	status = eval(fds, env, parser_out->ast, NULL, FD_NONE);
	if (status == EVAL_FATAL)
		exit(1);
	g_last_status = status;
	return (status);
}

int	repl(t_env env)
{
	int		ret;
	char	*line;

	print_prompt();
	while ((ret = ft_getline(STDIN_FILENO, &line)) == FTGL_OK)
	{
		if (*line == '\0')
		{
			print_prompt();
			continue ;
		}
		if (execute(env, line) == EVAL_FATAL)
			return (1);
		print_prompt();
	}
	if (ret != FTGL_EOF)
		return (1);
	return (0);
}

#ifndef MINISHELL_TEST

int	main(int argc, char **argv, char **envp)
{
	t_env	env;

	if (argc != 1)
	{
		ft_putendl_fd("minishell doesn't accept any arguments", STDERR_FILENO);
		return (1);
	}
	if ((env = env_from_array(envp)) == NULL)
		return (1);
	setup(argv[0], env);
	g_last_status = 0;
	repl(env);
	ft_vecdestroy(env, free);
	return (g_last_status);
}

#else

int	main(int argc, char **argv, char **envp)
{
	t_env	env;

	if ((env = env_from_array(envp)) == NULL)
		return (1);
	setup(argv[0], env);
	g_last_status = 0;
	if (argc == 3 && ft_strcmp(argv[1], "-l") == 0)
		return (debug_lexer(argv[2]));
	if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
		return (execute(env, argv[2]));
	repl(env);
	ft_vecdestroy(env, free);
	return (g_last_status);
}

#endif
