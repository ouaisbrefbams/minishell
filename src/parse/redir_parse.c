
#include "parser.h"

int                 parse_redir_true_false(enum e_tok tag)
{
    if (tag & TAG_IS_REDIR)
        return (1);
    return (0);
}

t_ast 				*push_redir(t_ast *ast, t_ftlst *rest)
{
    t_ftlst         *new;

	if (ast == NULL)
    {
        ast = ast_new(AST_CMD);
        ast->redirs = ft_lstnew((t_token *)rest->data);
    }
    else
    {
        new = ft_lstnew((t_token *)rest->data);
        ft_lstpush_back(&ast->redirs, new);
    }
    return (ast);
}
