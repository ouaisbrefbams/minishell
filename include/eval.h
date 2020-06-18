/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:30 by charles           #+#    #+#             */
/*   Updated: 2020/06/18 13:38:53 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVAL_H
# define EVAL_H

/*
** \file   eval.h
** \brief  Evaluation module
*/

# include "minishell.h"
# include "lexer.h"
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
	t_op			*op;
	int fd_in;
	int fd_out;
}					t_fork_param_op;

typedef struct
{
	char			*exec_path;
	char			**argv;
	t_env			env;
	t_builtin_func	builtin;
}					t_fork_param_cmd;

# define MS_NO_FD -2
# define FDS_WRITE 1
# define FDS_READ 0

/*
** op.c
*/

int			eval_op(int fds[2], t_env env, t_path path, t_op *op);
int			eval(int fds[2], t_env env, t_path path, t_ast *ast);

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

int				fork_wrap(int fds[2], void *passed, int (*wrapped)(void *param));
int				eval_cmd(int fds[2], t_env env, t_path path, t_ast *ast);

/*
** redir.c
*/

bool			redir_extract(t_ftlst *redirs, t_env env, int fds[2]);

/*
** exec.c
*/

bool	exec_is_path(char *exec_name);
bool	exec_is_valid(char *exec_path);
char	*exec_search_path(t_path path, char *path_var, char *exec_name);

#endif
