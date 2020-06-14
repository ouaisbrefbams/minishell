/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cacharle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 11:45:44 by cacharle          #+#    #+#             */
/*   Updated: 2020/06/13 11:56:27 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   main.c
** \brief  Minishell entrypoint
*/

#include "minishell.h"
#include "ast.h"
#include "lexer.h"
#include "parser.h"


/////////////////////////////////////////////////////////////////////////////////////////
// lexer main
/////////////////////////////////////////////////////////////////////////////////////////

 int         main(void)
 {
	 char *input;

 	if (!(input = malloc(sizeof(char) * ft_strlen(argv[1]) + 1)))
      	return(0);
  	ft_strlcpy(input, argv[1], ft_strlen(argv[1]) + 1);

    lexer(input);
	exit(0);
    return (0);
 } */
