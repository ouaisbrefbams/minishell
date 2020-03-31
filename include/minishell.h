/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 15:33:51 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/28 15:30:10 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>  // for debugging - dont remove

#ifndef MINISHELL_H
# define MINISHELL_H

/**
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

/**
** \brief  Value of pipe entry if closed
*/

# define PIPE_CLOSED -1

/**
** \brief  Pipe write index
*/

# define PIPE_WRITE 1

/**
** \brief  Pipe read index
*/

# define PIPE_READ 0

typedef t_ftht*			t_path;
typedef t_ftht*			t_env;

/*
** path.c
*/

t_path					path_update(t_path path, char *path_var);

/*
** env.c
*/

t_env					env_from_array(char **envp);
char					**env_to_array(t_env env);

/*
** builtin*.c - directory with all builtin commands
*/

int                     builtin_echo(char **argv);
int                     builtin_cd(t_env env, char **argv);
int                     builtin_pwd(void);
int                     builtin_export(t_env env, char **argv);
int                     builtin_unset(t_env env, char **argv);
int                     builtin_env(t_env env);
int                     builtin_exit(void);

/*
** util.c - various utilitary functions
*/

void					ht_del_str_entry(t_ftht_entry *entry);

#endif
