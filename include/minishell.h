/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 15:33:51 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/26 16:10:56 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "libft_ht.h"


typedef struct
{
	int		argc;
	char	**argv;
}						t_command;

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

// parsing steps
// 1. interpolation des variable

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
