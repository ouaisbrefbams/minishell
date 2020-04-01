/**
** \file   eval.c
** \brief  Evaluation of an AST
*/

#include "eval.h"

/**
** \brief        Evaluate a line
** \param state  State of the evaluation
** \param line   Line to evaluate
** \return       Last Executed command status or -1 on error
*/

static int eval_line(t_eval_state *state, t_line *line)
{
	int	status;

	if (line->right == NULL)
		return (eval(state, line->left));
	if ((status = eval(state, line->left)) == -1)
		return (-1);
	if ((line->sep == SEP_AND && status != 0) ||
		(line->sep == SEP_OR && status == 0))
		return (status);
	return (eval(state, line->right));
}

/**
** \brief        Evaluate a command
** \param state  Evaluation state
** \param cmd    Command to evaluate
** \return       Executable status or -1 on error
*/

static int eval_cmd(t_eval_state *state, t_cmd *cmd)
{
	int		child_pid;
	char	*exec_path;
	bool	is_builtin;

	is_builtin = builtin_check_exec_name(cmd->argv[0]);
	if (!is_builtin)
	{
		if ((exec_path = exec_search_path(state->path,
						ft_htget(state->env, "PATH"), cmd->argv[0])) == NULL)
			return (-1);
	}
	pipe_setup_parent(cmd, state->pipe_in, state->pipe_out);
	if ((child_pid = fork()) == -1)
		return (-1);
	if (child_pid == 0)
	{
		pipe_setup_child(state->pipe_in, state->pipe_out);
		if (is_builtin)
			exit(builtin_dispatch_run(cmd->argv, state->env));
		else if (execve(exec_path, cmd->argv, NULL /*env_array*/) == -1)
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	wait(&child_pid);
	return (WEXITSTATUS(child_pid));
}

/**
** \brief        Evaluate an AST
** \param state  State of the evaluation
** \param ast    Abstract syntax tree to evaluate
** \return       Executable status or -1 on error
*/

int	eval(t_eval_state *state, t_ast *ast)
{
	errno = 0;
	if (ast->tag == TAG_LINE)
		return eval_line(state, &ast->data.line);
	if (ast->tag == TAG_CMD)
		return eval_cmd(state, &ast->data.cmd);
	return (-1);
}
