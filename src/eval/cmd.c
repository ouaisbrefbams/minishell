/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 10:41:31 by charles           #+#    #+#             */
/*   Updated: 2020/09/13 14:19:37 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

pid_t	g_child_pid = -1;
int		g_last_status = 0;

void token_debug(void *v);

/*
** \brief          Wrap a function in a fork
** \param fd_in    fork input file descriptor
** \param fd_out   fork output file descriptor
** \param passed   param of the wrapped function
** \param wrapped  function to wrap
*/

int			fork_wrap(
	int fds[2],
	void *passed,
	int (*wrapped)(void *param),
	pid_t *child_pid
)
{
	int		status;
	bool	waiting;
	pid_t	pid;

	waiting = child_pid == NULL;
	if (waiting)
		child_pid = &pid;
	if ((*child_pid = fork()) == -1)
		return (EVAL_FATAL);
	if (*child_pid == 0)
	{
		if ((fds[FD_READ] != FD_NONE && dup2(fds[FD_READ], STDIN_FILENO) == -1) ||
			(fds[FD_WRITE] != FD_NONE && dup2(fds[FD_WRITE], STDOUT_FILENO) == -1))
			exit(EXIT_FAILURE);
		if ((status = wrapped(passed)) == EVAL_FATAL) // FIXME detect fatal in child (pipe ?)
			exit(EXIT_FAILURE);
		exit(status);
	}
	g_child_pid = *child_pid;
	if (waiting)
	{
		waitpid(*child_pid, child_pid, 0);
		close(fds[FD_WRITE]);
		/* close(fds[FD_READ]); */
		return (WEXITSTATUS(*child_pid));
	}
	return (0);
}

int 		forked_cmd(void *void_param)
{
	t_fork_param_cmd	*param;
	int					status;
	struct stat			statbuf;

	param = void_param;
	if (param->builtin != NULL)
		return (param->builtin->func(param->argv, param->env));
	else
	{
		if (stat(param->exec_path, &statbuf) == -1)
			return (errorf_ret(126, "%s: %s\n", param->exec_path, strerror(errno)));
		if (S_ISDIR(statbuf.st_mode))
			return (errorf_ret(126, "%s: Is a directory\n", param->exec_path));
		if (!(statbuf.st_mode & 0444))
			return (errorf_ret(126, "%s: %s\n", param->exec_path, strerror(EACCES)));
		errno = 0;
		status = execve(param->exec_path, param->argv, (char**)param->env->data);
		if (status == -1)
		{
			if (errno == ENOEXEC)
				return (0);
			return (errorf_ret(126, "%s: %s\n", param->exec_path, strerror(errno)));
		}
		return (status);
	}
}

int			eval_cmd(int fds[2], t_env env, t_path path, t_ast *ast, pid_t *child_pid)
{
	t_fork_param_cmd	param;
	char				**argv;
	int					status;

	if ((status = redir_extract(&ast->redirs, env, fds)) != 0)
		return (status);

	if ((argv = preprocess(&ast->cmd_argv, env)) == NULL)
	{
		ast->cmd_argv = NULL;
		return (EVAL_FATAL);
	}

	if (argv[0] == NULL)
		return (0);
	param.builtin = builtin_search_func(argv[0]);
	if (param.builtin == NULL)
	{
		param.exec_path = exec_search_path(path, env_search(env, "PATH"), argv[0]);
		if (param.exec_path == NULL)
		{
			errorf("%s: command not found\n", argv[0]);
			ft_split_destroy(argv);
			return (127);
		}
	}
	else if (!param.builtin->forked && child_pid == NULL)
		return (param.builtin->func(argv, env));

	param.argv = argv;
	param.env = env;
	status = fork_wrap(fds, &param, &forked_cmd, child_pid);
	ft_split_destroy(argv);
	g_last_status = status;
	return (status);
}
