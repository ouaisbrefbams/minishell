/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 10:41:31 by charles           #+#    #+#             */
/*   Updated: 2020/06/14 12:52:59 by charles          ###   ########.fr       */
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
		if (dup2(STDIN_FILENO, fd_in) == -1 ||
			dup2(STDOUT_FILENO, fd_out) == -1)
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



int	eval_cmd(t_env env, t_path path, t_ast *ast)
{
	t_fork_param_cmd	param;

	// check in and out (single string)
	// argv[0]: [string]: ambiguous redirect  code 1

	// check in and out (exist)
	// argv[0]: [string]: No such file or directory (probably from errno)

	return (0);
	/* if (in != NULL) */
	/* { */
	/* 	char **redir_in = preprocess(in); */
	/* 	if (redir_in[1] != NULL) */
	/* 		ambiguous; */
	/* 	if ((fd_in = open(cmd->in, O_RDONLY)) == -1) */
	/* 		file error; */
	/* } */
    /*  */
	/* if (out != NULL) */
	/* { */
	/* 	char **redir_out = preprocess(out); */
	/* 	if (redir_out[1] != NULL) */
	/* 		ambiguous; */
	/* 	if ((fd_out = open(cmd->out, (is_append ? O_APPEND : O_WRONLY) | O_CREAT)) == -1) */
	/* 		file error; */
	/* } */
    /*  */
	/* char **argv = preprocess(cmd_argv); */
    /*  */
	/* param.builtin = builtin_search_func(argv[0]); */
	/* if (param.builtin) */
	/* 	param.exec_path = exec_search_path(path, env_search(env, "PATH"), argv[0]); */
    /*  */
	/* // get cmd path */
	/* // argv[0]: [string]: command not found   code 127 */
	/* if (param.exec_path == NULL) */
	/* 	not_found; */
    /*  */
	/* param.argv = argv; */
	/* param.env = env; */
	/* return (fork_wrap(fd_in, fd_out, &param, &execve_wrapper)); */
}
