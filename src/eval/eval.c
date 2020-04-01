/**
** \file   eval.c
** \brief  Evaluation of an AST
*/

#include "eval.h"

static int eval_line(t_eval_state *state, t_line *line)
{
	int	status;

	if (line->right == NULL)
		return (eval(state, line->left));
	status = eval(state, line->left);
	if ((line->sep == SEP_AND && status != 0) ||
		(line->sep == SEP_OR && status == 0))
		return (status);
	return (eval(state, line->right));
}

static bool	is_exec_path(char *path_str)
{
	return (ft_strncmp(path_str, "../", 3) == 0
			|| ft_strncmp(path_str, "./", 2) == 0
			|| ft_strncmp(path_str, "/", 1) == 0);
}

static bool is_valid_exec(char *exec_path)
{
	struct stat	statbuf;

	if (stat(exec_path, &statbuf) != 0)
		return (false);
	if (!S_ISREG(statbuf.st_mode)) // also need to manage link
		return (false);
	// could test permission but probably handled by execve
	return (true);
}

static char	*search_exec_path(t_path path, char *path_var, char *exec_name)
{
	char			*exec_path;

	if (is_exec_path(exec_name))
		return (exec_name);
	// try current first
	if ((exec_path = ft_htget(path, exec_name)) == NULL)
	{
		if (path_update(path, path_var) == NULL)  // optimise by not updating not changed path in ht
			return (NULL);
		if ((exec_path = ft_htget(path, exec_name)) == NULL)
			return (NULL);
	}
	return (exec_path);
}

static int eval_cmd(t_eval_state *state, t_cmd *cmd)
{
	int		child_pid;
	char	*exec_path;

	if ((exec_path = search_exec_path(state->path,
					ft_htget(state->env, "PATH"), cmd->argv[0])) == NULL)
		return (-1);
	if (cmd->in != NULL)
	{
		if ((state->in_pipe[PIPE_WRITE] = open(cmd->in, O_RDONLY)) < 0)
			return (-1);
	}
	if (cmd->out != NULL)
	{
		if ((state->out_pipe[PIPE_READ] = open(cmd->out,
				(cmd->is_append ? O_WRONLY : O_APPEND) | O_CREAT)) < 0)
			return (-1);
	}
	if ((child_pid = fork()) == -1)
		return (-1);
	if (child_pid == 0)
	{
		if (state->in_pipe[PIPE_READ] != PIPE_CLOSED)
			dup2(STDIN_FILENO, state->in_pipe[PIPE_READ]);
		if (state->out_pipe[PIPE_WRITE] != PIPE_CLOSED)
			dup2(STDOUT_FILENO, state->out_pipe[PIPE_WRITE]);
		if (execve(exec_path, cmd->argv, NULL /*env_array*/) == -1)
			exit(EXIT_FAILURE);
	}
	wait(&child_pid);
	return (WEXITSTATUS(child_pid));
}

int	eval(t_eval_state *state, t_ast *ast)
{
	errno = 0;
	if (ast->tag == TAG_LINE)
		return eval_line(state, &ast->data.line);
	if (ast->tag == TAG_CMD)
		return eval_cmd(state, &ast->data.cmd);
	return (-1);
}
