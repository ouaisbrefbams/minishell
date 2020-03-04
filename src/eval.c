
#include "minishell.h"

/* static int				st_frame_init(t_command_frame *frame) */
/* { */
/* 	#<{(| if (pipe(frame->pipe_in) == -1) |)}># */
/* 	#<{(| 	return (-1); |)}># */
/* 	#<{(| if (pipe(frame->pipe_out) == -1) |)}># */
/* 	#<{(| 	return (-1); |)}># */
/* 	frame->pipe_in[MS_PIPE_READ] = -1; */
/* 	frame->pipe_in[MS_PIPE_WRITE] = -1; */
/* 	frame->pipe_out[MS_PIPE_READ] = -1; */
/* 	frame->pipe_out[MS_PIPE_WRITE] = -1; */
/* 	return (0); */
/* } */
/*  */
/* #<{(| */
/* ** Run child process with executable located by `path`, his arguments in `argv` */
/* ** and the current environment variables in `envp`. */
/* ** Returns the child process pid or -1 if an error occured when forking. */
/* |)}># */
/*  */
/* static pid_t			st_run_process(char *path, char **argv, char **envp, t_command_frame *frame) */
/* { */
/* 	int		i; */
/* 	pid_t	child_pid; */
/*  */
/* 	if (path == NULL || argv == NULL || envp == NULL || frame == NULL) */
/* 		return (-1); */
/* 	if ((child_pid = fork()) == -1) */
/* 		return (-1); */
/* 	if (child_pid == 0) */
/* 	{ */
/* 		if (frame->pipe_in[MS_PIPE_READ] != -1) */
/* 			dup2(STDIN_FILENO, frame->pipe_in[MS_PIPE_WRITE]); */
/* 		if (frame->pipe_out[MS_PIPE_WRITE] != -1) */
/* 			dup2(STDOUT_FILENO, frame->pipe_out[MS_PIPE_READ]); */
/* 		if ((execve(path, argv, envp)) == -1) */
/* 			exit(EXIT_FAILURE); */
/* 	} */
/* 	else */
/* 		return (child_pid); */
/* } */
/*  */
/* #<{(| */
/* ** Run a command. */
/* |)}># */
/*  */
/* static int				st_command_run(t_command *command, t_command_frame *frame) */
/* { */
/* 	t_redirection	*tmp; */
/*  */
/* 	if ((tmp = st_has_redirection_in(command)) != NULL) */
/* 	{ */
/* 		if (pipe(frame->pipe_in) == -1) */
/* 			return (-1); */
/* 		if ((frame->pipe_in[MS_PIPE_READ] = open(tmp->filename, O_RDONLY)) < 0) */
/* 			return (-1); */
/* 	} */
/* 	if ((tmp = st_has_redirection_out(command)) != NULL) */
/* 	{ */
/* 		if (pipe(frame->pipe_out) == -1) */
/* 			return (-1); */
/* 		if ((frame->pipe_out[MS_PIPE_WRITE] = open(tmp->filename, O_WRONLY | O_CREAT)) < 0) */
/* 			return (-1); */
/* 	} */
/* 	if ((child_pid = st_run_process(path, argv, envp, &frame)) == -1) */
/* 		return (-1); */
/* 	wait(&child_pid); */
/* 	return (0); */
/* } */
/*  */
/* #<{(| */
/* ** Run a standalone command. */
/* |)}># */
/*  */
/* static int				st_command_standalone_run(t_command *command) */
/* { */
/* 	t_command_frame	frame; */
/*  */
/* 	if (st_frame_init(&frame) == -1) */
/* 		return (-1); */
/* 	return (st_command_run(command, &frame)); */
/* } */
/*  */
/* #<{(| */
/* ** Run a piped command */
/* |)}># */
/*  */
/* // in main loop, if previous was piped, init frame with previous pipe */
/* static int				st_command_piped_run(t_command *command, int pipe_out[2]) */
/* { */
/* 	t_command_frame	frame; */
/*  */
/* 	if (st_frame_init(&frame) == -1) */
/* 		return (-1); */
/* 	frame.pipe_out[MS_PIPE_WRITE] =  */
/*  */
/* } */
/*  */
/*  */
/* #<{(| */
/* ** Evaluate the commands in parsing according by what they are separated by. */
/* |)}># */
/*  */
/* int						ms_eval(t_parsing *parsing) */
/* { */
/* 	pid_t			child_pid; */
/* 	t_command_frame	frame; */
/*  */
/* 	if (parsing == NULL) */
/* 		return (-1); */
/* 	if (ft_lstsize(parsing->commands) != ft_lstsize(parsing->separators) - 1) */
/* 		return (-1); */
/* 	while (parsing->separators != NULL) */
/* 	{ */
/* 		if ((t_separator)parsing->separators->content == SEPARATOR_SEMICOLON) */
/* 		{ */
/* 		} */
/* 		else if ((t_separator)parsing->separators->content == SEPARATOR_PIPE) */
/* 		{ */
/*  */
/* 			// if has out redirection */
/* 			//      */
/* 			 */
/* 			// create pipe  */
/* 			// fork fst */
/* 			//    redirect stdout to pipe in */
/* 			//    execve */
/*  */
/* 			// fork snd */
/* 			//    redirect std to pipe out */
/* 			//    execve */
/*  */
/* 		} */
/* 		ft_lstpop_front(&parsing->commands, free); */
/* 		ft_lstpop_front(&parsing->separators, NULL); */
/* 	} */
/* 	return (0); */
/* } */
