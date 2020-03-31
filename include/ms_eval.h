#ifndef MS_EVAL_H
# define MS_EVAL_H

/**
** \file   ms_eval.h
** \brief  Evaluation module
*/

# include "minishell.h"

/**
** \brief        Evaluation state struct
*/

typedef struct
{
	int			status;
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

int				ms_eval(t_eval_state *state, t_ast *ast);

#endif
