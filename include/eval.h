/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:30 by charles           #+#    #+#             */
/*   Updated: 2020/08/27 17:15:19 by charles          ###   ########.fr       */
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

typedef struct
{
	t_path		path;
	t_env		env;
	t_ast		*ast;
	int			fds[2];
}					t_fork_param_parent;

typedef struct
{
	char			*exec_path;
	char			**argv;
	t_env			env;
	t_env			env_local;
	t_builtin_entry	*builtin;
}					t_fork_param_cmd;

# define MS_NO_FD -2
# define FDS_WRITE 1
# define FDS_READ 0

extern pid_t	g_child_pid;

/*
** op.c
*/

int				eval_op(int fds[2], t_env env, t_path path, t_ast *ast);
int				eval(int fds[2], t_env env, t_path path, t_ast *ast);

/*
** cmd.c
*/

int				fork_wrap(int fds[2], void *passed, int (*wrapped)(void *param));
int				eval_cmd(int fds[2], t_env env, t_path path, t_ast *ast);
t_ftlst         *split_token(t_ftlst **lst, enum e_tok);

/*
** redir.c
*/

bool			redir_extract(t_tok_lst *redirs, t_env env, int fds[2]);

/*
** exec.c
*/

bool			exec_is_path(char *exec_name);
bool			exec_is_valid(char *exec_path);
char			*exec_search_path(t_path path, char *path_var, char *exec_name);

#endif
