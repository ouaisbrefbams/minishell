/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/01 17:05:21 by charles           #+#    #+#             */
/*   Updated: 2020/04/02 17:08:18 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** \file   eval.c
** \brief  Evaluation of an AST
*/

#include "eval.h"

#define PARAM_SIZE 3

#define PARAM_STATE 0
#define PARAM_LINE 1

#define PARAM_EXEC_PATH 0
#define PARAM_ARGV 1
#define PARAM_ENVP 2

int			io_frame_init(t_io_frame *frame)
{
	if (pipe(frame->pipe_in) == -1
		|| pipe(frame->pipe_out) == -1)
		return (-1);
	/* frame->pipe_in[PIPE_READ] = STDIN_FILENO; */
	/* frame->pipe_in[PIPE_WRITE] = PIPE_CLOSED; */
	/* frame->pipe_out[PIPE_READ] = PIPE_CLOSED; */
	/* frame->pipe_out[PIPE_WRITE] = STDOUT_FILENO; */
	return (0);
}

static int	eval_root(void *params[PARAM_SIZE])
{
	int				status;
	t_eval_state	*state;
	t_line			*line;
	t_io_frame		frame_left;
	t_io_frame		frame_right;

	state = params[PARAM_STATE];
	line = params[PARAM_LINE];
	io_frame_init(&frame_left);
	if (line->right == NULL)
		return (eval(&frame_left, state, line->left));

	if (line->sep == SEP_PIPE)
		dup2(STDOUT_FILENO, frame_left.pipe_out[PIPE_WRITE]);

	if ((status = eval(&frame_left, state, line->left)) == -1)
		return (-1);
	if ((line->sep == SEP_AND && status != 0) ||
		(line->sep == SEP_OR && status == 0))
		return (status);

	if (line->sep == SEP_PIPE)
		dup2(frame_right.pipe_in[PIPE_WRITE], frame_left.pipe_out[PIPE_READ]);

	return (eval(&frame_right, state, line->right));
}

int execve_wrapper(void *params[PARAM_SIZE])
{
	return (execve(
		params[PARAM_EXEC_PATH],
		params[PARAM_ARGV],
		params[PARAM_ENVP]
	));
}

int exec_wrap(
	t_io_frame *frame,
	void *passed[PARAM_SIZE],
	int (*wrapped)(void *params[PARAM_SIZE])
)
{
	int		status;
	pid_t	child_pid;

	if ((child_pid = fork()) == -1)
		return (-1);
	if (child_pid == 0)
	{
		if (dup2(STDIN_FILENO, frame->pipe_in[PIPE_READ]) == -1 ||
			dup2(STDOUT_FILENO, frame->pipe_out[PIPE_WRITE]) == -1)
			exit(EXIT_FAILURE);
		if ((status = wrapped(passed)) == -1)
			exit(EXIT_FAILURE);
		exit(status);
	}
	wait(&child_pid);
	return (WEXITSTATUS(child_pid));
}

/*
** \brief        Evaluate a line
** \param state  State of the evaluation
** \param line   Line to evaluate
** \return       Last Executed command status or -1 on error
*/

static int	eval_line(t_io_frame *frame, t_eval_state *state, t_line *line)
{
	void	*params[PARAM_SIZE];

	params[PARAM_STATE] = state;
	params[PARAM_LINE] = line;
	return (exec_wrap(frame, params, &eval_root));
}

int run_builtin(t_eval_state *state, char **argv)
{
	return (builtin_dispatch_run(argv, state->env));
}

/*
** \brief        Evaluate a command
** \param state  Evaluation state
** \param cmd    Command to evaluate
** \return       Executable status or -1 on error
*/

static int	eval_cmd(t_io_frame *frame, t_eval_state *state, t_cmd *cmd)
{
	void	*params[PARAM_SIZE];

	if (builtin_check_exec_name(cmd->argv[0]))
		return (run_builtin(state, cmd->argv));
	params[PARAM_EXEC_PATH] = exec_search_path(
			state->path, env_search(state->env, "PATH"), cmd->argv[0]);
	if (params[PARAM_EXEC_PATH] == NULL)
		return (-1);
	if (cmd->in != NULL)
		if ((frame->pipe_in[PIPE_WRITE] = open(cmd->in, O_RDONLY)) == -1)
			return (-1);
	if (cmd->out != NULL)
		if ((frame->pipe_out[PIPE_READ] = open(cmd->out,
				(cmd->is_append ? O_APPEND : O_RDONLY) | O_CREAT)) == -1)
			return (-1);
	params[PARAM_ARGV] = cmd->argv;
	params[PARAM_ENVP] = state->env->data;
	return (exec_wrap(frame, params, &execve_wrapper));
}

/*
** \brief        Evaluate an AST
** \param state  State of the evaluation
** \param ast    Abstract syntax tree to evaluate
** \return       Executable status or -1 on error
*/

int			eval(t_io_frame *frame, t_eval_state *state, t_ast *ast)
{
	void	*params[PARAM_SIZE];

	errno = 0;
	if (ast->tag == TAG_ROOT)
	{
		params[PARAM_STATE] = state;
		params[PARAM_LINE] = &ast->data.line;
		return (eval_root(params));
	}
	if (ast->tag == TAG_LINE)
		return (eval_line(frame, state, &ast->data.line));
	if (ast->tag == TAG_CMD)
		return (eval_cmd(frame, state, &ast->data.cmd));
	return (-1);
}
