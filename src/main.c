/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <cacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/10/10 09:25:52 by cacharle         ###   ########.fr       */
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

t_state	g_state;

int		run_eval(t_env env, t_parsed *parser_out)
{
	int	status;
	int	fds[2];

	fds[0] = FD_NONE;
	fds[1] = FD_NONE;
	status = eval(fds, env, parser_out->ast);
	ast_destroy(parser_out->ast);
	free(parser_out);
	if (status == EVAL_FATAL)
		exit(1);
	g_state.last_status = status;
	return (status);
}

int		execute(t_env env, char *input)
{
	t_tok_lst	*lexer_out;
	int			status;
	t_parsed	*parser_out;

	if (utils_strisblank(input))
		return (0);
	status = lexer(input, &lexer_out);
	if (status != 0)
	{
		tok_lst_destroy(&lexer_out, free);
		return (status);
	}
	parser_out = parse(lexer_out);
	if (parsed_check(parser_out))
	{
		free(parser_out);
		return (2);
	}
	return (run_eval(env, parser_out));
}

int		repl(t_env env)
{
	int		ret;
	char	*line;

	print_prompt();
	while ((ret = ft_getline(STDIN_FILENO, &line)) == FTGL_OK)
	{
		g_state.killed = false;
		if (execute(env, line) == EVAL_FATAL)
			return (2);
		g_child_pid = -1;
		print_prompt();
		free(line);
	}
	free(line);
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
	g_state.child_pid = -1;
	repl(env);
	ft_vecdestroy(env, free);
	return (g_state.last_status);
}

#else

int		main(int argc, char **argv, char **envp)
{
	t_env	env;

	if ((env = env_from_array(envp)) == NULL)
		return (1);
	setup(argv[0], env);
	g_state.last_status = 0;
	if (argc == 3 && ft_strcmp(argv[1], "-l") == 0)
		g_state.last_status = debug_lexer(argv[2]);
	else if (argc == 3 && ft_strcmp(argv[1], "-p") == 0)
		g_state.last_status = debug_parser(argv[2]);
	else if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
		g_state.last_status = execute(env, argv[2]);
	else
		repl(env);
	ft_vecdestroy(env, free);
	return (g_state.last_status);
}

#endif
