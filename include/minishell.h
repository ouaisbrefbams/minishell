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

# include "ms_parse.h"

# define MS_PATH_KEY "PATH"
# define MS_PIPE_WRITE 1
# define MS_PIPE_READ 0

typedef t_ftht*			t_path;
typedef t_ftht*			t_env;

/*
** eval.c
*/

typedef struct
{
	t_status_type		type;
	union
	{
		char			*error_msg;
		char			*arg;
		int				code;
	}					value;
}						t_status;

typedef enum
{
	STYPE_ERROR,
	STYPE_ARG,
	STYPE_FILE,
	// ...
}						t_status_type;

int						ms_eval(t_path path, t_env env, t_ast *ast);

/*
** path.c
*/

t_path					ms_path_update(t_path path, char *path_var);

/*
** env.c
*/

t_env					ms_env_from_array(char **envp);
char					**ms_env_to_array(t_env env);

/*
** builtin*.c - directory with all builtin commands
*/

int                     ms_echo(char **argv);
int                     ms_cd(t_env env, char **argv);
int                     ms_pwd(void);
int                     ms_export(t_env env, char **argv);
int                     ms_unset(t_env env, char **argv);
int                     ms_env(t_env env);
int                     ms_exit(void);

/*
** util.c - various utilitary functions
*/

void					ms_ht_del_str_entry(t_ftht_content *content);

#endif
