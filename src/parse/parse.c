#include "minishell.h"

t_parsing				*ms_parse(char *input)
{
    char **out_lex;
    if (!(out_lex = ms_lexer(input)))
      return (NULL);

    //(void)input;
	return (NULL);
}
