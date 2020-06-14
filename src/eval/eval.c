/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:21 by charles           #+#    #+#             */
/*   Updated: 2020/06/14 10:42:37 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   eval.c
** \brief  Evaluation of an AST
*/

/* #include "eval.h" */

/* #<{(| */
/* ** \brief        Evaluate a line */
/* ** \param state  State of the evaluation */
/* ** \param line   Line to evaluate */
/* ** \return       Last Executed command status or -1 on error */
/* |)}># */
/*  */
/* static int	eval_line(void *param) */
/* { */
/* 	int				status; */
/* 	t_eval_state	*state; */
/* 	t_line			*line; */
/* 	int fd_in; */
/* 	int fd_out; */
/*  */
/* 	state = ((t_fork_param_line*)param)->state; */
/* 	line = ((t_fork_param_line*)param)->line; */
/* 	fd_in = ((t_fork_param_line*)param)->fd_in; */
/* 	fd_out = ((t_fork_param_line*)param)->fd_out; */
/*  */
/* 	#<{(| if (line->right == NULL) |)}># */
/* 	#<{(| 	return (eval(state, line->left)); |)}># */
/*  */
/* 	#<{(| if (line->sep == SEP_PIPE) |)}># */
/* 	#<{(| 	pipe(state->p); |)}># */
/*  */
/* 	if (line->left->tag == AST_LINE) */
/* 	{ */
/* 		return (fork_wrap(fd_in, fd_out, param, &eval_line)); */
/* 	} */
/* 	if ((status = eval(fd_in, fd_out, state, line->left)) == -1) */
/* 		return (-1); */
/* 	if ((line->sep == SEP_AND && status != 0) || */
/* 		(line->sep == SEP_OR && status == 0)) */
/* 		return (status); */
/*  */
/* 	return (eval(fd_in, fd_out, state, line->right)); */
/* } */
/*  */
/* #<{(| */
/* ** \brief        Evaluate an AST */
/* ** \param state  State of the evaluation */
/* ** \param ast    Abstract syntax tree to evaluate */
/* ** \return       Last command status or -1 on error */
/* |)}># */
/*  */
/* int			eval(int fd_in, int fd_out, t_eval_state *state, t_ast *ast) */
/* { */
/* 	t_fork_param_line param; */
/*  */
/* 	errno = 0; */
/* 	if (ast->tag == TAG_LINE) */
/* 	{ */
/* 		param.state = state; */
/* 		param.line = &ast->line; */
/* 		param.fd_in = fd_in; */
/* 		param.fd_out = fd_out; */
/* 		return (eval_line(&param)); */
/* 	} */
/* 	if (ast->tag == TAG_CMD) */
/* 		return (eval_cmd(fd_in, fd_out, state, &ast->cmd)); */
/* 	return (-1); */
/* } */
