/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:30 by charles           #+#    #+#             */
/*   Updated: 2020/09/13 14:20:17 by charles          ###   ########.fr       */
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
	t_path			path;
	t_env			env;
	t_ast			*ast;
	int				fds[2];
}					t_fork_param_args;

typedef struct
{
	char			*exec_path;
	char			**argv;
	t_env			env;
	t_builtin_entry	*builtin;
}					t_fork_param_cmd;

# define FD_NONE -2
# define FD_WRITE 1
# define FD_READ 0

extern pid_t		g_child_pid;

/*
** op.c
*/

int					eval_op(int fds[2], t_env env, t_path path, t_ast *ast);
int					eval(int fds[2], t_env env, t_path path, t_ast *ast, pid_t *child_pid);
int					eval_forked(int fds[2], t_env env, t_path path, t_ast *ast, pid_t *child_pid);

/*
** cmd.c
*/

int					fork_wrap(int fds[2], void *passed, int (*wrapped)(void *param), pid_t *child_pid);
int					fork_wrap_wait(int fds[2], void *passed, int (*wrapped)(void *param));
int					eval_cmd(int fds[2], t_env env, t_path path, t_ast *ast, pid_t *child_pid);

/*
** redir.c
*/

int					redir_extract(t_tok_lst **redirs, t_env env, int fds[2]);

/*
** exec.c
*/

bool				exec_is_path(char *exec_name);
bool				exec_is_valid(char *exec_path);
char				*exec_search_path(t_path path, char *path_var, char *exec_name);

#endif
