/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 15:33:51 by cacharle          #+#    #+#             */
/*   Updated: 2020/06/12 11:57:36 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdio.h>  // for debugging - dont remove

/*
** \file   minishell.h
** \brief  Common header
*/

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <stdbool.h>

# include "libft.h"
# include "libft_ht.h"
# include "libft_lst.h"
# include "libft_util.h"
# include "libft_vec.h"
# include "libft_dstr.h"

/*
** \brief  Value of pipe entry if closed
*/

# define PIPE_CLOSED -1

/*
** \brief  Pipe write index
*/

# define PIPE_WRITE 1

/*
** \brief  Pipe read index
*/

# define PIPE_READ 0

# define BUILTIN_NOT_FOUND -2

typedef t_ftht*			t_path;
typedef t_ftvec*		t_env;

/*
** path.c
*/

t_path					path_update(t_path path, char *path_var);

/*
** env.c
*/

t_env					env_from_array(char **envp);
char					*env_search(t_env env, char *key);
char					*env_search_first_match(t_env env, const char *haystack);

/*
** builtin*.c - directory with all builtin commands
*/

/*
** \brief       Type of a builtin main function
*/

typedef int	(*t_builtin_func)(char **argv, t_env env);

/*
** \brief       Entry of builtin lookup array
** \param name  Executable name of builtin
** \param func  Associated function
*/

struct					s_builtin_entry
{
	char				*name;
	t_builtin_func		func;
};

int						builtin_dispatch_run(char **argv, t_env env);
bool					builtin_check_exec_name(char *exec_name);
int						builtin_echo(char **argv, t_env env);
int						builtin_cd(char **argv, t_env env);
int						builtin_pwd(char **argv, t_env env);
int						builtin_export(char **argv, t_env env);
int						builtin_unset(char **argv, t_env env);
int						builtin_env(char **argv, t_env env);
int						builtin_exit(char **argv, t_env env);

/*
** preprocess.c
*/

char					**preprocess(t_ftvec *argv, t_env env);

#endif
