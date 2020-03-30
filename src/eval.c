#include "minishell.h"

t_val *val_new_redir(t_val_type type, char *filename)
{
	t_val *val;

	if ((val = val_new(type)) == NULL)
		return (NULL);
	if (type == VAL_REDIR_IN)
		val->data.fd = open(filename, O_RDONLY);
	else if (type == VAL_REDIR_OUT)
		val->data.fd = open(filename, O_WRONLY);
	else if (type == VAL_REDIR_APPEND)
		val->data.fd = open(filename, O_RDWR);
	if (val->data.fd < 0)
	{
		free(val);
		return (NULL);
	}
	return (val);
}

t_val *val_new_err(char *msg)
{
	t_val *val;

	if ((val == malloc(sizeof(t_val))) == NULL)
		return (NULL);
	val->type = VAL_ERR;
	if ((val->data.str = ft_strdup(msg)) == NULL)
		return (NULL);
	return (val);
}

t_val *read_ast(t_ast *ast)
{
	t_val *val;

	if (ast->tag == TAG_REDIR_IN)
	{
		val = val_new();
		val->type = VAL_REDIR_IN;
		val->value.fd = open(ast->content, O_RDONLY);
	}
}

static bool check_node(t_ast *ast)
{

}

static int eval_sep(t_ast *ast)
{
	if (ast->children_num != 2)
		return (-1);
	if (ast->tag == TAG_ENDCMD)
	{
		ms_eval(path, env, ast->children[0]);
		return (ms_eval(path, env, ast->children[1]));
	}
	if (ast->tag == TAG_AND)
	{
		status = ms_eval(path, env, ast->children[0]);
		if (status == 0)
			return (ms_eval(path, env, ast->children[1]));
		else
			return (status);
	}
	if (ast->tag == TAG_OR)
	{
		status = ms_eval(path, env, ast->children[0]);
		if (status != 0)
			return (ms_eval(path, env, ast->children[1]));
		else
			return (status);
	}
	return (-1);
}

static char	**get_args(t_ast *ast)
{
	int		i;
	int		counter;
	char	**argv;

	if (ast->tag != TAG_CMD)
		return (NULL);
	counter = 0;
	i = -1;
	while (++i < ast->children_num)
		if (ast->children[i]->tag == TAG_ARG)
			counter++;
	if ((argv = (char**)ft_calloc(counter + 1, sizeof(char*))) == NULL)
		return (NULL);
	counter = 0;
	i = -1;
	while (++i < ast->children_num)
	{
		if (ast->children[i]->tag != TAG_ARG)
			continue ;
		if ((argv[counter] = ft_strdup(ast->children[i]->content)) == NULL)
		{
			ft_split_destroy(argv);
			return (NULL);
		}
	}
	return (argv);

	/* maybe
	int		i;
	char	*tmp;
	t_vec	*vec;

	if ((vec = ft_vecnew(sizeof(char*))) == NULL)
		return (NULL);
	i = -1;
	while (++i < ast->children_num)
	{
		if (ast->children[i]->tag != TAG_ARG)
			continue ;
		if ((tmp = ft_strdup(ast->children[i]->content)) == NULL
			|| ft_vecpush(vec, tmp) == NULL))
		{
			ft_vecdestroy(vec, free);
			return (NULL);
		}
	}
	return (vec);
	*/
}

int	ms_eval(t_path path, t_env env, t_ast *ast)
{
	int	status;

	if (ast->tag == TAG_ENDCMD || ast->tag == TAG_AND
			|| ast->tag == TAG_OR || ast->tag == TAG_PIPE)
		return (eval_sep(ast));

}
