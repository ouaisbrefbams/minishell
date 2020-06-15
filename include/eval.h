/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:30 by charles           #+#    #+#             */
/*   Updated: 2020/06/15 11:09:49 by charles          ###   ########.fr       */
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
	t_env			env;
	t_builtin_func	builtin;
}					t_fork_param_cmd;

#define MS_NO_FD -2

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

enum			e_error
{
	ERROR_AMBIGUOUS_REDIR,
	ERROR_OPEN,
	ERROR_CMD_NOT_FOUND,
	ERROR_SYNTAX,
};

typedef struct
{
	enum e_error	id;
	int				status;
	char			*msg;       // if NULL call strerror
	// char			*basename;
}					t_error;

/*
** error.c
*/

void			error_eval_put(enum e_error id, char *unexpected);

/*
** cmd.c
*/

int				eval_cmd(t_env env, t_path path, t_ast *ast);

/*
** redir.c
*/

bool			redir_extract(t_ftlst *redirs, t_env env, int *fd_in, int *fd_out);

#endif
