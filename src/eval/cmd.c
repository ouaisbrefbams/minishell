/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 10:41:31 by charles           #+#    #+#             */
/*   Updated: 2020/09/15 17:51:05 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

pid_t	g_child_pid = -1;
int		g_last_status = 0;

int			wrapped_cmd(t_fork_param_cmd *param)
{
	int	status;

	if (param->builtin != NULL)
		return (param->builtin->func(param->argv, param->env));
	status = execve(param->exec_path, param->argv, (char**)param->env->data);
	if (status == -1)
	{
		if (errno == ENOEXEC)
			return (0);
		return (errorf_ret(126, "%s: %s\n", param->exec_path, strerror(errno)));
	}
	return (status);
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

	if (param.builtin == NULL
		&& (status = path_search(env, argv[0], param.exec_path, true)) != 0)
	{
		ft_split_destroy(argv);
		return (status);
	}

	param.argv = argv;
	param.env = env;
	status = fork_wrap(fds, &param, (t_wrapped_func)wrapped_cmd, child_pid);
	ft_split_destroy(argv);
	g_last_status = status;
	return (status);
}
