/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:21 by charles           #+#    #+#             */
/*   Updated: 2020/05/04 12:00:38 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   eval.c
** \brief  Evaluation of an AST
*/

#include "eval.h"

/*
** \brief          Wrap a function in a fork
** \param fd_in    fork input file descriptor
** \param fd_out   fork output file descriptor
** \param passed   param of the wrapped function
** \param wrapped  function to wrap
*/

int			fork_wrap(
	int	fd_in,
	int fd_out,
	void *passed,
	int (*wrapped)(void *param))
{
	int		status;
	pid_t	child_pid;

	if ((child_pid = fork()) == -1)
		return (-1);
	if (child_pid == 0)
	{
		if (dup2(STDIN_FILENO, fd_in) == -1 ||
			dup2(STDOUT_FILENO, fd_out) == -1)
			exit(EXIT_FAILURE);
		if ((status = wrapped(passed)) == -1)
			exit(EXIT_FAILURE);
		exit(status);
	}
	wait(&child_pid);
	return (WEXITSTATUS(child_pid));
}

int 		run_builtin(t_eval_state *state, char **argv)
{
	return (builtin_dispatch_run(argv, state->env));
}

/*
** \brief        execve syscall wrapper passed it to fork_wrap
** \param param  function params
** \return       execve return value
*/

int 		execve_wrapper(void *param)
{
	return (execve(
		((t_fork_param_execve*)param)->exec_path,
		((t_fork_param_execve*)param)->argv,
		((t_fork_param_execve*)param)->envp
	));
}

/*
** \brief        Evaluate a command
** \param state  Evaluation state
** \param cmd    Command to evaluate
** \return       Executable status or -1 on error
*/

static int	eval_cmd(int fd_in, int fd_out, t_eval_state *state, t_cmd *cmd)
{
	t_fork_param_execve	param;

	if (builtin_check_exec_name(cmd->argv[0]))
		return (run_builtin(state, cmd->argv));
	param.exec_path = exec_search_path(
			state->path, env_search(state->env, "PATH"), cmd->argv[0]);
	if (param.exec_path == NULL)
		return (-1);
	if (cmd->in != NULL && (fd_in = open(cmd->in, O_RDONLY)) == -1)
		return (-1);
	if (cmd->out != NULL && (fd_out = open(cmd->out,
			(cmd->is_append ? O_APPEND : O_RDONLY) | O_CREAT)) == -1)
		return (-1);
	param.argv = cmd->argv;
	param.envp = (char**)state->env->data;
	return (fork_wrap(fd_in, fd_out, &param, &execve_wrapper));
}

/*
** \brief        Evaluate a line
** \param state  State of the evaluation
** \param line   Line to evaluate
** \return       Last Executed command status or -1 on error
*/
static int	eval_line(void *param)
{
	int				status;
	t_eval_state	*state;
	t_line			*line;
	int fd_in;
	int fd_out;

	state = ((t_fork_param_line*)param)->state;
	line = ((t_fork_param_line*)param)->line;
	fd_in = ((t_fork_param_line*)param)->fd_in;
	fd_out = ((t_fork_param_line*)param)->fd_out;

	/* if (line->right == NULL) */
	/* 	return (eval(state, line->left)); */

	/* if (line->sep == SEP_PIPE) */
	/* 	pipe(state->p); */

	if (line->left->tag == TAG_LINE)
	{
		return (fork_wrap(fd_in, fd_out, param, &eval_line));
	}
	if ((status = eval(fd_in, fd_out, state, line->left)) == -1)
		return (-1);
	if ((line->sep == SEP_AND && status != 0) ||
		(line->sep == SEP_OR && status == 0))
		return (status);

	return (eval(fd_in, fd_out, state, line->right));
}

/*
** \brief        Evaluate an AST
** \param state  State of the evaluation
** \param ast    Abstract syntax tree to evaluate
** \return       Last command status or -1 on error
*/

int			eval(int fd_in, int fd_out, t_eval_state *state, t_ast *ast)
{
	t_fork_param_line param;

	errno = 0;
	if (ast->tag == TAG_LINE)
	{
		param.state = state;
		param.line = &ast->line;
		param.fd_in = fd_in;
		param.fd_out = fd_out;
		return (eval_line(&param));
	}
	if (ast->tag == TAG_CMD)
		return (eval_cmd(fd_in, fd_out, state, &ast->cmd));
	return (-1);
}
