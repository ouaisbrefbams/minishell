

#include "parser.h"


int                 parse_cmd_str_true_false(enum e_token_tag tag)
{
    if (tag & TAG_STR || tag & TAG_STR_DOUBLE || tag & TAG_STR_SINGLE)
        return (1);
    return(0);
}

t_ast 				*push_cmd(t_ast *ast, t_ftlst *rest)
{
    t_ftlst         *new;

	if (ast == NULL)
    {
        ast = ast_new(AST_CMD);
        ast->cmd_argv = ft_lstnew((t_token *)rest->data);
    }
    else
    {
        new = ft_lstnew((t_token *)rest->data);
        ft_lstpush_back(&ast->cmd_argv, new);
    }
    return (ast);
}
