#ifndef MS_EVAL_H
# define MS_EVAL_H

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

read state
 if redir:
 	open file keep fd


typedef struct
{
	t_status_type		type;
	union
	{
		char			*str;
		int				code;
		int				fd;
	}					data;
}						t_val;



typedef enum
{
	STYPE_ERROR,
	STYPE_ARG,
	STYPE_FILE,
	STYPE_SEXPR,
	// ...
}						t_status_type;

int						ms_eval(t_path path, t_env env, t_ast *ast);

#endif
