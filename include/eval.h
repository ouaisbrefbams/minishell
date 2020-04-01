#ifndef MS_EVAL_H
# define MS_EVAL_H

/**
** \file   eval.h
** \brief  Evaluation module
*/

# include "minishell.h"
# include "ast.h"

/**
** \brief        Evaluation state struct
*/

typedef struct
{
	int			in_pipe[2];   // need stack pipe
	int			out_pipe[2];
	t_path		path;
	t_env		env;
}				t_eval_state;

/**
** \brief      Evaluation status struct
*/

typedef struct
{
	char		*err;
	int			status;
}				t_eval_status;


/**
** \brief        Evaluate an AST
** \param state  State of the evaluation
** \param ast    Abstract syntax tree to evaluate
*/

int				eval(t_eval_state *state, t_ast *ast);

#endif
