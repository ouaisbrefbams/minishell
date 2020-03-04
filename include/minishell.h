/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 15:33:51 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/28 15:30:10 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>

# include "libft.h"
# include "libft_ht.h"
# include "libft_lst.h"
# include "libft_util.h"

# include "ms_parse.h"

# define MS_PATH_KEY "PATH"
# define MS_PIPE_WRITE 1
# define MS_PIPE_READ 0

typedef struct
{
	t_ftht				*commands;
	t_ftlst				*dirs;
}						t_path;

typedef struct
{
	t_path				*path;
	t_ftht				*environment;
}						t_state;

typedef struct
{
	int					pipe_in[2];
	int					pipe_out[2];
}						t_command_frame;

/*
** state.c
*/

int						ms_state_init(t_state *state, const char **envp);
void					ms_state_destroy(t_state *state);

/*
** eval.c
*/

int						ms_eval(t_parsing *parsing);

/*
** path.c
*/

t_path					*ms_path_update(t_path *path, const char *path_str);
void					ms_path_destroy(t_path *path);

/*
** environment.c
*/

t_ftht					*ms_environment_from_array(const char **envp);
char					**ms_environment_to_array(t_ftht *environment);

/*
** builtin*.c
*/

typedef int     		(*t_builtin_func)(t_state *state);
t_builtin_func 			ms_echo;
t_builtin_func 			ms_cd;
t_builtin_func 			ms_pwd;
t_builtin_func 			ms_export;
t_builtin_func 			ms_unset;
// t_builtin_func 			ms_env;
int                     ms_env(t_state *state);
t_builtin_func 			ms_exit;

/*
** util.c
*/

void				ms_ht_del_str_entry(t_ftht_content *content);

#endif
