/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 15:33:51 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/27 18:07:06 by cacharle         ###   ########.fr       */
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

typedef int 			t_status;

typedef enum
{
	REDIRECTION_OUT,
	REDIRECTION_IN,
	REDIRECTION_APPEND
}						t_redirection_type;

typedef struct
{
	char				*filename;
	t_redirection_type	type;
}						t_redirection;

typedef struct
{
	char				*name;
	char				**argv;
	t_ftlst				*redirections;
}						t_command;

typedef enum
{
	SEPARATOR_SEMICOLON,
	SEPARATOR_PIPE,
	SEPARATOR_AND,
	SEPARATOR_OR,
}						t_separator;

typedef struct
{
	t_ftlst 			*commands;
	t_ftlst 			*separators;
}						t_parsing;

/*
** parse/.c
*/

t_parsing				*ms_parse(char *input);

/*
** path.c
*/

typedef struct
{
	t_ftht				*commands;
	t_ftlst				*dirs;
}						t_path;

t_path					*ms_path_update(t_path *path, const char *path_str);
void					ms_path_destroy(t_path *path);

/*
** environment.c
*/

// t_ftht					*ms_environment_update(t_ftht *environment, char **envp);
// char					**ms_environment_(t_ftht *environment, char **envp);
// void					ms_environment_destroy(t_ftht *environment);


/*
** builtin*.c
*/

typedef t_status		(*t_builtin_func)(int argc, char **argv, char **envp);
t_builtin_func 			ms_echo;
t_builtin_func 			ms_cd;
t_builtin_func 			ms_pwd;
t_builtin_func 			ms_export;
t_builtin_func 			ms_unset;
t_builtin_func 			ms_env;
t_builtin_func 			ms_exit;

#endif
