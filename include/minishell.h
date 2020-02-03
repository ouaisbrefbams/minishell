#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "libft_ht.h"


typedef struct
{
	int		argc;
	char	**argv;
}	t_command;

// void find_exe(char *name);

typedef int t_status;

/*
** 
*/

/*
** builtin*.c
*/

typedef t_status	(*t_builtin_func)(int argc, char **argv, char **envp);
t_builtin_func ms_echo;
t_builtin_func ms_cd;
t_builtin_func ms_pwd;
t_builtin_func ms_export;
t_builtin_func ms_unset;
t_builtin_func ms_env;
t_builtin_func ms_exit;

#endif
