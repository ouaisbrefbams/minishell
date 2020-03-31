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

/**
** \brief       Program entrypoint
** \param argc  Number of arguments in `argv`
** \param argv  Array of string, argument of the program
** \param envp  NULL terminated array of string representing the environment
** \return      0 if ok, 1 otherwise
*/

int main(int argc, char **argv, const char **envp)
{
	t_path	path;
	t_env	env;
	/* char	*line; */
	/* int		ret; */

    (void)argc;
    (void)argv;
	env = env_from_array((char**)envp);
	path = path_update(NULL, ft_htget(env, "PATH"));

	printf("%s\n", (char*)ft_htget(path, "nmap"));
	/* env(env); */
	/* pwd(&state); */
	/* cd(&state, NULL); */
	/* pwd(&state); */

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
