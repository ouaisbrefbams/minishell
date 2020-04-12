
#include "lexer.h"

int                 lexer_sep(char input)
{
    char            *sep;
    int             i;

    i = 0;
    sep = ";&<>|";
    while(sep[i] != '\0')
    {
        if(sep[i] == input)
            return(1);
        i++;
    }
    return (0);
}

int             lexer_verif_entre_cote(char *input, int i)
{
    while((input[++i] != '"' || input[i] == '\'') && (input[i] != '\0'))
        ;
    i++;
    if(input[i] == '"' || input[i] == '\'')
        return(lexer_verif_entre_cote(input, i));
    return(i);
}
