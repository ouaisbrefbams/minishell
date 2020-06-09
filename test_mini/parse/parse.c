
#include "parse.h"
#include <stdio.h>


t_return         *parse(t_return *nw, char **input)
{
    int i;

    i = 0;
    verif_part(input, ';');
    //printf("%s\n","salut");
    return (nw);
}

t_return				*parse_nw(char **input)
{
    t_return *nw;

    if (!(nw = malloc(sizeof(t_ast))))
        exit(0);
    nw->rest = input;
    parse(nw, input);
    return (nw);
}
