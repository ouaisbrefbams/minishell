/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 10:41:31 by charles           #+#    #+#             */
/*   Updated: 2020/09/10 20:32:13 by charles          ###   ########.fr       */
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
				int (*wrapped)(void *param))
{
	int		status;
	pid_t	child_pid;

	if ((child_pid = fork()) == -1)
		return (EVAL_FATAL);
	if (child_pid == 0)
	{
		if ((fds[FD_READ] != FD_NONE && dup2(fds[FD_READ], STDIN_FILENO) == -1) ||
			(fds[FD_WRITE] != FD_NONE && dup2(fds[FD_WRITE], STDOUT_FILENO) == -1))
			exit(EXIT_FAILURE);
		if ((status = wrapped(passed)) == EVAL_FATAL) // FIXME detect fatal in child (pipe ?)
			exit(EXIT_FAILURE);
		exit(status);
	}
	g_child_pid = child_pid;
	wait(&child_pid);
	close(fds[FD_WRITE]);
	return (WEXITSTATUS(child_pid));
}

int 		forked_cmd(void *void_param)
{
	t_fork_param_cmd	*param;
	int					status;
	struct stat			statbuf;

	param = void_param;
	ft_vecpop(param->env_local, NULL);
	if (ft_vecswallow_at(param->env, param->env->size - 1, param->env_local) == NULL)
	{
		ft_vecdestroy(param->env_local, free);
		return (EVAL_FATAL);
	}
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

int			eval_cmd(int fds[2], t_env env, t_path path, t_ast *ast)
{
	t_fork_param_cmd	param;
	char				**argv;
	int					status;

	if ((status = redir_extract(&ast->redirs, env, fds)) != 0)
		return (status);
	if ((param.env_local = env_from_array((char*[]){NULL})) == NULL)
		return (EVAL_FATAL);
	if (!variable_extract(&ast->cmd_argv, env, param.env_local))
		return (EVAL_FATAL);

	/* char **strs = preprocess(&start, env); */
    /*  */
	/* if (env_export(env_local, id, strs[0]) == NULL) */
	/* 	return (-1); */
	/* if (ast->cmd_argv == NULL) // FIXME special env not passed to child processes */
	/* { */
	/* 	ft_vecpop(param.env_local, NULL); */
	/* 	if (ft_vecswallow_at(env, env->size - 1, param.env_local) == NULL) */
	/* 	{ */
	/* 		ft_vecdestroy(param.env_local, free); */
	/* 		return (-1); */
	/* 	} */
	/* 	g_last_status = 0; */
	/* 	return (0); */
	/* } */

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
		// check env local for PATH
		param.exec_path = exec_search_path(path, env_search(env, "PATH"), argv[0]);
		if (param.exec_path == NULL)
		{
			errorf("%s: command not found\n", argv[0]);
			ft_split_destroy(argv);
			return (127);
		}
	}
	else if (!param.builtin->forked)
		return (param.builtin->func(argv, env));

	param.argv = argv;
	param.env = env;
	status = fork_wrap(fds, &param, &forked_cmd);
	ft_split_destroy(argv);
	ft_vecdestroy(param.env_local, free);
	g_last_status = status;
	return (status);
}
