/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 10:41:31 by charles           #+#    #+#             */
/*   Updated: 2020/09/14 15:50:15 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

pid_t	g_child_pid = -1;
int		g_last_status = 0;

int 		wrapped_cmd(void *void_param)
{
	t_fork_param_cmd	*param;
	int					status;

	param = void_param;
	if (param->builtin != NULL)
		return (param->builtin->func(param->argv, param->env));
	else
	{
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

int			check_exec_path(char *exec_path)
{
	struct stat	statbuf;

	if (stat(exec_path, &statbuf) == -1)
		return (errorf_ret(126, "%s: %s\n", exec_path, strerror(errno)));
	if (S_ISDIR(statbuf.st_mode))
		return (errorf_ret(126, "%s: Is a directory\n", exec_path));
	if (!(statbuf.st_mode & 0444))
		return (errorf_ret(126, "%s: %s\n", exec_path, strerror(EACCES)));
	return (0);
}

int			eval_cmd(int fds[2], t_env env, t_path path, t_ast *ast, pid_t *child_pid)
{
	t_fork_param_cmd	param;
	char				**argv;
	int					status;

	if ((status = redir_extract(&ast->redirs, env, fds)) != 0)
		return (status);
	if ((argv = preprocess(&ast->cmd_argv, env)) == NULL)
		return (EVAL_FATAL);
	if (argv[0] == NULL)
		return (0);
	param.builtin = builtin_search_func(argv[0]);
	if (param.builtin != NULL && !param.builtin->forked && child_pid == NULL)
		return (param.builtin->func(argv, env));

	if (param.builtin == NULL)
	{
		param.exec_path = exec_search_path(path, env_search(env, "PATH"), argv[0]);
		if (param.exec_path == NULL)
		{
			errorf("%s: command not found\n", argv[0]);
			ft_split_destroy(argv);
			return (127);
		}
		if ((status = check_exec_path(param.exec_path)) != 0)
			return (status);
	}

	param.argv = argv;
	param.env = env;
	status = fork_wrap(fds, &param, &wrapped_cmd, child_pid);
	ft_split_destroy(argv);
	g_last_status = status;
	return (status);
}
