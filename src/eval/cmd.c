/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 10:41:31 by charles           #+#    #+#             */
/*   Updated: 2020/06/14 21:23:20 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

#define MS_NO_FD -2

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
		if (dup2(fd_in, STDIN_FILENO) == -1 ||
			dup2(fd_out, STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
		if ((status = wrapped(passed)) == -1)
			exit(EXIT_FAILURE);
		exit(status);
	}
	wait(&child_pid);
	return (WEXITSTATUS(child_pid));
}

/*
** \brief        execve syscall wrapper passed it to fork_wrap
** \param param  function params
** \return       execve return value
*/

int 		forked_cmd(void *void_param)
{
	t_fork_param_cmd	*param;

	param = void_param;

	if (param->builtin != NULL)
		return (param->builtin(param->argv, param->env));
	else
		return (execve(param->exec_path, param->argv, (char**)param->env->data));
}

int	open_redirection(t_ftlst *filename_tokens, t_env env, int oflag)
{
	// check in and out (single string)
	// argv[0]: [string]: ambiguous redirect  code 1

	// check in and out (exist)
	// argv[0]: [string]: No such file or directory (probably from errno)

	char	*filename;
	int		fd;

	if (filename_tokens == NULL)
		return (MS_NO_FD);
	if ((filename = preprocess_filename(filename_tokens, env)) == NULL)
		return (-1);
	if ((fd = open(filename, oflag)) == -1)
		return (-1); //file error;
	return (fd);
}

bool	redir_has_tag(t_ftlst *redir, enum e_token_tag tags)
{
	return (((t_token*)redir->data)->tag & tags);
}

bool	redir_extract(t_ftlst *redirs, t_env env, int *fd_in, int *fd_out)
{
	t_ftlst	*after;
	t_ftlst	*curr;
	char	*filename;

	if (redirs == NULL)
		return (true);
	if (!redir_has_tag(redirs, TAG_REDIR_IN | TAG_REDIR_OUT | TAG_REDIR_APPEND)
		|| redirs->next == NULL
		|| !redir_has_tag(redirs->next, TAG_STR | TAG_STR_SINGLE | TAG_STR_DOUBLE))
		return (false);
	curr = redirs->next;
	while (curr != NULL && redir_has_tag(curr, TAG_STR | TAG_STR_SINGLE | TAG_STR_DOUBLE))
	{
		if (curr->next == NULL || redir_has_tag(curr->next, TAG_REDIR_IN | TAG_REDIR_OUT | TAG_REDIR_APPEND))
		{
			after = curr->next;
			curr->next = NULL;
		}
		curr = curr->next;
	}
	filename = preprocess_filename(redirs->next, env);
	if (redir_has_tag(redirs, TAG_REDIR_IN))
	{
		if (*fd_in != STDIN_FILENO)
			close(*fd_in);
		if ((*fd_in = open(filename, O_RDONLY)) == -1)
			return (false);
	}
	else if (redir_has_tag(redirs, TAG_REDIR_OUT | TAG_REDIR_APPEND))
	{
		if (*fd_out != STDOUT_FILENO)
			close(*fd_out);
		if ((*fd_out = open(filename,
					(redir_has_tag(redirs, TAG_REDIR_APPEND) ? O_APPEND : O_WRONLY)
					| O_CREAT | O_TRUNC, 0644)) == -1)
			return (false);
	}
	return (redir_extract(after, env, fd_in, fd_out));
}

int	eval_cmd(t_env env, t_path path, t_ast *ast)
{
	t_fork_param_cmd	param;
	int					fd_in;
	int					fd_out;
	char				**argv;

	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	if (!redir_extract(ast->redirs, env, &fd_in, &fd_out))
		return (-1);

	argv = preprocess(ast->cmd_argv, env);

	param.builtin = builtin_search_func(argv[0]);
	if (param.builtin == NULL)
	{
		param.exec_path = exec_search_path(path, env_search(env, "PATH"), argv[0]);
		/* // get cmd path */
		/* // argv[0]: [string]: command not found   code 127 */
		if (param.exec_path == NULL)
			return (-1); //not_found;
	}

	param.argv = argv;
	param.env = env;
	return (fork_wrap(fd_in, fd_out, &param, &forked_cmd));
}
