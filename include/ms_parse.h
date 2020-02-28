/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 09:00:00 by cacharle          #+#    #+#             */
/*   Updated: 2020/02/28 14:57:58 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_PARSE_H
# define MS_PARSE_H

# include "minishell.h"

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
	t_ftlst				*redirections;  // need to store them in reverse order
}						t_command;

typedef enum
{
	SEPARATOR_SEMICOLON,
	SEPARATOR_PIPE,
	// SEPARATOR_AND,  // with parhenthesis => harder
	// SEPARATOR_OR,
}						t_separator;

typedef struct
{
	t_ftlst 			*commands;
	t_ftlst 			*separators;
}						t_parsing;

/*
** parse.c
*/

t_parsing				*ms_parse(char *input);

#endif
