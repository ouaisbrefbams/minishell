/**
** \file   val.c
** \brief  Evaluation value manipulation
*/

#include "ms_evalue.h"

/**
** \brief       Allocate memory for a t_value and set his type
** \param type  Type of valueue
** \return      The allocated value
*/

t_value	*value_new(t_value_type type)
{
	t_value	*value;

	if ((value = (t_value*)malloc(sizeof(t_value*))) == NULL)
		return (NULL);
	value->type = type;
	return (value);
}

/**
** \brief           Create a new redirection value from a filename
** \param type      Type of redirection and value
** \param filename  Name of the file to open
** \return          Redirection value
** \warning         Undefined behavior on none redirection type.
*/

t_value	*value_new_redir(t_value_type type, char *filename)
{
	t_value	*value;

	if ((value = value_new(type)) == NULL)
		return (NULL);
	if (type == VAL_REDIR_IN)
		value->data.fd = open(filename, O_RDONLY);
	else if (type == VAL_REDIR_OUT)
		value->data.fd = open(filename, O_WRONLY | O_CREAT);
	else if (type == VAL_REDIR_APPEND)
		value->data.fd = open(filename, O_APPEND | O_CREAT);
	if (value->data.fd < 0)
	{
		free(value);
		return (NULL);
	}
	return (value);
}

/**
** \brief           Create a new string value (i.e error, arg, cmd)
** \param type      String value type
** \param str       String data
** \return          String value
*/

t_value	*value_new_string(t_value_type type, char *str)
{
	t_value *value;

	if ((value == malloc(sizeof(t_value))) == NULL)
		return (NULL);
	value->type = type;
	if ((value->data.str = ft_strdup(str)) == NULL)
		return (NULL);
	return (value);
}
