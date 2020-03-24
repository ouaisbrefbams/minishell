#include "minishell.h"

void			*ms_parse(char *input)
{
    char **out_lex;

    if (!(out_lex = ms_lexer(input)))
      ;

    //(void)input;
}
