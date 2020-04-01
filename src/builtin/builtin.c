/**
** \file      builtin.c
** \brief     Builtin functions
*/

#include "minishell.h"

/**
** \brief      Array storing builtin executable name and associated functions
*/

static struct s_builtin_entry g_builtin_lookup[] = {
	{"echo", builtin_echo},
	{"cd", builtin_cd},
	{"pwd", builtin_pwd},
	{"export", builtin_export},
	{"unset", builtin_unset},
	{"env", builtin_env},
	{"exit", builtin_exit},
};

/**
** \brief       Call builtin function associated with command name
** \param argv  Arguments to the builtin 'main', with argv[0] being the executable name
** \param env   Environment Vector
** \return      Builtin main return status
*/

int		builtin_dispatch_run(char **argv, t_env env)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_builtin_lookup) / sizeof(struct s_builtin_entry))
	{
		if (ft_strcmp(g_builtin_lookup[i].name, argv[0]) == 0)
			return (g_builtin_lookup[i].func(argv, env));
		i++;
	}
	return (BUILTIN_NOT_FOUND);
}

/**
** \brief            Check if executable name is a builtin
** \param exec_name  Executable name
** \return           True if executable name is a builtin
*/

bool		builtin_check_exec_name(char *exec_name)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_builtin_lookup) / sizeof(struct s_builtin_entry))
	{
		if (ft_strcmp(g_builtin_lookup[i].name, exec_name) == 0)
			return (true);
		i++;
	}
	return (false);
}
