/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/06 16:14:07 by cacharle         ###   ########.fr       */
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

t_state	g_state;

int		execute(t_env env, char *input)
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
		return (2);
	fds[0] = FD_NONE;
	fds[1] = FD_NONE;
	status = eval(fds, env, parser_out->ast, NULL, FD_NONE);
	/* ast_destroy(parser_out->ast); */
	free(parser_out);
	if (status == EVAL_FATAL)
		exit(1);
	g_state.last_status = status;
	return (status);
}

int		repl(t_env env)
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
			return (2);
		print_prompt();
	}
	if (ret != FTGL_EOF)
		return (2);
	return (0);
}

#ifndef MINISHELL_TEST

int		main(int argc, char **argv, char **envp)
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
	/* g_state.child_pid = 0; */
	repl(env);
	ft_vecdestroy(env, free);
	return (g_state.last_status);
}

#else

int		main(int argc, char **argv, char **envp)
{
	int		status;
	t_env	env;

	if ((env = env_from_array(envp)) == NULL)
		return (1);
	setup(argv[0], env);
	g_state.last_status = 0;
	if (argc == 3 && ft_strcmp(argv[1], "-l") == 0)
		return (debug_lexer(argv[2]));
	if (argc == 3 && ft_strcmp(argv[1], "-p") == 0)
		return (debug_parser(argv[2]));
	if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		status = execute(env, argv[2]);
		ft_vecdestroy(env, free);
		return (status);
	}
	repl(env);
	return (g_state.last_status);
}

#endif
