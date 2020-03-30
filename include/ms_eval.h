#ifndef MS_EVAL_H
# define MS_EVAL_H

/**
** \file   ms_eval.h
** \brief  Evaluation module
*/

/*
** arg:
**   type ARG
**   value string
**
** redir in:
**   type REDIRIN
**   value fd
**
** redir out:
**   type REDIROUT
**   value fd
**
** redir append:
**   type REDIRAPPEND
**   value fd
*/

/**
** \enum t_val_type
** \brief  A type for an evaluation node
**
** \param VAL_ERR error
** \param VAL_ERR argument
** \param VAL_REDIR redirection
** \param VAL_SEXPR S-expression
*/

typedef enum
{
	VAL_ERR,
	VAL_ARG,
	VAL_EXEC,
	VAL_REDIR_IN,
	VAL_REDIR_OUT,
	VAL_REDIR_APPEND,
	VAL_CMD,
	VAL_SEXPR,
}				t_val_type;

/**
** \brief            An evaluation node struct
** \param type       type of node
** \param data       union of possible data
** \param data::fd   file descriptor for redirection node
** \param data::str  string for error, arguments, command
*/

typedef struct
{
	t_val_type	type;
	union
	{
		char	*str;
		int		code;
		int		fd;
	}			data;
}				t_val;

/**
** \brief  evaluate an AST
** \param path  path to commands executable
** \param env   environment variables
** \param ast   Abstract syntax tree to evaluate
*/
int				ms_eval(t_path path, t_env env, t_ast *ast);

#endif
