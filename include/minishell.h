/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 15:33:51 by cacharle          #+#    #+#             */
/*   Updated: 2020/09/15 17:43:16 by charles          ###   ########.fr       */
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
# include <stdarg.h>

# include "libft.h"
# include "libft_ht.h"
# include "libft_lst.h"
# include "libft_util.h"
# include "libft_vec.h"
# include "libft_dstr.h"

# include "lexer.h"
# include "error.h"

/*
** \brief  Value of pipe entry if closed
*/

# define PIPE_CLOSED -1

# define BUILTIN_NOT_FOUND -2

typedef t_ftvec*	t_env;

extern int			g_last_status;
extern char			*g_basename;

/*
** path.c
*/

int					path_search(t_env env, char *exec_name, char exec_path[PATH_MAX + 1], bool print);

/*
** env.c
*/

t_env				env_from_array(char **envp);
char				*env_search(t_env env, char *key, size_t *found_index);
char				*env_export(t_env env, char *key, char *value);
char				*env_search_first_match(t_env env, const char *haystack);
size_t				env_key_len(char *key, bool allow_status);

/*
** builtin*.c - directory with all builtin commands
*/

/*
** \brief       Type of a builtin main function
*/

typedef int			(*t_builtin_func)(char **argv, t_env env);

/*
** \brief       Entry of builtin lookup array
** \param name  Executable name of builtin
** \param func  Associated function
*/

typedef struct
{
	char			*name;
	t_builtin_func	func;
	bool			forked;
}					t_builtin_entry;

t_builtin_entry		*builtin_search_func(char *name);

int					builtin_echo(char **argv, t_env env);
int					builtin_cd(char **argv, t_env env);
int					builtin_pwd(char **argv, t_env env);
int					builtin_export(char **argv, t_env env);
int					builtin_unset(char **argv, t_env env);
int					builtin_env(char **argv, t_env env);
int					builtin_exit(char **argv, t_env env);

/*
** preprocess.c
*/

char				**preprocess(t_tok_lst **tokens, t_env env);
int					preprocess_filename(t_tok_lst **tokens, t_env env, char **filename);

/*
** signal.c
*/

void				signal_sigint(int signum);
void				signal_sigquit(int signum);
void				signal_sigterm(int signum);

/*
** utils.c
*/

void				print_prompt(void);

#endif
