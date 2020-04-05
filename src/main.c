/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/04/05 12:15:57 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   main.c
** \brief  Minishell entrypoint
*/

#include "minishell.h"
#include "ast.h"
#include "eval.h"
#include "ms_glob.h"

/*
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
	/* t_path	path; */
	t_env	env;
	/* char	*line; */
	/* int		ret; */

	env = env_from_array(envp);
	/* path = path_update(NULL, env_search(env, "PATH")); */
    /*  */
	/* t_ast *ast; */
	/* t_line line; */
	/* t_cmd cmd; */
	/* t_eval_state state; */
    /*  */
	/* cmd.argv = ft_split("ls -l", ' '); */
	/* cmd.in = NULL; */
	/* cmd.out = NULL; */
	/* cmd.is_append = false; */
    /*  */
	/* line.left = ast_new(TAG_CMD, &cmd); */
	/* line.right = NULL; */
	/* line.sep = SEP_END; */
	/* ast = ast_new(TAG_LINE, &line); */

	/* printf("%p\n", ast); */
	/* printf("%d\n", ast->tag); */
	/* printf("%p\n", ast->data.line.left); */
	/* printf("%p\n", ast->data.line.right); */
	/* printf("%d\n", ast->data.line.left->tag); */
	/* printf("%p\n", ast->data.line.left->data.cmd.argv); */
	/* printf("%s\n", ast->data.line.left->data.cmd.argv[0]); */
	/* printf("%s\n", ast->data.line.left->data.cmd.argv[1]); */

	/* state.pipe_in[0] = -1; */
	/* state.pipe_in[1] = -1; */
	/* state.pipe_out[0] = -1; */
	/* state.pipe_out[1] = -1; */
	/* state.path = path; */
	/* state.env = env; */
	/* t_io_frame frame; */
	/* io_frame_init(&frame); */
	/* printf("ret: %d %s\n", eval(&frame,&state, ast), strerror(errno)); */

	/* char buf[2048]; */
	/* printf("%s\n", getcwd(buf, 2048)); */
	/* builtin_env(NULL, state.env); */
	/* ast_destroy(ast); */
	/* while ((ret = ft_next_line(STDIN_FILENO, &line)) == 1) */
	/* { */
	/* 	if (eval(parse(line)) == -1) */
	/* 		continue ;  // and display error */
	/* 	free(line); */
	/* } */
	/* free(line); */
	/* ft_htdestroy(path, free); */
	/* ms_glob("src#<{(|"); */
	char *j = ms_glob("*/*.c");
	printf("%s\n", j);
	free(j);
	ft_vecdestroy(env, free);
	return (0);
}
