/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 10:41:31 by charles           #+#    #+#             */
/*   Updated: 2020/07/19 19:00:54 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

pid_t	g_child_pid = -1;
int		g_last_status_code = 0;

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
				int (*wrapped)(void *param))
{
	int		status;
	pid_t	child_pid;

	if ((child_pid = fork()) == -1)
		return (-1);
	if (child_pid == 0)
	{
		if ((fds[FDS_READ] != MS_NO_FD && dup2(fds[FDS_READ], STDIN_FILENO) == -1) ||
			(fds[FDS_WRITE] != MS_NO_FD && dup2(fds[FDS_WRITE], STDOUT_FILENO) == -1))
			exit(EXIT_FAILURE);
		if ((status = wrapped(passed)) == -1)
			exit(EXIT_FAILURE);
		exit(status);
	}
	g_child_pid = child_pid;
	wait(&child_pid);
	close(fds[FDS_WRITE]);
	// also read end?
	return (WEXITSTATUS(child_pid));
}

int 		forked_cmd(void *void_param)
{
	t_fork_param_cmd	*param;
	int					ret;

	param = void_param;
	ft_vecpop(param->env_local, NULL);
	if (ft_vecswallow_at(param->env, param->env->size - 1, param->env_local) == NULL)
	{
		ft_vecdestroy(param->env_local, free);
		return (-1);
	}
	if (param->builtin != NULL)
		return (param->builtin->func(param->argv, param->env));
	else
	{
		errno = 0;
		ret = execve(param->exec_path, param->argv, (char**)param->env->data);
		if (ret == -1)
		{
			if (errno == ENOEXEC)
				return (0);
			struct stat statbuf;
			if (stat(param->exec_path, &statbuf) != -1 && S_ISDIR(statbuf.st_mode))
			{
				errorf("%s: Is a directory\n", param->exec_path);
				ret = 126;
			}
			else
			{
				errorf("%s: %s\n", param->exec_path, strerror(errno));
				ret = 126;
			}
		}
		return (ret);
	}
}

int			eval_cmd(int fds[2], t_env env, t_path path, t_ast *ast)
{
	t_fork_param_cmd	param;
	char				**argv;

	if (!redir_extract(ast->redirs, env, fds))
	{
		ast->redirs = NULL;
		return (-1);
	}
	ast->redirs = NULL;
	if ((param.env_local = env_from_array((char*[]){NULL})) == NULL)
		return (-1);
	// TODO generate token list after `=` for variable value preprocessing
	while (ast->cmd_argv != NULL
		&& ((t_token*)ast->cmd_argv->data)->tag & TAG_IS_STR
		&& utils_start_with_valid_identifier(((t_token*)ast->cmd_argv->data)->content))
	{
		if (env_export_full(param.env_local, ((t_token*)ast->cmd_argv->data)->content) == NULL)
			return (-1);
		ft_lstpop_front(&ast->cmd_argv, (void (*)(void*))token_destroy);
	}
	if (ast->cmd_argv == NULL) // FIXME special env not passed to child processes
	{
		ft_vecpop(param.env_local, NULL);
		if (ft_vecswallow_at(env, env->size - 1, param.env_local) == NULL)
		{
			ft_vecdestroy(param.env_local, free);
			return (-1);
		}
		g_last_status_code = 0;
		return (0);
	}

	if ((argv = preprocess(&ast->cmd_argv, env)) == NULL)
	{
		ast->cmd_argv = NULL;
		return (-1);
	}

	// can have no command (e.g `< file`)
	if (argv[0] == NULL)
		return (0);
	param.builtin = builtin_search_func(argv[0]);
	if (param.builtin == NULL)
	{
		// check env local for PATH
		param.exec_path = exec_search_path(path, env_search(env, "PATH"), argv[0]);
		if (param.exec_path == NULL)
		{
			g_last_status_code = 127;
			errorf("%s: command not found\n", argv[0]);
			ft_split_destroy(argv);
			return (-1); // return error status
		}
	}
	else if (!param.builtin->forked)
	{
		g_last_status_code = param.builtin->func(argv, env);
		return (g_last_status_code);
	}

	param.argv = argv;
	param.env = env;
	int ret = fork_wrap(fds, &param, &forked_cmd);
	g_last_status_code = ret;
	ft_split_destroy(argv);
	ft_vecdestroy(param.env_local, free);
	return (ret);
}
