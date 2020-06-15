/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 10:41:31 by charles           #+#    #+#             */
/*   Updated: 2020/06/15 11:09:38 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if ((fd_in != MS_NO_FD && dup2(fd_in, STDIN_FILENO) == -1) ||
			(fd_out != MS_NO_FD && dup2(fd_out, STDOUT_FILENO) == -1))
			exit(EXIT_FAILURE);
		if ((status = wrapped(passed)) == -1)
			exit(EXIT_FAILURE);
		exit(status);
	}
	wait(&child_pid);
	return (WEXITSTATUS(child_pid));
}

int 		forked_cmd(void *void_param)
{
	t_fork_param_cmd	*param;

	param = void_param;

	if (param->builtin != NULL)
		return (param->builtin(param->argv, param->env));
	else
		return (execve(param->exec_path, param->argv, (char**)param->env->data));
}

int		eval_cmd(t_env env, t_path path, t_ast *ast)
{
	t_fork_param_cmd	param;
	int					fd_in;
	int					fd_out;
	char				**argv;

	fd_in = MS_NO_FD;
	fd_out = MS_NO_FD;
	if (!redir_extract(ast->redirs, env, &fd_in, &fd_out))
	{
		ast->redirs = NULL;
		return (-1);
	}
	ast->redirs = NULL;

	if ((argv = preprocess(&ast->cmd_argv, env)) == NULL)
	{
		ast->cmd_argv = NULL;
		return (-1);
	}

	param.builtin = builtin_search_func(argv[0]);
	if (param.builtin == NULL)
	{
		param.exec_path = exec_search_path(path, env_search(env, "PATH"), argv[0]);
		if (param.exec_path == NULL)
		{
			error_eval_put(ERROR_CMD_NOT_FOUND, argv[0]);
			ft_split_destroy(argv);
			return (-1); // return error status
		}
	}

	param.argv = argv;
	param.env = env;
	int ret = fork_wrap(fd_in, fd_out, &param, &forked_cmd);
	ft_split_destroy(argv);
	return (ret);
}
