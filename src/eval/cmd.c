/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 10:41:31 by charles           #+#    #+#             */
/*   Updated: 2020/09/14 19:50:55 by charles          ###   ########.fr       */
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

int			eval_cmd(int fds[2], t_env env, t_ast *ast, pid_t *child_pid)
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

		if (!path_search(env, argv[0], param.exec_path))
		{
			errorf("%s: command not found\n", argv[0]);
			ft_split_destroy(argv);
			return (127);
		}
		if ((status = exec_path_check(param.exec_path)) != 0)
			return (status);
	}

	param.argv = argv;
	param.env = env;
	status = fork_wrap(fds, &param, &wrapped_cmd, child_pid);
	ft_split_destroy(argv);
	g_last_status = status;
	return (status);
}
