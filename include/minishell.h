/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 15:33:51 by cacharle          #+#    #+#             */
/*   Updated: 2020/08/27 20:34:44 by charles          ###   ########.fr       */
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

typedef t_ftht*			t_path;
typedef t_ftvec*		t_env;

extern int		g_last_status_code;
extern char		*g_basename;

/*
** path.c
*/

t_path					path_update(t_path path, char *path_var);

/*
** env.c
*/

// FIXME refactor env code
t_env					env_from_array(char **envp);
int						env_keycmp(char *var, char *key);
char					*env_search(t_env env, char *key);
int						env_search_index(t_env env, char *key);
char					*env_search_first_match(t_env env, const char *haystack);
char					*env_export(t_env env, char *key, char *value);
char					*env_export_full(t_env env, char *variable);

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

typedef struct
{
	char				*name;
	t_builtin_func		func;
	bool				forked;
}						t_builtin_entry;

t_builtin_entry			*builtin_search_func(char *name);

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

char					**preprocess(t_tok_lst **tokens, t_env env);
char					*preprocess_filename(t_tok_lst **tokens, t_env env);

/*
** signal.c
*/

void signal_sigint(int signum);
void signal_sigquit(int signum);
void signal_sigterm(int signum);

/*
** utils.c
*/

typedef int	(*t_directory_iter_func)(char*, struct dirent*, void*);

int		utils_directory_iter(
	char *dirname,
	void *param,
	t_directory_iter_func f
);

size_t	utils_var_end(char *name);
bool	utils_valid_identifier(char *name);
bool	utils_start_with_valid_identifier(char *name);

void print_prompt(void);

#endif
