/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:30 by charles           #+#    #+#             */
/*   Updated: 2020/09/15 20:09:27 by charles          ###   ########.fr       */
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
# include "parser.h"

typedef struct
{
	t_env			env;
	t_ast			*ast;
	int				fds[2];
}					t_fork_param_args;

typedef struct
{
	char			exec_path[PATH_MAX + 1];
	char			**argv;
	t_env			env;
	t_builtin_entry	*builtin;
}					t_fork_param_cmd;

typedef int (*t_wrapped_func)(void *param);

# define FD_NONE -2
# define FD_WRITE 1
# define FD_READ 0

extern pid_t		g_child_pid;

/*
** eval.c
*/

int					fork_wrap(int fds[2], void *passed, t_wrapped_func wrapped, pid_t *child_pid, int fd_to_close);
int					eval(int fds[2], t_env env, t_ast *ast, pid_t *child_pid, int fd_to_close);

/*
** cmd.c
*/

int					eval_cmd(int fds[2], t_env env, t_ast *ast, pid_t *child_pid, int fd_to_close);

/*
** operation.c
*/

int					eval_operation(int fds[2], t_env env, t_ast *ast);

/*
** parenthesis.c
*/

int					eval_parenthesis(int fds[2], t_env env, t_ast *ast, pid_t *child_pid, int fd_to_close);

/*
** redir.c
*/

int					redir_extract(t_tok_lst **redirs, t_env env, int fds[2]);

#endif
