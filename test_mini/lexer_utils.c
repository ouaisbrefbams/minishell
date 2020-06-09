
#include "lexer.h"


enum e_token_tag                ret_token_sep_redir_append(char *input, int i)
{
    if (input[i + 1] ==  '>')
        return(LTAG_REDIR_APPEND);
    return (LTAG_REDIR_OUT);

}

enum e_token_tag                ret_token_sep(char *input, int  i)
{
    if (input[i] == ';')
        return(LTAG_AND);
    if (input[i] == '&')
        return(LTAG_END);
    if (input[i]  == '|' && input[i + 1] == '|')
        return(LTAG_OR);
    if(input[i]  == '|')
        return(LTAG_PIPE);
    if (input[i] == '>')
        return(ret_token_sep_redir_append(input,i));
    if (input[i] == '<')
        return(LTAG_REDIR_IN);

    return(0);

}


int                 lexer_sep(char input)
{
    char            *sep;
    int             i;

    i = 0;
    sep = ";&|><()";
    while(sep[i] != '\0')
    {
        if(sep[i] == input)
            return(1);
        i++;
    }
    return (0);
}

int             lexe_space(char *input)
{
    int i;

    i=0;
    while(input[i] == ' ')
        i++;
    return(i);
}

int             simple_cote(char  *input, int i)
{
    int cote;

    cote = 39;
    if (cote == input[i])
        return (1);
    return(0);
}


int             lexer_verif_entre_cote(char *input, int i)
{
    while((input[++i] != '"' || simple_cote(input,i)) && (input[i] != '\0'))
        ;
    //i++;
    //if(input[i] == '"' || simple_cote(input,i))
    //    return(lexer_verif_entre_cote(input, i));
    //if (input[i] == ' ')
    //    while(input[i] == ' ')
    //        i++;
    return(i);
}
