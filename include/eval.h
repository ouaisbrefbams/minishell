/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:30 by charles           #+#    #+#             */
/*   Updated: 2020/05/04 11:58:16 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

/*
** \file   eval.h
** \brief  Evaluation module
*/

# include "minishell.h"
# include "ast.h"

/*
** \brief        Evaluation state struct
*/

typedef struct
{
	int			p[2];
	t_path		path;
	t_env		env;
}				t_eval_state;

/*
** \brief      Evaluation status struct
*/

typedef struct	s_eval_status
{
	char		*err;
	int			status;
}				t_eval_status;

typedef struct
{
	t_eval_state	*state;
	t_line			*line;
	int fd_in;
	int fd_out;
}					t_fork_param_line;

typedef struct
{
	char			*exec_path;
	char			**argv;
	char			**envp;
}					t_fork_param_execve;

/*
** eval.c
*/

int				eval(int fd_in, int fd_out, t_eval_state *state, t_ast *ast);

/*
** exec.c
*/

bool			exec_is_path(char *path_str);
bool			exec_is_valid(char *exec_path);
char			*exec_search_path(t_path path, char *path_var, char *exec_name);

/*
** pipe.c
*/

int				pipe_setup_parent(t_cmd *cmd, int pipe_in[2], int pipe_out[2]);
int				pipe_setup_child(int pipe_in[2], int pipe_out[2]);

#endif
