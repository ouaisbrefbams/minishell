/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:51:54 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/28 12:36:10 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int						ms_state_init(t_state *state, const char **envp)
{
	if ((state->environment = ms_environment_from_array(envp)) == NULL)
		return (-1);
	state->path = NULL;
	if ((state->path = ms_path_update(state->path, ft_htget(state->environment, MS_PATH_KEY))) == NULL)
		return (-1);
	return (0);
}

void					ms_state_destroy(t_state *state)
{
	if (state == NULL)
		return ;
	ms_path_destroy(state->path);
	ft_htdestroy(state->environment, ms_ht_del_str_entry);
}
