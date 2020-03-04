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

#include "minishell.h"

int main(int argc, char **argv, const char **envp)
{
	t_state	state;
	/* char	*line; */
	/* int		ret; */

    (void)argc;
    (void)argv;
	if (ms_state_init(&state, envp) == -1)
		return (1);
	ms_env(&state);

	/* while ((ret = ft_next_line(STDIN_FILENO, &line)) == 1) */
	/* { */
	/* 	if (ms_eval(ms_parse(line)) == -1) */
	/* 		continue ;  // and display error */
	/* 	free(line); */
	/* } */
	/* free(line); */
	ms_state_destroy(&state);
	return (0);
}
