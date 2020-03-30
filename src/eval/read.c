/**
** \file   read.c
** \brief  Convert AST to value tree
*/

#include "ms_evalue.h"

static t_value	*read_cmd_args(t_value *value_cmd, t_ast *cmd)
{

}

/* static void		arg_count_iterator(int *counter, t_ast *child) */
/* { */
/* 	if (child == TAG_STRING) */
/* 		(*counter)++; */
/* } */
/*  */
/* static void		arg_add_iterator(t_value *value_cmd, t_ast *child) */
/* { */
/* 	if (child != TAG_STRING) */
/* 		return ; */
/* 	value_cmd_push( */
/* } */

/**
** \brief      Convert a command AST to a value
** \param cmd  Command AST
** \return     Converted value
*/

static t_value	*read_cmd(t_ast *cmd)
{
	int		i;
	int		arg_num;
	t_value	*value_cmd;

	if (ast->children_num < 1 || ast->children[0]->type != TAG_STRING)
		return (value_new_string(VAL_ERR, "Empty command");
	arg_num = 0;
	i = -1;
	while (++i < ast->children_num)
		if (ast->children[i]->type == TAG_STRING)
			arg_num++;
	if ((value_cmd = value_new_cmd(arg_num)) == NULL)
		return (NULL);
	i = -1;
	while (++i < ast->children_num)
		if (ast->children[i]->type == TAG_STRING)
		{
			if ((value_cmd->args[arg_num++] =
					ft_strdup(ast->children[i]->contents)) == NULL)
				return (NULL);
		}
	while (i-- > 0)
		if (ast->children[i]->type != TAG_STRING)
		{
			if (ast->children[i]->tag == TAG_REDIR_IN
					&& value_cmd->in == NULL)
				value_cmd->in = value_new_redir(VAL_REDIR_IN); // check null
			else if (ast->children[i]->tag == TAG_REDIR_OUT
					&& value_cmd->out == NULL))
				value_cmd->out = value_new_redir(VAL_REDIR_OUT);
			else if (ast->children[i]->tag == TAG_REDIR_APPEND
					&& value_cmd->append == NULL))
				value_cmd->append = value_new_redir(VAL_REDIR_APPEND);
		}
	return (value_cmd);
}

/**
** \brief      Convert an AST to value
** \param ast  AST to convert
** \return     Converted value
*/

t_value *eval_read(t_ast *ast)
{
	t_value *value;

	/* if (ast->tag == TAG_REDIR_IN) */
	/* 	return (value_new_redir(VAL_REDIR_IN)); */
	/* else if (ast->tag == TAG_REDIR_OUT) */
	/* 	return (value_new_redir(VAL_REDIR_OUT)); */
	/* else if (ast->tag == TAG_REDIR_APPEND) */
	/* 	return (value_new_redir(VAL_REDIR_APPEND)); */
	else if (ast->tag == TAG_CMD)
		return (read_cmd(ast));
}
