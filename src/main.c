/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/28 11:50:29 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
** \file   main.c
** \brief  Minishell entrypoint
*/

#include "minishell.h"
#include "ast.h"
#include "eval.h"

/**
** \brief       Program entrypoint
** \param argc  Number of arguments in `argv`
** \param argv  Array of string, argument of the program
** \param envp  NULL terminated array of string representing the environment
** \return      0 if ok, 1 otherwise
*/

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
	path = path_update(NULL, ft_htget(env, "PATH"));

	t_ast *ast;
	t_line line;
	t_cmd cmd;
	t_eval_state state;

	cmd.argv = ft_split("ls -l", ' ');
	cmd.in = NULL;
	cmd.out = NULL;
	cmd.is_append = false;

	line.left = ast_new(TAG_CMD, &cmd);
	line.right = NULL;
	line.sep = SEP_END;
	ast = ast_new(TAG_LINE, &line);

	/* printf("%p\n", ast); */
	/* printf("%d\n", ast->tag); */
	/* printf("%p\n", ast->data.line.left); */
	/* printf("%p\n", ast->data.line.right); */
	/* printf("%d\n", ast->data.line.left->tag); */
	/* printf("%p\n", ast->data.line.left->data.cmd.argv); */
	/* printf("%s\n", ast->data.line.left->data.cmd.argv[0]); */
	/* printf("%s\n", ast->data.line.left->data.cmd.argv[1]); */

	state.in_pipe[0] = -1;
	state.in_pipe[1] = -1;
	state.out_pipe[0] = -1;
	state.out_pipe[1] = -1;
	state.path = path;
	state.env = env;
	eval(&state, ast);

	ast_destroy(ast);
	/* while ((ret = ft_next_line(STDIN_FILENO, &line)) == 1) */
	/* { */
	/* 	if (eval(parse(line)) == -1) */
	/* 		continue ;  // and display error */
	/* 	free(line); */
	/* } */
	/* free(line); */
	ft_htdestroy(path, ht_del_str_entry);
	ft_htdestroy(env, ht_del_str_entry);
	return (0);
}
